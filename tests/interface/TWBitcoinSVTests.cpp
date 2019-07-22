// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinCashAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinTransactionSigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(BitcoinSV, Address) {
    EXPECT_TRUE(TWBitcoinCashAddressIsValidString(STRING("bitcoincash:qr545rg9kultrky4xenz8fkzyllt3988hsn6rq0ayx").get()));
    EXPECT_TRUE(TWBitcoinAddressIsValidString(STRING("1NGrKYMJg9NmbewXmLvQVsxbBWjuxVRV6v").get()));
}

TEST(BitcoinSV, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("").get()
    ));

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoinSV, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoinSV, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9zRGTX4fcSHqceZnQrEiaFrJyjkBj1uz782hCJiLQCZASMZwYD2vShME1BAssjRgpG4xFtHqypSTNVLjyZ6Gds3VhPX97c3mFxu78g6UX5s");
    assertStringsEqual(xpub, "xpub6DQcs2bZSor8q8eFWsmiwPo3Xmag8UdqULxHzh7wxY69K9u65kMAzVfhrQQDUPL1Koe5yAoNeLDaNtXkUFmRsg7Aven1ExUhHDiydPidz6M");
}

TEST(BitcoinSV, DeriveFromXPub) {
    // m/44'/236' xpubKey
    auto xpub = STRING("xpub6DQcs2bZSor8q8eFWsmiwPo3Xmag8UdqULxHzh7wxY69K9u65kMAzVfhrQQDUPL1Koe5yAoNeLDaNtXkUFmRsg7Aven1ExUhHDiydPidz6M");
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/236'/0'/0/2").get());
    auto pubKey9 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/236'/0'/0/9").get());

    auto addr2 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, pubKey2));
    auto addr9 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, pubKey9));

    assertStringsEqual(addr2, "1AzTmTvoFwKN3a4U9wioPJ6zhwXgQtkXDa");
    assertStringsEqual(addr9, "1Nx3HC3G2QkiTHfTKkka5u8RJ2YB9cxvma");
}

TEST(BitcoinSV, SignTransaction) {
    const int64_t amount = 600;
    const int64_t fee = 226;

    // Transaction on Bitcoin Cash Mainnet
    // https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeFork | TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU");

    auto hash0 = DATA("e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(2);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(5151);
    auto script0 = parse_hex("76a914aff1e0789e5fe316b729577665aa0a04d5b0f8c788ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    // Sign
    auto signer = TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction>(std::move(input));
    auto result = signer.sign();
    auto signedTx = result.payload();

    ASSERT_TRUE(result);
    ASSERT_EQ(fee, signer.plan.fee);

    // txid = "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4"

    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(hex(serialized),
        "01000000"
        "01"
            "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05" "02000000" "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b" "ffffffff"
        "02"
            "5802000000000000" "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac"
            "e510000000000000" "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac"
        "00000000");
}

