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

static const auto gSealedPassword = TW::data(std::string("password"));
static const char* gSealedMnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";

static Ethereum::Proto::SigningInput sealedEthTransferNoKey() {
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
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);

    // Baseline (today): app extracts the key, sets it in the proto, then signs.
    PrivateKey pk = key.privateKey(TWCoinTypeEthereum, gSealedPassword);
    auto baseInput = sealedEthTransferNoKey();
    baseInput.set_private_key(pk.bytes.data(), pk.bytes.size());
    Data baseline;
    anyCoinSign(TWCoinTypeEthereum, TW::data(baseInput.SerializeAsString()), baseline);

    // Sealed: key derived, used, and wiped inside the library.
    Data sealed = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));

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
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    Data out = sealedSign(key, TWCoinTypeBitcoin, gSealedPassword, TW::data(std::string("anything")));
    EXPECT_TRUE(out.empty());
}

// No account for the coin: fail closed, and never add one as a side effect of signing.
TEST(SealedSigner, NoAccountReturnsEmptyWithoutMutating) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    Data out = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 0ul);
}

// Only a custom-path account for the coin: the default key would not match its address -> reject.
TEST(SealedSigner, CustomPathAccountRejected) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    key.addAccount("0x0000000000000000000000000000000000000001", TWCoinTypeEthereum, TWDerivationDefault,
                   DerivationPath("m/44'/60'/1'/0/0"), "", "");
    Data out = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// Default account plus another account for the same coin: ambiguous which one the caller means -> reject.
TEST(SealedSigner, MultipleAccountsRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    key.addAccount("0x0000000000000000000000000000000000000001", TWCoinTypeEthereum, TWDerivationDefault,
                   DerivationPath("m/44'/60'/1'/0/0"), "", "");
    Data out = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 2ul);
}

// The caller must not supply a key: an input that already carries one is rejected.
TEST(SealedSigner, InputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    auto input = sealedEthTransferNoKey();
    const auto otherKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(otherKey.data(), otherKey.size());
    Data out = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(input.SerializeAsString()));
    EXPECT_TRUE(out.empty());
}

// Bytes that are not a valid SigningInput are rejected before the keystore is decrypted.
TEST(SealedSigner, MalformedInputRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    // Wrong password would throw if decryption were reached; malformed input must return first.
    Data out = sealedSign(key, TWCoinTypeEthereum, TW::data(std::string("wrong")), parse_hex("ffffffff"));
    EXPECT_TRUE(out.empty());
}

// Private-key keystore: signs byte-identically for its own coin.
TEST(SealedSigner, PrivateKeyKeystoreByteIdentical) {
    const auto pkData = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeEthereum, pkData);

    auto baseInput = sealedEthTransferNoKey();
    baseInput.set_private_key(pkData.data(), pkData.size());
    Data baseline;
    anyCoinSign(TWCoinTypeEthereum, TW::data(baseInput.SerializeAsString()), baseline);

    Data sealed = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));
    EXPECT_FALSE(sealed.empty());
    EXPECT_EQ(sealed, baseline);
}

// Private-key keystore imported for another coin has no Ethereum account -> reject.
TEST(SealedSigner, PrivateKeyKeystoreOtherCoinRejected) {
    const auto pkData = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeBitcoin, pkData);
    Data out = sealedSign(key, TWCoinTypeEthereum, gSealedPassword, TW::data(sealedEthTransferNoKey().SerializeAsString()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// Wrong password: fail closed (decryption throws; the C ABI maps it to null).
TEST(SealedSigner, WrongPasswordThrows) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    EXPECT_ANY_THROW(sealedSign(key, TWCoinTypeEthereum, TW::data(std::string("wrong")), TW::data(sealedEthTransferNoKey().SerializeAsString())));
}

} // namespace TW::Keystore::tests
