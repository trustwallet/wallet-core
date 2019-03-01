// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWUInt256 {
    uint8_t bytes[32];
};

TW_EXPORT_STATIC_PROPERTY
struct TWUInt256 TWUInt256Zero(void);

TW_EXPORT_STATIC_PROPERTY
struct TWUInt256 TWUInt256One(void);

TW_EXPORT_STATIC_METHOD
bool TWUInt256InitWithData(struct TWUInt256 *_Nonnull instance, TWData *_Nonnull data);

TW_EXPORT_STATIC_METHOD
bool TWUInt256InitWithString(struct TWUInt256 *_Nonnull instance, TWString *_Nonnull string);

TW_EXPORT_STATIC_METHOD
void TWUInt256InitWithUInt32(struct TWUInt256 *_Nonnull instance, uint32_t);

TW_EXPORT_STATIC_METHOD
void TWUInt256InitWithUInt64(struct TWUInt256 *_Nonnull instance, uint64_t);

TW_EXPORT_PROPERTY
bool TWUInt256IsZero(struct TWUInt256 value);

TW_EXPORT_PROPERTY
uint32_t TWUInt256UInt32Value(struct TWUInt256 value);

TW_EXPORT_PROPERTY
uint64_t TWUInt256UInt64Value(struct TWUInt256 value);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWUInt256Data(struct TWUInt256 value);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWUInt256Description(struct TWUInt256 value);

TW_EXPORT_STATIC_METHOD
bool TWUInt256Equal(struct TWUInt256 lhs, struct TWUInt256 rhs);

TW_EXPORT_STATIC_METHOD
bool TWUInt256Less(struct TWUInt256 lhs, struct TWUInt256 rhs);

/// Formats the number as a string with the given number of decimals.
TW_EXPORT_METHOD
TWString *_Nonnull TWUInt256Format(struct TWUInt256 value, int decimals);

TW_EXTERN_C_END
