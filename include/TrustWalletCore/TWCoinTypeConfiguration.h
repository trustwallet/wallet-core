// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"
#include "TWCoinType.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWCoinTypeConfiguration { 
    uint8_t unused; // C doesn't allow zero-sized struct
};

/// Returns stock symbol of coin
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCoinTypeConfigurationGetSymbol(enum TWCoinType type);

/// Returns max count decimal places for minimal coin unit
TW_EXPORT_STATIC_METHOD
int TWCoinTypeConfigurationGetDecimals(enum TWCoinType type);

/// Returns transaction url in blockchain explorer
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCoinTypeConfigurationGetTransactionURL(enum TWCoinType type, TWString *_Nonnull transactionID);

/// Returns full name of coin in lower case
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCoinTypeConfigurationGetID(enum TWCoinType type);

/// Returns full name of coin
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCoinTypeConfigurationGetName(enum TWCoinType type);

TW_EXPORT_STATIC_METHOD
uint8_t TWCoinTypeConfigurationGetP2PKHPrefix(enum TWCoinType coin);

TW_EXPORT_STATIC_METHOD
uint8_t TWCoinTypeConfigurationGetP2SHPrefix(enum TWCoinType coin);

TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCoinTypeConfigurationGetHRPPrefix(enum TWCoinType coin);

TW_EXTERN_C_END
