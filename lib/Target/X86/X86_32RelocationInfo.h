//===- X86_32RelocationInfo.h--------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//
// Per-relocation properties for the i386 ELF32 relocations.
//
// The table is indexed by the ELF relocation number. Keep every slot from
// R_386_NONE through R_386_GOT32X so unsupported relocations are rejected
// explicitly instead of indexing unrelated data.
//===----------------------------------------------------------------------===//

#ifndef X86_32_RELOCATION_INFO_H
#define X86_32_RELOCATION_INFO_H

#include "X86_32LLVMExtern.h"
#include "llvm/BinaryFormat/ELF.h"

namespace x86_32 {

#define X86_32_UNSUPPORTED_RELOC(Name, Number)                                 \
  {Name, Number, EncTy_None, 0, false, false, 0}

inline constexpr RelocationInfo x86_32Relocs[] = {
    {"R_386_NONE", llvm::ELF::R_386_NONE, EncTy_None, 0, false, false, 0},
    {"R_386_32", llvm::ELF::R_386_32, EncTy_32, 0, true, false, 32},
    X86_32_UNSUPPORTED_RELOC("R_386_PC32", llvm::ELF::R_386_PC32),
    X86_32_UNSUPPORTED_RELOC("R_386_GOT32", llvm::ELF::R_386_GOT32),
    X86_32_UNSUPPORTED_RELOC("R_386_PLT32", llvm::ELF::R_386_PLT32),
    X86_32_UNSUPPORTED_RELOC("R_386_COPY", llvm::ELF::R_386_COPY),
    X86_32_UNSUPPORTED_RELOC("R_386_GLOB_DAT", llvm::ELF::R_386_GLOB_DAT),
    X86_32_UNSUPPORTED_RELOC("R_386_JUMP_SLOT", llvm::ELF::R_386_JUMP_SLOT),
    X86_32_UNSUPPORTED_RELOC("R_386_RELATIVE", llvm::ELF::R_386_RELATIVE),
    X86_32_UNSUPPORTED_RELOC("R_386_GOTOFF", llvm::ELF::R_386_GOTOFF),
    X86_32_UNSUPPORTED_RELOC("R_386_GOTPC", llvm::ELF::R_386_GOTPC),
    X86_32_UNSUPPORTED_RELOC("R_386_32PLT", llvm::ELF::R_386_32PLT),
    X86_32_UNSUPPORTED_RELOC("R_386_RESERVED_12", 12),
    X86_32_UNSUPPORTED_RELOC("R_386_RESERVED_13", 13),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_TPOFF", llvm::ELF::R_386_TLS_TPOFF),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_IE", llvm::ELF::R_386_TLS_IE),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GOTIE", llvm::ELF::R_386_TLS_GOTIE),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LE", llvm::ELF::R_386_TLS_LE),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GD", llvm::ELF::R_386_TLS_GD),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDM", llvm::ELF::R_386_TLS_LDM),
    {"R_386_16", llvm::ELF::R_386_16, EncTy_16, 0, true, false, 16},
    X86_32_UNSUPPORTED_RELOC("R_386_PC16", llvm::ELF::R_386_PC16),
    {"R_386_8", llvm::ELF::R_386_8, EncTy_8, 0, true, false, 8},
    X86_32_UNSUPPORTED_RELOC("R_386_PC8", llvm::ELF::R_386_PC8),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GD_32", llvm::ELF::R_386_TLS_GD_32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GD_PUSH", llvm::ELF::R_386_TLS_GD_PUSH),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GD_CALL", llvm::ELF::R_386_TLS_GD_CALL),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GD_POP", llvm::ELF::R_386_TLS_GD_POP),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDM_32", llvm::ELF::R_386_TLS_LDM_32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDM_PUSH",
                             llvm::ELF::R_386_TLS_LDM_PUSH),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDM_CALL",
                             llvm::ELF::R_386_TLS_LDM_CALL),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDM_POP", llvm::ELF::R_386_TLS_LDM_POP),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LDO_32", llvm::ELF::R_386_TLS_LDO_32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_IE_32", llvm::ELF::R_386_TLS_IE_32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_LE_32", llvm::ELF::R_386_TLS_LE_32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_DTPMOD32",
                             llvm::ELF::R_386_TLS_DTPMOD32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_DTPOFF32",
                             llvm::ELF::R_386_TLS_DTPOFF32),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_TPOFF32", llvm::ELF::R_386_TLS_TPOFF32),
    X86_32_UNSUPPORTED_RELOC("R_386_RESERVED_38", 38),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_GOTDESC", llvm::ELF::R_386_TLS_GOTDESC),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_DESC_CALL",
                             llvm::ELF::R_386_TLS_DESC_CALL),
    X86_32_UNSUPPORTED_RELOC("R_386_TLS_DESC", llvm::ELF::R_386_TLS_DESC),
    X86_32_UNSUPPORTED_RELOC("R_386_IRELATIVE", llvm::ELF::R_386_IRELATIVE),
    X86_32_UNSUPPORTED_RELOC("R_386_GOT32X", llvm::ELF::R_386_GOT32X),
};

inline constexpr uint32_t X86_32_MAXRELOCS =
    sizeof(x86_32Relocs) / sizeof(x86_32Relocs[0]);

#undef X86_32_UNSUPPORTED_RELOC

} // namespace x86_32

#endif
