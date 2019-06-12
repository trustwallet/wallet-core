// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE from \coins.json, changes made here WILL BE LOST.
//

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

///  Registered human-readable parts for BIP-0173
///
/// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0173.md
TW_EXPORT_ENUM()
enum TWHRP {
    TWHRPUnknown     /* "" */,
    TWHRPBinance     /* "bnb" */,
    TWHRPBitcoin     /* "bc" */,
    TWHRPBitcoinCash     /* "bitcoincash" */,
    TWHRPCosmos     /* "cosmos" */,
    TWHRPDigiByte     /* "dgb" */,
    TWHRPGroestlcoin     /* "grs" */,
    TWHRPLitecoin     /* "ltc" */,
    TWHRPQtum     /* "qc" */,
    TWHRPViacoin     /* "via" */,
    TWHRPZilliqa     /* "zil" */,
};

static const char *_Nonnull HRP_BINANCE = "bnb";
static const char *_Nonnull HRP_BITCOIN = "bc";
static const char *_Nonnull HRP_BITCOINCASH = "bitcoincash";
static const char *_Nonnull HRP_COSMOS = "cosmos";
static const char *_Nonnull HRP_DIGIBYTE = "dgb";
static const char *_Nonnull HRP_GROESTLCOIN = "grs";
static const char *_Nonnull HRP_LITECOIN = "ltc";
static const char *_Nonnull HRP_QTUM = "qc";
static const char *_Nonnull HRP_VIACOIN = "via";
static const char *_Nonnull HRP_ZILLIQA = "zil";

const char *_Nullable stringForHRP(enum TWHRP hrp);
enum TWHRP hrpForString(const char *_Nonnull string);

TW_EXTERN_C_END
