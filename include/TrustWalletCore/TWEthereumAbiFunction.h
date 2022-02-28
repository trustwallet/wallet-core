// Copyright © 2017-2020 Trust Wallet.
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
struct TWEthereumAbiFunction *_Nonnull TWEthereumAbiFunctionCreateWithString(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWEthereumAbiFunctionCreateWithString' method.
TW_EXPORT_METHOD
void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction *_Nonnull fn);

/// Return the function type signature, of the form "baz(int32,uint256)"
TW_EXPORT_METHOD
TWString *_Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction *_Nonnull fn);

/// Methods for adding parameters of the given type (input or output).  
/// For output parameters (isOutput=true) a value has to be specified, although usually not needd.
/// Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction *_Nonnull fn, uint8_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction *_Nonnull fn, uint16_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction *_Nonnull fn, uint32_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction *_Nonnull fn, uint64_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull fn, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction *_Nonnull fn, int bits, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction *_Nonnull fn, int8_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction *_Nonnull fn, int16_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull fn, int32_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction *_Nonnull fn, int64_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction *_Nonnull fn, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction *_Nonnull fn, int bits, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull fn, bool val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull fn, TWString *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction *_Nonnull fn, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction *_Nonnull fn, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction *_Nonnull fn, size_t size, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull fn, bool isOutput);

/// Methods for accessing the value of an output or input parameter, of different types.
TW_EXPORT_METHOD
uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);
TW_EXPORT_METHOD
uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);
TW_EXPORT_METHOD
bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);
TW_EXPORT_METHOD
TWString *_Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction *_Nonnull fn, int idx, bool isOutput);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt8(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, uint8_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt16(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, uint16_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, uint32_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt64(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, uint64_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUIntN(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int bits, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt8(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int8_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt16(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int16_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt32(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int32_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt64(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int64_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt256(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamIntN(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, int bits, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBool(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, bool val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamString(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, TWString *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamAddress(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBytes(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBytesFix(struct TWEthereumAbiFunction *_Nonnull fn, int arrayIdx, size_t size, TWData *_Nonnull val);

TW_EXTERN_C_END
