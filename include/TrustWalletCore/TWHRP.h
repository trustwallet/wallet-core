// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE from \registry.json, changes made here WILL BE LOST.
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
    TWHRPBitcoin     /* "bc" */,
    TWHRPLitecoin     /* "ltc" */,
    TWHRPViacoin     /* "via" */,
    TWHRPGroestlcoin     /* "grs" */,
    TWHRPDigiByte     /* "dgb" */,
    TWHRPMonacoin     /* "mona" */,
    TWHRPSyscoin     /* "sys" */,
    TWHRPVerge     /* "vg" */,
    TWHRPCosmos     /* "cosmos" */,
    TWHRPBitcoinCash     /* "bitcoincash" */,
    TWHRPBitcoinGold     /* "btg" */,
    TWHRPIoTeX     /* "io" */,
    TWHRPNervos     /* "ckb" */,
    TWHRPZilliqa     /* "zil" */,
    TWHRPTerra     /* "terra" */,
    TWHRPCryptoOrg     /* "cro" */,
    TWHRPKava     /* "kava" */,
    TWHRPOasis     /* "oasis" */,
    TWHRPBluzelle     /* "bluzelle" */,
    TWHRPBandChain     /* "band" */,
    TWHRPMultiversX     /* "erd" */,
    TWHRPSecret     /* "secret" */,
    TWHRPAgoric     /* "agoric" */,
    TWHRPBinance     /* "bnb" */,
    TWHRPECash     /* "ecash" */,
    TWHRPTHORChain     /* "thor" */,
    TWHRPBitcoinDiamond     /* "bcd" */,
    TWHRPHarmony     /* "one" */,
    TWHRPCardano     /* "addr" */,
    TWHRPQtum     /* "qc" */,
    TWHRPPactus     /* "pc" */,
    TWHRPStratis     /* "strax" */,
    TWHRPNativeInjective     /* "inj" */,
    TWHRPOsmosis     /* "osmo" */,
    TWHRPTerraV2     /* "terra" */,
    TWHRPCoreum     /* "core" */,
    TWHRPNativeZetaChain     /* "zeta" */,
    TWHRPNativeCanto     /* "canto" */,
    TWHRPSommelier     /* "somm" */,
    TWHRPFetchAI     /* "fetch" */,
    TWHRPMars     /* "mars" */,
    TWHRPUmee     /* "umee" */,
    TWHRPQuasar     /* "quasar" */,
    TWHRPPersistence     /* "persistence" */,
    TWHRPAkash     /* "akash" */,
    TWHRPNoble     /* "noble" */,
    TWHRPSei     /* "sei" */,
    TWHRPStargaze     /* "stars" */,
    TWHRPNativeEvmos     /* "evmos" */,
    TWHRPTia     /* "celestia" */,
    TWHRPDydx     /* "dydx" */,
    TWHRPJuno     /* "juno" */,
    TWHRPTBinance     /* "tbnb" */,
    TWHRPStride     /* "stride" */,
    TWHRPAxelar     /* "axelar" */,
    TWHRPCrescent     /* "cre" */,
    TWHRPKujira     /* "kujira" */,
    TWHRPComdex     /* "comdex" */,
    TWHRPNeutron     /* "neutron" */,
};

static const char *_Nonnull HRP_BITCOIN = "bc";
static const char *_Nonnull HRP_LITECOIN = "ltc";
static const char *_Nonnull HRP_VIACOIN = "via";
static const char *_Nonnull HRP_GROESTLCOIN = "grs";
static const char *_Nonnull HRP_DIGIBYTE = "dgb";
static const char *_Nonnull HRP_MONACOIN = "mona";
static const char *_Nonnull HRP_SYSCOIN = "sys";
static const char *_Nonnull HRP_VERGE = "vg";
static const char *_Nonnull HRP_COSMOS = "cosmos";
static const char *_Nonnull HRP_BITCOINCASH = "bitcoincash";
static const char *_Nonnull HRP_BITCOINGOLD = "btg";
static const char *_Nonnull HRP_IOTEX = "io";
static const char *_Nonnull HRP_NERVOS = "ckb";
static const char *_Nonnull HRP_ZILLIQA = "zil";
static const char *_Nonnull HRP_TERRA = "terra";
static const char *_Nonnull HRP_CRYPTOORG = "cro";
static const char *_Nonnull HRP_KAVA = "kava";
static const char *_Nonnull HRP_OASIS = "oasis";
static const char *_Nonnull HRP_BLUZELLE = "bluzelle";
static const char *_Nonnull HRP_BAND = "band";
static const char *_Nonnull HRP_ELROND = "erd";
static const char *_Nonnull HRP_SECRET = "secret";
static const char *_Nonnull HRP_AGORIC = "agoric";
static const char *_Nonnull HRP_BINANCE = "bnb";
static const char *_Nonnull HRP_ECASH = "ecash";
static const char *_Nonnull HRP_THORCHAIN = "thor";
static const char *_Nonnull HRP_BITCOINDIAMOND = "bcd";
static const char *_Nonnull HRP_HARMONY = "one";
static const char *_Nonnull HRP_CARDANO = "addr";
static const char *_Nonnull HRP_QTUM = "qc";
static const char *_Nonnull HRP_PACTUS = "pc";
static const char *_Nonnull HRP_STRATIS = "strax";
static const char *_Nonnull HRP_NATIVEINJECTIVE = "inj";
static const char *_Nonnull HRP_OSMOSIS = "osmo";
static const char *_Nonnull HRP_TERRAV2 = "terra";
static const char *_Nonnull HRP_COREUM = "core";
static const char *_Nonnull HRP_ZETACHAIN = "zeta";
static const char *_Nonnull HRP_NATIVECANTO = "canto";
static const char *_Nonnull HRP_SOMMELIER = "somm";
static const char *_Nonnull HRP_FETCHAI = "fetch";
static const char *_Nonnull HRP_MARS = "mars";
static const char *_Nonnull HRP_UMEE = "umee";
static const char *_Nonnull HRP_QUASAR = "quasar";
static const char *_Nonnull HRP_PERSISTENCE = "persistence";
static const char *_Nonnull HRP_AKASH = "akash";
static const char *_Nonnull HRP_NOBLE = "noble";
static const char *_Nonnull HRP_SEI = "sei";
static const char *_Nonnull HRP_STARGAZE = "stars";
static const char *_Nonnull HRP_NATIVEEVMOS = "evmos";
static const char *_Nonnull HRP_TIA = "celestia";
static const char *_Nonnull HRP_DYDX = "dydx";
static const char *_Nonnull HRP_JUNO = "juno";
static const char *_Nonnull HRP_TBINANCE = "tbnb";
static const char *_Nonnull HRP_STRIDE = "stride";
static const char *_Nonnull HRP_AXELAR = "axelar";
static const char *_Nonnull HRP_CRESCENT = "cre";
static const char *_Nonnull HRP_KUJIRA = "kujira";
static const char *_Nonnull HRP_COMDEX = "comdex";
static const char *_Nonnull HRP_NEUTRON = "neutron";

const char *_Nullable stringForHRP(enum TWHRP hrp);
enum TWHRP hrpForString(const char *_Nonnull string);

TW_EXTERN_C_END
