/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_JNI_QUICK_ARM64_CALLING_CONVENTION_ARM64_H_
#define ART_COMPILER_JNI_QUICK_ARM64_CALLING_CONVENTION_ARM64_H_

#include "base/macros.h"
#include "base/pointer_size.h"
#include "jni/quick/calling_convention.h"

namespace art HIDDEN {
namespace arm64 {

class Arm64ManagedRuntimeCallingConvention final : public ManagedRuntimeCallingConvention {
 public:
  Arm64ManagedRuntimeCallingConvention(
      bool is_static, bool is_synchronized, std::string_view shorty)
      : ManagedRuntimeCallingConvention(is_static,
                                        is_synchronized,
                                        shorty,
                                        PointerSize::k64) {}
  ~Arm64ManagedRuntimeCallingConvention() override {}
  // Calling convention
  ManagedRegister ReturnRegister() const override;
  // Managed runtime calling convention
  ManagedRegister MethodRegister() override;
  ManagedRegister ArgumentRegisterForMethodExitHook() override;
  bool IsCurrentParamInRegister() override;
  bool IsCurrentParamOnStack() override;
  ManagedRegister CurrentParamRegister() override;
  FrameOffset CurrentParamStackOffset() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Arm64ManagedRuntimeCallingConvention);
};

class Arm64JniCallingConvention final : public JniCallingConvention {
 public:
  Arm64JniCallingConvention(bool is_static,
                            bool is_synchronized,
                            bool is_fast_native,
                            bool is_critical_native,
                            std::string_view shorty);
  ~Arm64JniCallingConvention() override {}
  // Calling convention
  ManagedRegister ReturnRegister() const override;
  ManagedRegister IntReturnRegister() const override;
  // JNI calling convention
  size_t FrameSize() const override;
  size_t OutFrameSize() const override;
  ArrayRef<const ManagedRegister> CalleeSaveRegisters() const override;
  ArrayRef<const ManagedRegister> CalleeSaveScratchRegisters() const override;
  ArrayRef<const ManagedRegister> ArgumentScratchRegisters() const override;
  uint32_t CoreSpillMask() const override;
  uint32_t FpSpillMask() const override;
  bool IsCurrentParamInRegister() override;
  bool IsCurrentParamOnStack() override;
  ManagedRegister CurrentParamRegister() override;
  FrameOffset CurrentParamStackOffset() override;

  // aarch64 calling convention leaves upper bits undefined.
  bool RequiresSmallResultTypeExtension() const override {
    return HasSmallReturnType();
  }

  // Locking argument register, used to pass the synchronization object for calls
  // to `JniLockObject()` and `JniUnlockObject()`.
  ManagedRegister LockingArgumentRegister() const override;

  // Hidden argument register, used to pass the method pointer for @CriticalNative call.
  ManagedRegister HiddenArgumentRegister() const override;

  // Whether to use tail call (used only for @CriticalNative).
  bool UseTailCall() const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Arm64JniCallingConvention);
};

}  // namespace arm64
}  // namespace art

#endif  // ART_COMPILER_JNI_QUICK_ARM64_CALLING_CONVENTION_ARM64_H_
