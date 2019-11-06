// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWEthereumAbiFunction;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
TW_EXPORT_STATIC_METHOD
struct TWEthereumAbiFunction *_Nullable TWEthereumAbiFunctionCreateWithString(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWEthereumAbiFunctionCreateWithString' method.
TW_EXPORT_METHOD
void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Return the function type signature, of the form "baz(int32,uint256)"
TW_EXPORT_METHOD
TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Methods for adding an input parameter of the given type.  Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, uint64_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, bool val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamString(struct TWEthereumAbiFunction *_Nonnull func_in, TWString *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamByteArray(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamByteArrayFix(struct TWEthereumAbiFunction *_Nonnull func_in, size_t count, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInParamArray(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Methods for adding an output parameter of the given type.  Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddOutParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddOutParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in);

/// Methods for accessing the value of an output parameter, of different types.
TW_EXPORT_METHOD
uint64_t TWEthereumAbiFunctionGetOutParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int idx);
TWData *_Nonnull TWEthereumAbiFunctionGetOutParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int idx);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);

TW_EXTERN_C_END
