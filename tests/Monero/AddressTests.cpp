// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Monero/Address.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Monero;


TEST(MoneroAddress, FromString){
    Address address = Address("439EMUy9kEZF2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wEhjc8eG");
    ASSERT_EQ(address.string(), "439EMUy9kEZF2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wEhjc8eG");
}

TEST(MoneroAddress, FromBadPubSpendKey){
    // TODO
}

TEST(MoneroAddress, FromBadPubViewKey){
    // TODO
}

TEST(MoneroAddress, FromBadString){
    std::string badString = "439EMUy9kEZF2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wEhjc8e3453453G";

    // TODO
//    ASSERT_THROW(Address(badString);, std::invalid_argument);
}

TEST(MoneroAddress, StealthAddressIsValid){
    // TODO
//    ASSERT_TRUE(Address::isValid("6cabaac48d3b9043525a703e9e5feb72132f69ea6deca9b4acf9228beb74cd8f"));
}

TEST(MoneroAddress, IntegratedAddressIsValid){
    // TODO
//    ASSERT_TRUE(Address::isValid("4CquNHneMW5F2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wMLEoAsMWnGpMN6RfDF"));
}

TEST(TWMoneroAddress, PrivateViewKeyFromPrivateSpendKey) {
    PrivateKey privateSpendKey = PrivateKey(
            parse_hex("d17f7ee37fc904cd04692a0db2a8aa003008de6865d7b0ed7c1515b9892cca03"));
    PrivateKey privateViewKey = TW::Monero::Address::privateViewKeyFromPrivateSpendKey(privateSpendKey);
    ASSERT_EQ(TW::hex(privateViewKey.bytes), "33c71dd92b22bfb25e8adfa25e2a7efb4565cbda457a2e0bc1aba021484d5f05");
}

TEST(TWMoneroAddress, PublicViewKeyFromPrivateViewKey) {
    PrivateKey privateViewKey = PrivateKey(parse_hex("33c71dd92b22bfb25e8adfa25e2a7efb4565cbda457a2e0bc1aba021484d5f05"));
    PublicKey publicViewKey = TW::Monero::Address::publicKeyFromPrivateKey(privateViewKey);
    ASSERT_EQ(TW::hex(publicViewKey.bytes), "e203501c6ffb515c59b7620e81bc3d7a7de06eecdf07eec059cc4324f0dfd079");
}

TEST(TWMoneroAddress, PublicSpendKeyFromPrivateSpendKey){
    PrivateKey privateSpendKey = PrivateKey(parse_hex("d17f7ee37fc904cd04692a0db2a8aa003008de6865d7b0ed7c1515b9892cca03"));
    PublicKey publicSpendKey = TW::Monero::Address::publicKeyFromPrivateKey(privateSpendKey);
    ASSERT_EQ(TW::hex(publicSpendKey.bytes), "2817dc38531a3e53e07546584c3ea3b2352c4719eb456a64445cf0a1f61687c3");
}

TEST(Address, StandardAddressFromKeys){
    PublicKey publicSpendKey = PublicKey(parse_hex("2817dc38531a3e53e07546584c3ea3b2352c4719eb456a64445cf0a1f61687c3"), TWPublicKeyTypeED25519);
    PublicKey publicViewKey = PublicKey(parse_hex("e203501c6ffb515c59b7620e81bc3d7a7de06eecdf07eec059cc4324f0dfd079"), TWPublicKeyTypeED25519);

    auto address = Address(Address::mainnetPrefix, publicSpendKey, publicViewKey);

    ASSERT_EQ(address.string(), "439EMUy9kEZF2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wEhjc8eG");
}

TEST(TWMoneroAddress, StealthAddress){
    PublicKey publicSpendKey = PublicKey(parse_hex("3bcb82eecc13739b463b386fc1ed991386a046b478bf4864673ca0a229c3cec1"), TWPublicKeyTypeED25519);
    PublicKey publicViewKey = PublicKey(parse_hex("6bb8297dc3b54407ac78ffa4efa4afbe5f1806e5e41aa56ae98c2fe53032bb4b"), TWPublicKeyTypeED25519);
    std::string address = TW::Monero::Address::getStealthAddress(publicViewKey, publicSpendKey);

    // TODO
    ASSERT_EQ(address, "6cabaac48d3b9043525a703e9e5feb72132f69ea6deca9b4acf9228beb74cd8f");

}

TEST(TWMoneroAddress, IntegratedAddress){
    // TODO
//    PublicKey publicSpendKey = PublicKey(parse_hex("2817dc38531a3e53e07546584c3ea3b2352c4719eb456a64445cf0a1f61687c3"), TWPublicKeyTypeED25519);
//    PublicKey publicViewKey = PublicKey(parse_hex("e203501c6ffb515c59b7620e81bc3d7a7de06eecdf07eec059cc4324f0dfd079"), TWPublicKeyTypeED25519);
//    auto paymentID = parse_hex("8e22a4ede8ae31b4");
//
//    auto address = Address(Address::integratedPrefix, publicSpendKey, publicViewKey, paymentID);

//    ASSERT_EQ(address.string(), "4CquNHneMW5F2iBC85AvVGWoqbzf6A6cHHmiXME7KFYzZmJvxnqoqJgGSv5NFUgGFnMVKjXReSDGZZB3XA1jQj2wMLEoAsMWnGpMN6RfDF");
}
