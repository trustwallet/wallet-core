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
struct TWHarmonyAbiFunction;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
TW_EXPORT_STATIC_METHOD
struct TWHarmonyAbiFunction *_Nullable TWHarmonyAbiFunctionCreateWithString(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWHarmonyAbiFunctionCreateWithString' method.
TW_EXPORT_METHOD
void TWHarmonyAbiFunctionDelete(struct TWHarmonyAbiFunction *_Nonnull func_in);

/// Return the function type signature, of the form "baz(int32,uint256)"
TW_EXPORT_METHOD
TWString *_Nonnull TWHarmonyAbiFunctionGetType(struct TWHarmonyAbiFunction *_Nonnull func_in);

/// Methods for adding parameters of the given type (input or output).  
/// For output parameters (isOutput=true) a value has to be specified, although usually not needd.
/// Returns the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, uint8_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, uint16_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, uint32_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, uint64_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamUIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int8_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int16_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int32_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int64_t val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int bits, TWData *_Nonnull val, bool isOutput);
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, bool val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, TWString *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamBytes(struct TWHarmonyAbiFunction *_Nonnull func_in, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamBytesFix(struct TWHarmonyAbiFunction *_Nonnull func_in, size_t count_in, TWData *_Nonnull val, bool isOutput);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddParamArray(struct TWHarmonyAbiFunction *_Nonnull func_in, bool isOutput);

/// Methods for accessing the value of an output or input parameter, of different types.
TW_EXPORT_METHOD
uint8_t TWHarmonyAbiFunctionGetParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
uint64_t TWHarmonyAbiFunctionGetParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWHarmonyAbiFunctionGetParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
bool TWHarmonyAbiFunctionGetParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWString *_Nonnull TWHarmonyAbiFunctionGetParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);
TW_EXPORT_METHOD
TWData *_Nonnull TWHarmonyAbiFunctionGetParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, int idx, bool isOutput);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint8_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint16_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint32_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, uint64_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamUIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamInt8(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int8_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamInt16(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int16_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamInt32(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int32_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamInt64(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int64_t val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamInt256(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamIntN(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, int bits, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamBool(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, bool val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamString(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWString *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamAddress(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamBytes(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, TWData *_Nonnull val);

TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWHarmonyAbiFunctionAddInArrayParamBytesFix(struct TWHarmonyAbiFunction *_Nonnull func_in, int arrayIdx, size_t count_in, TWData *_Nonnull val);

TW_EXTERN_C_END
