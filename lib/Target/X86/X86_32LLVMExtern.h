//===- X86_32LLVMExtern.h------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef X86_32_LLVM_EXTERN_H
#define X86_32_LLVM_EXTERN_H

#include <cstdint>

namespace x86_32 {

enum EncodingType { EncTy_None, EncTy_8, EncTy_16, EncTy_32 };

struct RelocationInfo {
  const char *Name;
  const uint32_t Type;
  const EncodingType EncType;
  const uint32_t Shift;
  const bool VerifyRange;
  const bool IsSigned;
  const uint32_t Size;
};

uint32_t doRelocX86_32(const RelocationInfo &RelocInfo, uint32_t Instruction,
                       uint64_t Value);

bool verifyRangeX86_32(const RelocationInfo &RelocInfo, uint64_t Value);

unsigned getNumberOfBitsX86_32(EncodingType Type);

} // namespace x86_32

#endif
