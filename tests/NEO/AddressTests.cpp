// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NEO/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::NEO;

 TEST(NEOAddress, FromPublicKey) {
     const auto publicKey = PublicKey(parse_hex("0222b2277d039d67f4197a638dd5a1d99c290b17aa8c4a16ccee5165fe612de66a"), TWPublicKeyTypeSECP256k1);
     const auto address = Address(publicKey);
     auto str = hex(address.bytes);
     ASSERT_EQ(string("AKmrAHRD9ZDUnu4m3vWWonpsojo4vgSuqp"), address.string());
 }

 TEST(NEOAddress, FromString) {
     string neoAddress = "AXkgwcMJTy9wTAXHsbyhauxh7t2Tt31MmC";
     const auto address = Address(neoAddress);
     ASSERT_EQ(address.string(), neoAddress);
 }

TEST(NEOAddress, isValid) {
    string neoAddress = "AQAsqiyHS4SSVWZ4CmMmnCxWg7vJ84GEj4";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(neoAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
}
