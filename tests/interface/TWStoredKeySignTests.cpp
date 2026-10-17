// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// S2.1 — C ABI sealed-signing test (Security Layer, Phase 2).
// Drives TWStoredKeySign across the extern "C" boundary and proves it is byte-identical to the
// current extract-key-then-sign flow (TWStoredKeyPrivateKey + TWAnySignerSign) for Ethereum.

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"
#include "proto/Common.pb.h"
#include "proto/Ethereum.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWStoredKey.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

static Data ethTransferNoKey() {
    Ethereum::Proto::SigningInput in;
    auto chainId = store(uint256_t(1));
    in.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(0));
    in.set_nonce(nonce.data(), nonce.size());
    auto gasPrice = store(uint256_t(20000000000ULL));
    in.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(21000));
    in.set_gas_limit(gasLimit.data(), gasLimit.size());
    in.set_to_address("0x3535353535353535353535353535353535353535");
    auto& transfer = *in.mutable_transaction()->mutable_transfer();
    auto amount = store(uint256_t(1000000000000000000ULL));
    transfer.set_amount(amount.data(), amount.size());
    return data(in.SerializeAsString());
}

TEST(TWStoredKeySign, EthereumEndToEndByteIdentical) {
    const auto mnemonic = STRING("team engine square letter hero song dizzy scrub tornado fabric divert saddle");
    const auto name = STRING("name");
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>("password"), 8));
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), TWCoinTypeEthereum));
    ASSERT_NE(key.get(), nullptr);

    const auto inputBytes = ethTransferNoKey();
    const auto input = WRAPD(TWDataCreateWithBytes(inputBytes.data(), inputBytes.size()));

    // Sealed path over the C ABI.
    const auto sealed = WRAPD(TWStoredKeySign(key.get(), TWCoinTypeEthereum, password.get(), input.get()));
    ASSERT_NE(sealed.get(), nullptr);
    const auto sealedData = data(TWDataBytes(sealed.get()), TWDataSize(sealed.get()));

    // Baseline: extract the key (C ABI) and sign via TWAnySigner.
    const auto pk = WRAP(TWPrivateKey, TWStoredKeyPrivateKey(key.get(), TWCoinTypeEthereum, password.get()));
    const auto pkData = WRAPD(TWPrivateKeyData(pk.get()));
    Ethereum::Proto::SigningInput baseInput;
    baseInput.ParseFromArray(inputBytes.data(), static_cast<int>(inputBytes.size()));
    baseInput.set_private_key(TWDataBytes(pkData.get()), TWDataSize(pkData.get()));
    const auto baseInBytes = data(baseInput.SerializeAsString());
    const auto baseIn = WRAPD(TWDataCreateWithBytes(baseInBytes.data(), baseInBytes.size()));
    const auto baseline = WRAPD(TWAnySignerSign(baseIn.get(), TWCoinTypeEthereum));
    const auto baselineData = data(TWDataBytes(baseline.get()), TWDataSize(baseline.get()));

    EXPECT_EQ(hex(sealedData), hex(baselineData)); // byte-identical through the C boundary

    Ethereum::Proto::SigningOutput out;
    ASSERT_TRUE(out.ParseFromArray(sealedData.data(), static_cast<int>(sealedData.size())));
    EXPECT_EQ(out.error(), Common::Proto::OK);
    EXPECT_FALSE(out.encoded().empty());
}

TEST(TWStoredKeySign, UnsupportedCoinReturnsNull) {
    const auto mnemonic = STRING("team engine square letter hero song dizzy scrub tornado fabric divert saddle");
    const auto name = STRING("name");
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>("password"), 8));
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), TWCoinTypeEthereum));

    const auto input = DATA("00");
    // Bitcoin is not yet wired into the sealed path -> null (caller falls back).
    TWData* out = TWStoredKeySign(key.get(), TWCoinTypeBitcoin, password.get(), input.get());
    EXPECT_EQ(out, nullptr);
    if (out != nullptr) {
        TWDataDelete(out);
    }
}
