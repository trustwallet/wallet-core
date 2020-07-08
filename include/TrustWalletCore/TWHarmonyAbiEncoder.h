// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

// Wrapper class for Harmony ABI encoding & decoding.  Also builder for Function objects.
// See also TWHarmonyAbiFunction.

TW_EXTERN_C_BEGIN

struct TWHarmonyAbiFunction;

TW_EXPORT_CLASS
struct TWHarmonyAbiEncoder;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
/// Note: Create name is reserved for own-class creation in the codegen toolchain
TW_EXPORT_STATIC_METHOD
struct TWHarmonyAbiFunction *_Nullable TWHarmonyAbiEncoderBuildFunction(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWHarmonyAbiEncoderBuildFunction' method.
/// Note: func is a reserved keyword in codegenerated swift.
TW_EXPORT_STATIC_METHOD
void TWHarmonyAbiEncoderDeleteFunction(struct TWHarmonyAbiFunction *_Nonnull func_in);

/// Encode function to Eth ABI binary
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWHarmonyAbiEncoderEncode(struct TWHarmonyAbiFunction *_Nonnull func_in);

/// Decode function output from Eth ABI binary, fill output parameters
TW_EXPORT_STATIC_METHOD
bool TWHarmonyAbiEncoderDecodeOutput(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull encoded);

TW_EXTERN_C_END
