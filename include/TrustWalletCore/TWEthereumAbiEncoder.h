// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWData.h>

// Wrapper class for Ethereum ABI encoding & decoding.  Also builder for Function objects.
// See also TWEthereumAbiFunction.

TW_EXTERN_C_BEGIN

struct TWEthereumAbiFunction;

TW_EXPORT_CLASS
struct TWEthereumAbiEncoder;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
/// Note: Create name is reserved for own-class creation in the codegen toolchain
TW_EXPORT_STATIC_METHOD
struct TWEthereumAbiFunction *_Nullable TWEthereumAbiEncoderBuildFunction(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWEthereumAbiEncoderBuildFunction' method.
/// Note: func is a reserved keyword in codegenerated swift.
TW_EXPORT_STATIC_METHOD
void TWEthereumAbiEncoderDeleteFunction(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Encode function to Eth ABI binary
TW_EXPORT_STATIC_METHOD
TWData*_Nonnull TWEthereumAbiEncoderEncode(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Decode function output from Eth ABI binary, fill output parameters
TW_EXPORT_STATIC_METHOD
bool TWEthereumAbiEncoderDecodeOutput(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull encoded);

TW_EXTERN_C_END
