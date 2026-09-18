// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// S2.1 — real sealed-signing implementation tests (Security Layer, Phase 2).
// Proves the sealed path (key never leaves the library) is byte-identical to the current
// extract-then-sign flow.

#include "Coin.h"
#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "Keystore/SealedSigner.h"
#include "Keystore/StoredKey.h"
#include "proto/Common.pb.h"
#include "proto/Ethereum.pb.h"

#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

namespace TW::Keystore::tests {

static const auto gPassword = TW::data(std::string("password"));
static const char* gMnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";

static Ethereum::Proto::SigningInput ethTransferNoKey() {
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
    return in;
}

// Sealed sign produces byte-identical output to extract-then-sign for Ethereum.
TEST(SealedSigner, EthereumByteIdentical) {
    auto key = StoredKey::createWithMnemonic("n", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);

    // Baseline (today): app extracts the key, sets it in the proto, then signs.
    PrivateKey pk = key.privateKey(TWCoinTypeEthereum, gPassword);
    auto baseInput = ethTransferNoKey();
    baseInput.set_private_key(pk.bytes.data(), pk.bytes.size());
    Data baseline;
    anyCoinSign(TWCoinTypeEthereum, TW::data(baseInput.SerializeAsString()), baseline);

    // Sealed: key derived, used, and wiped inside the library.
    Data sealed = sealedSign(key, TWCoinTypeEthereum, gPassword, TW::data(ethTransferNoKey().SerializeAsString()));

    Ethereum::Proto::SigningOutput baseOut, sealedOut;
    ASSERT_TRUE(baseOut.ParseFromArray(baseline.data(), static_cast<int>(baseline.size())));
    ASSERT_TRUE(sealedOut.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
    EXPECT_EQ(baseOut.error(), Common::Proto::OK);
    EXPECT_FALSE(baseOut.encoded().empty());
    EXPECT_EQ(hex(sealedOut.encoded()), hex(baseOut.encoded()));
    EXPECT_EQ(sealed, baseline);
}

// Coins not yet wired into the sealed path return empty (caller falls back).
TEST(SealedSigner, UnsupportedCoinReturnsEmpty) {
    auto key = StoredKey::createWithMnemonic("n", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    Data out = sealedSign(key, TWCoinTypeBitcoin, gPassword, TW::data(std::string("anything")));
    EXPECT_TRUE(out.empty());
}

} // namespace TW::Keystore::tests
