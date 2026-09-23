//===- X86_32Relocator.cpp-----------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//
// i386 uses Elf32_Rel records. The addend is therefore read from the target
// field rather than from the relocation record.
//===----------------------------------------------------------------------===//

#include "X86_32Relocator.h"
#include "X86_32LLVMExtern.h"
#include "X86_32RelocationInfo.h"
#include "eld/Config/LinkerConfig.h"
#include "eld/Support/MsgHandling.h"
#include "eld/SymbolResolver/LDSymbol.h"
#include "eld/SymbolResolver/Resolver.h"
#include "llvm/ADT/Twine.h"
#include "llvm/BinaryFormat/ELF.h"

namespace eld {

namespace {

bool isSupportedRelocation(Relocation::Type Type) {
  switch (Type) {
  case llvm::ELF::R_386_NONE:
  case llvm::ELF::R_386_8:
  case llvm::ELF::R_386_16:
  case llvm::ELF::R_386_32:
    return true;
  default:
    return false;
  }
}

Relocator::Result applyAbsolute(Relocation &Reloc, X86_32Relocator &Parent) {
  const x86_32::RelocationInfo &Info = x86_32::x86_32Relocs[Reloc.type()];
  Relocator::Address Symbol = Reloc.symValue(Parent.module());

  // i386 uses SHT_REL: the addend is the value already stored at the target.
  Relocator::DWord Addend = Reloc.target() + Reloc.addend();
  ResolveInfo *SymInfo = Reloc.symInfo();
  if (SymInfo && SymInfo->isWeakUndef() &&
      Parent.config().codeGenType() == LinkerConfig::Exec)
    Symbol = 0;

  const Relocator::DWord Result = Symbol + Addend;
  if (!x86_32::verifyRangeX86_32(Info, Result)) {
    const unsigned Bits = x86_32::getNumberOfBitsX86_32(Info.EncType);
    if (Info.IsSigned)
      return checkSignedRange(Reloc, Parent, Result, Bits);
    return reportUnsignedOverflow(Reloc, Parent, Result, Bits);
  }

  Reloc.target() = x86_32::doRelocX86_32(
      Info, static_cast<uint32_t>(Reloc.target()), Result);
  return Relocator::OK;
}

} // namespace

X86_32Relocator::X86_32Relocator(X86_32LDBackend &pParent,
                                 LinkerConfig &pConfig, Module &pModule)
    : Relocator(pConfig, pModule), m_Target(pParent) {}

Relocator::Result X86_32Relocator::applyRelocation(Relocation &pRelocation) {
  const Relocation::Type Type = pRelocation.type();
  if (Type >= x86_32::X86_32_MAXRELOCS)
    return Relocator::Unknown;

  ResolveInfo *SymInfo = pRelocation.symInfo();
  if (SymInfo) {
    LDSymbol *OutSymbol = SymInfo->outSymbol();
    if (OutSymbol && OutSymbol->hasFragRef()) {
      ELFSection *Section = OutSymbol->fragRef()->frag()->getOwningSection();
      if (Section->isDiscard() || (Section->getOutputSection() &&
                                   Section->getOutputSection()->isDiscard())) {
        std::lock_guard<std::mutex> RelocGuard(m_RelocMutex);
        issueUndefRef(pRelocation, *Section->getInputFile(), Section);
        return Relocator::OK;
      }
    }
  }

  if (!isSupportedRelocation(Type))
    return Relocator::Unsupport;

  if (Type == llvm::ELF::R_386_NONE)
    return Relocator::OK;
  return applyAbsolute(pRelocation, *this);
}

void X86_32Relocator::scanRelocation(Relocation &pReloc, eld::IRBuilder &,
                                     ELFSection &pSection,
                                     InputFile &pInputFile, CopyRelocs &) {
  if (LinkerConfig::Object == config().codeGenType())
    return;

  if (pReloc.type() >= x86_32::X86_32_MAXRELOCS ||
      !isSupportedRelocation(pReloc.type())) {
    config().raise(Diag::unsupported_reloc)
        << pReloc.type() << pSection.getDecoratedName(config().options())
        << pInputFile.getInput()->decoratedPath();
    return;
  }

  ResolveInfo *SymInfo = pReloc.symInfo();
  if (!SymInfo) {
    assert(pReloc.type() == llvm::ELF::R_386_NONE &&
           "ResolveInfo of relocation not set while scanRelocation");
    return;
  }

  if (m_Module.getPrinter()->traceReloc()) {
    std::lock_guard<std::mutex> RelocGuard(m_RelocMutex);
    const std::string RelocName = getName(pReloc.type());
    if (config().options().traceReloc(RelocName))
      config().raise(Diag::reloc_trace)
          << RelocName << SymInfo->name()
          << pInputFile.getInput()->decoratedPath();
  }

  if (SymInfo->isUndef() || SymInfo->isBitCode()) {
    std::lock_guard<std::mutex> RelocGuard(m_RelocMutex);
    if (m_Target.canIssueUndef(SymInfo)) {
      if (SymInfo->visibility() != ResolveInfo::Default)
        issueInvisibleRef(pReloc, pInputFile);
      issueUndefRef(pReloc, pInputFile, &pSection);
    }
  }

  // The absolute-relocation patch does not reserve GOT, PLT, or dynamic
  // relocation entries. Keep all relocation handling in this common path so
  // later i386 relocation patches do not need duplicate local/global logic.
}

void X86_32Relocator::partialScanRelocation(Relocation &pReloc,
                                            const ELFSection &) {
  if (!pReloc.symInfo())
    return;
  pReloc.updateAddend(m_Module);
  if (pReloc.symInfo()->type() != ResolveInfo::Section)
    return;

  LDSymbol *InputSymbol = pReloc.symInfo()->outSymbol();
  assert(InputSymbol->hasFragRef());
  ELFSection *OutputSection = InputSymbol->fragRef()->getOutputELFSection();
  pReloc.setSymInfo(m_Module.getSectionSymbol(OutputSection));
}

const char *X86_32Relocator::getName(Relocation::Type pType) const {
  if (pType >= x86_32::X86_32_MAXRELOCS)
    return "INVALID_RELOC";
  return x86_32::x86_32Relocs[pType].Name;
}

Relocation::Size X86_32Relocator::getSize(Relocation::Type pType) const {
  if (pType >= x86_32::X86_32_MAXRELOCS)
    return 0;
  return x86_32::x86_32Relocs[pType].Size;
}

uint32_t X86_32Relocator::getNumRelocs() const {
  return x86_32::X86_32_MAXRELOCS;
}

} // namespace eld
