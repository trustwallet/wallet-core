// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(TWHRP, StringForHRP) {
    ASSERT_STREQ(stringForHRP(TWHRPUnknown), nullptr);
    ASSERT_STREQ(stringForHRP(TWHRPBitcoin), "bc");
    ASSERT_STREQ(stringForHRP(TWHRPLitecoin), "ltc");
    ASSERT_STREQ(stringForHRP(TWHRPBitcoinCash), "bitcoincash");
    ASSERT_STREQ(stringForHRP(TWHRPBinance), "bnb");
    ASSERT_STREQ(stringForHRP(TWHRPCosmos), "cosmos");
    ASSERT_STREQ(stringForHRP(TWHRPDigiByte), "dgb");
    ASSERT_STREQ(stringForHRP(TWHRPGroestlcoin), "grs");
    ASSERT_STREQ(stringForHRP(TWHRPIoTeX), "io");
    ASSERT_STREQ(stringForHRP(TWHRPQtum), "qc");
    ASSERT_STREQ(stringForHRP(TWHRPZilliqa), "zil");
    ASSERT_STREQ(stringForHRP(TWHRPViacoin), "via");
    ASSERT_STREQ(stringForHRP(TWHRPTerra), "terra");
    ASSERT_STREQ(stringForHRP(TWHRPMonacoin), "mona");
    ASSERT_STREQ(stringForHRP(TWHRPKava), "kava");
    ASSERT_STREQ(stringForHRP(TWHRPBandChain), "band");
    ASSERT_STREQ(stringForHRP(TWHRPBluzelle), "bluzelle");
    ASSERT_STREQ(stringForHRP(TWHRPCardano), "addr");
    ASSERT_STREQ(stringForHRP(TWHRPMultiversX), "erd");
    ASSERT_STREQ(stringForHRP(TWHRPOasis), "oasis");
    ASSERT_STREQ(stringForHRP(TWHRPTHORChain), "thor");
    ASSERT_STREQ(stringForHRP(TWHRPCryptoOrg), "cro");
    ASSERT_STREQ(stringForHRP(TWHRPOsmosis), "osmo");
    ASSERT_STREQ(stringForHRP(TWHRPSecret), "secret");
}

TEST(TWHRP, HRPForString) {
    ASSERT_EQ(hrpForString(""), TWHRPUnknown);
    ASSERT_EQ(hrpForString("bc"), TWHRPBitcoin);
    ASSERT_EQ(hrpForString("ltc"), TWHRPLitecoin);
    ASSERT_EQ(hrpForString("bitcoincash"), TWHRPBitcoinCash);
    ASSERT_EQ(hrpForString("bnb"), TWHRPBinance);
    ASSERT_EQ(hrpForString("cosmos"), TWHRPCosmos);
    ASSERT_EQ(hrpForString("dgb"), TWHRPDigiByte);
    ASSERT_EQ(hrpForString("grs"), TWHRPGroestlcoin);
    ASSERT_EQ(hrpForString("io"), TWHRPIoTeX);
    ASSERT_EQ(hrpForString("qc"), TWHRPQtum);
    ASSERT_EQ(hrpForString("zil"), TWHRPZilliqa);
    ASSERT_EQ(hrpForString("via"), TWHRPViacoin);
    ASSERT_EQ(hrpForString("terra"), TWHRPTerra);
    ASSERT_EQ(hrpForString("mona"), TWHRPMonacoin);
    ASSERT_EQ(hrpForString("kava"), TWHRPKava);
    ASSERT_EQ(hrpForString("band"), TWHRPBandChain);
    ASSERT_EQ(hrpForString("addr"), TWHRPCardano);
    ASSERT_EQ(hrpForString("erd"), TWHRPMultiversX);
    ASSERT_EQ(hrpForString("oasis"), TWHRPOasis);
    ASSERT_EQ(hrpForString("thor"), TWHRPTHORChain);
    ASSERT_EQ(hrpForString("bluzelle"), TWHRPBluzelle);
    ASSERT_EQ(hrpForString("cro"), TWHRPCryptoOrg);
    ASSERT_EQ(hrpForString("osmo"), TWHRPOsmosis);
    ASSERT_EQ(hrpForString("ecash"), TWHRPECash);
    ASSERT_EQ(hrpForString("secret"), TWHRPSecret);
}

