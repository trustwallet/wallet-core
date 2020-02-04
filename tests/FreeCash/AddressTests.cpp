// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32.h"
#include "FreeCash/Address.h"
#include "FreeCash/SegwitAddress.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Coin.h"
#include "Base58Address.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWHDWallet.h>



#include <cstring>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FreeCash;

TEST(FreeCashAddress, Invalid) {
    auto p2pkh = TW::p2pkhPrefix(TWCoinTypeFreeCash);
    auto p2sh = TW::p2shPrefix(TWCoinTypeFreeCash);
    auto hrp = stringForHRP(TW::hrp(TWCoinTypeFreeCash));
    ASSERT_FALSE(SegwitAddress::isValid("abc",hrp) || Address::isValid("abc", {{p2pkh}, {p2sh}}));
    ASSERT_FALSE(SegwitAddress::isValid("1GiW5ptN66qxHh2Qg14QvEq7whtqTDrwxL",hrp)||
    Address::isValid("1GiW5ptN66qxHh2Qg14QvEq7whtqTDrwxL", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(SegwitAddress::isValid("FMYcYdKSwR4curuSXgiZtdMeyMurME2Tc6",hrp)||
    Address::isValid("FMYcYdKSwR4curuSXgiZtdMeyMurME2Tc6", {{p2pkh}, {p2sh}}));
}


TEST(FreeCashAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("0xc775e7b757ede630cd0aa1113bd102661ab38829ca52a6422ab782862f268646"));
    const auto p2pkh = TW::p2pkhPrefix(TWCoinTypeFreeCash);
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const auto address = Address(publicKey,p2pkh);

    ASSERT_EQ(address.string(), "FEMRWWpVHVVShfvVCPbqUkgezXJ2rJeTrJ");
}


