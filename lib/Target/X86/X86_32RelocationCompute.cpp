//===- X86_32RelocationCompute.cpp---------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "X86_32LLVMExtern.h"
#include "llvm/Support/MathExtras.h"

namespace {

bool fitsRange(uint64_t Value, bool IsSigned, x86_32::EncodingType Encoding) {
  switch (Encoding) {
  case x86_32::EncTy_8:
    return IsSigned ? llvm::isInt<8>(static_cast<int64_t>(Value))
                    : llvm::isUInt<8>(Value);
  case x86_32::EncTy_16:
    return IsSigned ? llvm::isInt<16>(static_cast<int64_t>(Value))
                    : llvm::isUInt<16>(Value);
  case x86_32::EncTy_32:
    return IsSigned ? llvm::isInt<32>(static_cast<int64_t>(Value))
                    : llvm::isUInt<32>(Value);
  case x86_32::EncTy_None:
    return true;
  }
  return false;
}

} // namespace

namespace x86_32 {

uint32_t doRelocX86_32(const RelocationInfo &RelocInfo, uint32_t Instruction,
                       uint64_t Value) {
  switch (RelocInfo.EncType) {
  case EncTy_8:
    return (Instruction & 0xFFFFFF00U) | (static_cast<uint32_t>(Value) & 0xFFU);
  case EncTy_16:
    return (Instruction & 0xFFFF0000U) |
           (static_cast<uint32_t>(Value) & 0xFFFFU);
  case EncTy_32:
    return static_cast<uint32_t>(Value);
  case EncTy_None:
    return Instruction;
  }
  return Instruction;
}

bool verifyRangeX86_32(const RelocationInfo &RelocInfo, uint64_t Value) {
  if (!RelocInfo.VerifyRange)
    return true;
  return fitsRange(Value >> RelocInfo.Shift, RelocInfo.IsSigned,
                   RelocInfo.EncType);
}

unsigned getNumberOfBitsX86_32(EncodingType Type) {
  switch (Type) {
  case EncTy_8:
    return 8;
  case EncTy_16:
    return 16;
  case EncTy_32:
    return 32;
  case EncTy_None:
    return 0;
  }
  return 0;
}

} // namespace x86_32
