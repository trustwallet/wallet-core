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

/// Methods for adding parameters of the given type (input or output).  
/// For output parameters (isOutput=true) a value has to be specified, although usually not needd.
/// Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int8_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int16_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int32_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int64_t val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, bool val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, size_t count_in, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction *_Nonnull func_in, bool isOutput);

/// Methods for accessing the value of an output or input parameter, of different types.
TW_EXPORT_METHOD
uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWString *_Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int idx, bool isOutput);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint8_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint16_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint32_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamUIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamInt8(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int8_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamInt16(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int16_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamInt32(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int32_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamInt64(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int64_t val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamInt256(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamIntN(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamBool(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, bool val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamString(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWString *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamAddress(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamBytes(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);
TW_EXPORT_METHOD
int TWEthereumAbiFunctionAddInArrayParamBytesFix(struct TWEthereumAbiFunction *_Nonnull func_in, int arrayIdx, size_t count_in, TWData *_Nonnull val);

TW_EXTERN_C_END
