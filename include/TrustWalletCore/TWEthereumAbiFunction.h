// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents Ethereum ABI function
TW_EXPORT_CLASS
struct TWEthereumAbiFunction;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
///
/// \param name function name
/// \return Non-null Ethereum abi function
TW_EXPORT_STATIC_METHOD
struct TWEthereumAbiFunction* _Nonnull TWEthereumAbiFunctionCreateWithString(TWString* _Nonnull name);

/// Deletes a function object created with a 'TWEthereumAbiFunctionCreateWithString' method.
///
/// \param fn Non-null Ethereum abi function
TW_EXPORT_METHOD
void TWEthereumAbiFunctionDelete(struct TWEthereumAbiFunction* _Nonnull fn);

/// Return the function type signature, of the form "baz(int32,uint256)"
///
/// \param fn A Non-null eth abi function
/// \return function type signature as a Non-null string.
TW_EXPORT_METHOD
TWString* _Nonnull TWEthereumAbiFunctionGetType(struct TWEthereumAbiFunction* _Nonnull fn);

/// Methods for adding parameters of the given type (input or output).
/// For output parameters (isOutput=true) a value has to be specified, although usually not need;

/// Add a uint8 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt8(struct TWEthereumAbiFunction* _Nonnull fn, uint8_t val, bool isOutput);

/// Add a uint16 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt16(struct TWEthereumAbiFunction* _Nonnull fn, uint16_t val, bool isOutput);

/// Add a uint32 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt32(struct TWEthereumAbiFunction* _Nonnull fn, uint32_t val, bool isOutput);

/// Add a uint64 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt64(struct TWEthereumAbiFunction* _Nonnull fn, uint64_t val, bool isOutput);

/// Add a uint256 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUInt256(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull val, bool isOutput);

/// Add a uint(bits) type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamUIntN(struct TWEthereumAbiFunction* _Nonnull fn, int bits, TWData* _Nonnull val, bool isOutput);

/// Add a int8 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt8(struct TWEthereumAbiFunction* _Nonnull fn, int8_t val, bool isOutput);

/// Add a int16 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt16(struct TWEthereumAbiFunction* _Nonnull fn, int16_t val, bool isOutput);

/// Add a int32 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt32(struct TWEthereumAbiFunction* _Nonnull fn, int32_t val, bool isOutput);

/// Add a int64 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt64(struct TWEthereumAbiFunction* _Nonnull fn, int64_t val, bool isOutput);

/// Add a int256 type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified (stored in a block of data)
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamInt256(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull val, bool isOutput);

/// Add a int(bits) type parameter
///
/// \param fn A Non-null eth abi function
/// \param bits Number of bits of the integer parameter
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamIntN(struct TWEthereumAbiFunction* _Nonnull fn, int bits, TWData* _Nonnull val, bool isOutput);

/// Add a bool type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBool(struct TWEthereumAbiFunction* _Nonnull fn, bool val, bool isOutput);

/// Add a string type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamString(struct TWEthereumAbiFunction* _Nonnull fn, TWString* _Nonnull val, bool isOutput);

/// Add an address type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamAddress(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull val, bool isOutput);

/// Add a bytes type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBytes(struct TWEthereumAbiFunction* _Nonnull fn, TWData* _Nonnull val, bool isOutput);

/// Add a bytes[N] type parameter
///
/// \param fn A Non-null eth abi function
/// \param size fixed size of the bytes array parameter (val).
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamBytesFix(struct TWEthereumAbiFunction* _Nonnull fn, size_t size, TWData* _Nonnull val, bool isOutput);

/// Add a type[] type parameter
///
/// \param fn A Non-null eth abi function
/// \param val for output parameters, value has to be specified
/// \param isOutput determines if the parameter is an input or output
/// \return the index of the parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddParamArray(struct TWEthereumAbiFunction* _Nonnull fn, bool isOutput);

/// Methods for accessing the value of an output or input parameter, of different types.

/// Get a uint8 type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter.
TW_EXPORT_METHOD
uint8_t TWEthereumAbiFunctionGetParamUInt8(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Get a uint64 type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter.
TW_EXPORT_METHOD
uint64_t TWEthereumAbiFunctionGetParamUInt64(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Get a uint256 type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter stored in a block of data.
TW_EXPORT_METHOD
TWData* _Nonnull TWEthereumAbiFunctionGetParamUInt256(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Get a bool type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter.
TW_EXPORT_METHOD
bool TWEthereumAbiFunctionGetParamBool(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Get a string type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter.
TW_EXPORT_METHOD
TWString* _Nonnull TWEthereumAbiFunctionGetParamString(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Get an address type parameter at the given index
///
/// \param fn A Non-null eth abi function
/// \param idx index for the parameter (0-based).
/// \param isOutput determines if the parameter is an input or output
/// \return the value of the parameter.
TW_EXPORT_METHOD
TWData* _Nonnull TWEthereumAbiFunctionGetParamAddress(struct TWEthereumAbiFunction* _Nonnull fn, int idx, bool isOutput);

/// Methods for adding a parameter of the given type to a top-level input parameter array.  Returns the index of the parameter (0-based).
/// Note that nested ParamArrays are not possible through this API, could be done by using index paths like "1/0"

/// Adding a uint8 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt8(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, uint8_t val);

/// Adding a uint16 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt16(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, uint16_t val);

/// Adding a uint32 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt32(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, uint32_t val);

/// Adding a uint64 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt64(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, uint64_t val);

/// Adding a uint256 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter stored in a block of data
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUInt256(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, TWData* _Nonnull val);

/// Adding a uint[N] type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param bits Number of bits of the integer parameter
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter stored in a block of data
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamUIntN(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int bits, TWData* _Nonnull val);

/// Adding a int8 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt8(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int8_t val);

/// Adding a int16 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt16(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int16_t val);

/// Adding a int32 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt32(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int32_t val);

/// Adding a int64 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt64(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int64_t val);

/// Adding a int256 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter stored in a block of data
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamInt256(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, TWData* _Nonnull val);

/// Adding a int[N] type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param bits Number of bits of the integer parameter
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter stored in a block of data
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamIntN(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, int bits, TWData* _Nonnull val);

/// Adding a bool type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBool(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, bool val);

/// Adding a string type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamString(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, TWString* _Nonnull val);

/// Adding an address type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamAddress(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, TWData* _Nonnull val);

/// Adding a bytes type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBytes(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, TWData* _Nonnull val);

/// Adding a int64 type parameter of to the top-level input parameter array
///
/// \param fn A Non-null eth abi function
/// \param arrayIdx array index for the abi function (0-based).
/// \param size fixed size of the bytes array parameter (val).
/// \param val the value of the parameter
/// \return the index of the added parameter (0-based).
TW_EXPORT_METHOD
TW_METHOD_DISCARDABLE_RESULT
int TWEthereumAbiFunctionAddInArrayParamBytesFix(struct TWEthereumAbiFunction* _Nonnull fn, int arrayIdx, size_t size, TWData* _Nonnull val);

TW_EXTERN_C_END