TEST(TWHPR, HPRByCoinType) {
    ASSERT_EQ(TWHRPBitcoin, TWCoinTypeHRP(TWCoinTypeBitcoin));
    ASSERT_EQ(TWHRPBitcoinCash, TWCoinTypeHRP(TWCoinTypeBitcoinCash));
    ASSERT_EQ(TWHRPBinance, TWCoinTypeHRP(TWCoinTypeBinance));
    ASSERT_EQ(TWHRPCosmos, TWCoinTypeHRP(TWCoinTypeCosmos));
    ASSERT_EQ(TWHRPDigiByte, TWCoinTypeHRP(TWCoinTypeDigiByte));
    ASSERT_EQ(TWHRPLitecoin, TWCoinTypeHRP(TWCoinTypeLitecoin));
    ASSERT_EQ(TWHRPGroestlcoin, TWCoinTypeHRP(TWCoinTypeGroestlcoin));
    ASSERT_EQ(TWHRPIoTeX, TWCoinTypeHRP(TWCoinTypeIoTeX));
    ASSERT_EQ(TWHRPViacoin, TWCoinTypeHRP(TWCoinTypeViacoin));
    ASSERT_EQ(TWHRPQtum, TWCoinTypeHRP(TWCoinTypeQtum));
    ASSERT_EQ(TWHRPZilliqa, TWCoinTypeHRP(TWCoinTypeZilliqa));
    ASSERT_EQ(TWHRPTerra, TWCoinTypeHRP(TWCoinTypeTerra));
    ASSERT_EQ(TWHRPMonacoin, TWCoinTypeHRP(TWCoinTypeMonacoin));
    ASSERT_EQ(TWHRPKava, TWCoinTypeHRP(TWCoinTypeKava));
    ASSERT_EQ(TWHRPBandChain, TWCoinTypeHRP(TWCoinTypeBandChain));
    ASSERT_EQ(TWHRPBluzelle, TWCoinTypeHRP(TWCoinTypeBluzelle));
    ASSERT_EQ(TWHRPCardano, TWCoinTypeHRP(TWCoinTypeCardano));
    ASSERT_EQ(TWHRPMultiversX, TWCoinTypeHRP(TWCoinTypeMultiversX));
    ASSERT_EQ(TWHRPOasis, TWCoinTypeHRP(TWCoinTypeOasis));
    ASSERT_EQ(TWHRPTHORChain, TWCoinTypeHRP(TWCoinTypeTHORChain));
    ASSERT_EQ(TWHRPCryptoOrg, TWCoinTypeHRP(TWCoinTypeCryptoOrg));
    ASSERT_EQ(TWHRPOsmosis, TWCoinTypeHRP(TWCoinTypeOsmosis));
    ASSERT_EQ(TWHRPECash, TWCoinTypeHRP(TWCoinTypeECash));
    ASSERT_EQ(TWHRPSecret, TWCoinTypeHRP(TWCoinTypeSecret));

    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeAion));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeCallisto));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeDash));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeDecred));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeDogecoin));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeEOS));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeEthereum));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeEthereumClassic));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeGoChain));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeICON));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeKin));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeNULS));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeNano));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeNimiq));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeOntology));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypePOANetwork));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeXRP));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeStellar));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeTezos));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeTheta));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeThunderCore));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeTomoChain));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeTron));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeVeChain));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeWanchain));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeZcash));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeFiro));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeZelcash));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeRavencoin));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeWaves));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeNEO));
    ASSERT_EQ(TWHRPUnknown, TWCoinTypeHRP(TWCoinTypeNebl));
}
