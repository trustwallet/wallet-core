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
struct TWEthAbiFunction;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
TW_EXPORT_STATIC_METHOD
struct TWEthAbiFunction *_Nullable TWEthAbiFunctionCreateWithString(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWEthAbiFunctionCreateWithString' method.
TW_EXPORT_METHOD
void TWEthAbiFunctionDelete(struct TWEthAbiFunction *_Nonnull func);

/// Return the function type signature, of the form "baz(int32,uint256)"
TW_EXPORT_METHOD
TWString *_Nonnull TWEthAbiFunctionGetType(struct TWEthAbiFunction *_Nonnull func);

/// Methods for adding an input parameter of the given type.  Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamInt32(struct TWEthAbiFunction *_Nonnull func, uint64_t val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamUInt256(struct TWEthAbiFunction *_Nonnull func, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamBool(struct TWEthAbiFunction *_Nonnull func, bool val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamString(struct TWEthAbiFunction *_Nonnull func, TWString *_Nonnull val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamByteArray(struct TWEthAbiFunction *_Nonnull func, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamByteArrayFix(struct TWEthAbiFunction *_Nonnull func, size_t count, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInParamArray(struct TWEthAbiFunction *_Nonnull func);

/// Methods for adding an output parameter of the given type.  Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
int TWEthAbiFunctionAddOutParamInt32(struct TWEthAbiFunction *_Nonnull func);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddOutParamUInt256(struct TWEthAbiFunction *_Nonnull func);

/// Methods for accessing the value of an output parameter, of different types.
TW_EXPORT_METHOD
uint64_t TWEthAbiFunctionGetOutParamInt32(struct TWEthAbiFunction *_Nonnull func, int idx);
TWData *_Nonnull TWEthAbiFunctionGetOutParamUInt256(struct TWEthAbiFunction *_Nonnull func, int idx);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInArrayParamUInt32(struct TWEthAbiFunction *_Nonnull func, int arrayIdx, uint64_t val);
TW_EXPORT_METHOD
int TWEthAbiFunctionAddInArrayParamUInt256(struct TWEthAbiFunction *_Nonnull func, int arrayIdx, TWData *_Nonnull val);

/// Encode function to Eth ABI binary
TW_EXPORT_METHOD
TWData*_Nonnull TWEthAbiFunctionEncode(struct TWEthAbiFunction *_Nonnull func);

TW_EXTERN_C_END
