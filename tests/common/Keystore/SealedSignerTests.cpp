// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// S2.1 — real sealed-signing implementation tests (Security Layer, Phase 2).
// Proves the sealed path (key never leaves the library) is byte-identical to the current
// extract-then-sign flow, and that it fails closed whenever it cannot prove which key to use.

#include "Coin.h"
#include "Data.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "Keystore/SealedSigner.h"
#include "Keystore/StoredKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/BitcoinV2.pb.h"
#include "proto/Common.pb.h"
#include "proto/Decred.pb.h"
#include "proto/Ethereum.pb.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWDerivation.h>

#include <gtest/gtest.h>

#include <utility>
#include <vector>

namespace TW::Keystore::tests {

static const auto gSealedPassword = TW::data(std::string("password"));
static const char* gSealedMnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
static const auto gSealedRawKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");

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

static Data sealedSerialize(const google::protobuf::MessageLite& message) {
    return TW::data(message.SerializeAsString());
}

static Account sealedOnlyAccount(const StoredKey& key, TWCoinType coin) {
    const auto accounts = key.getAccounts(coin);
    EXPECT_EQ(accounts.size(), 1ul);
    return accounts.front();
}

// Baseline (today's flow): the app extracts the key, sets it in the proto, then signs.
static Data sealedEthBaseline(const Data& keyBytes) {
    auto in = sealedEthTransferNoKey();
    in.set_private_key(keyBytes.data(), keyBytes.size());
    Data out;
    anyCoinSign(TWCoinTypeEthereum, sealedSerialize(in), out);
    return out;
}

// ---------------------------------------------------------------------------------------------
// Ethereum
// ---------------------------------------------------------------------------------------------

TEST(SealedSigner, EthereumByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeEthereum, account.derivationPath);
    const auto baseline = sealedEthBaseline(pk.bytes);

    const auto sealed = sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));

    Ethereum::Proto::SigningOutput sealedOut;
    ASSERT_TRUE(sealedOut.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
    EXPECT_EQ(sealedOut.error(), Common::Proto::OK);
    EXPECT_FALSE(sealedOut.encoded().empty());
    EXPECT_EQ(sealed, baseline);
}

// A non-default account (custom path) signs with its own key, selected by address.
TEST(SealedSigner, EthereumCustomPathAccountByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto defaultAccount = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto customPath = DerivationPath("m/44'/60'/1'/0/0");
    const auto customKey = key.wallet(gSealedPassword).getKey(TWCoinTypeEthereum, customPath);
    const auto customAddress = TW::deriveAddress(TWCoinTypeEthereum, customKey);
    ASSERT_NE(customAddress, defaultAccount.address);
    key.addAccount(customAddress, TWCoinTypeEthereum, TWDerivationDefault, customPath, "", "");

    const auto input = sealedSerialize(sealedEthTransferNoKey());
    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereum, customAddress, gSealedPassword, input), sealedEthBaseline(customKey.bytes));
    // The default account in the same keystore still signs with the default key.
    const auto defaultKey = key.wallet(gSealedPassword).getKey(TWCoinTypeEthereum, defaultAccount.derivationPath);
    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereum, defaultAccount.address, gSealedPassword, input), sealedEthBaseline(defaultKey.bytes));
    EXPECT_EQ(key.accounts.size(), 2ul);
}

TEST(SealedSigner, PrivateKeyKeystoreByteIdentical) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeEthereum, gSealedRawKey);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto sealed = sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_FALSE(sealed.empty());
    EXPECT_EQ(sealed, sealedEthBaseline(gSealedRawKey));
}

// ---------------------------------------------------------------------------------------------
// Fail-closed cases
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
// Other EVM chains: key from the chain's own account, signed under Ethereum (as the app does)
// ---------------------------------------------------------------------------------------------

static Ethereum::Proto::SigningInput sealedEvmTransferNoKey(uint64_t chainId, bool eip1559) {
    auto in = sealedEthTransferNoKey();
    auto id = store(uint256_t(chainId));
    in.set_chain_id(id.data(), id.size());
    if (eip1559) {
        in.set_tx_mode(Ethereum::Proto::Enveloped);
        auto tip = store(uint256_t(2000000000ULL));
        in.set_max_inclusion_fee_per_gas(tip.data(), tip.size());
        auto maxFee = store(uint256_t(30000000000ULL));
        in.set_max_fee_per_gas(maxFee.data(), maxFee.size());
    }
    return in;
}

static Data sealedEvmBaseline(Ethereum::Proto::SigningInput in, const Data& keyBytes) {
    in.set_private_key(keyBytes.data(), keyBytes.size());
    Data out;
    anyCoinSign(TWCoinTypeEthereum, sealedSerialize(in), out);
    return out;
}

// Chains on Ethereum's derivation path, both legacy and EIP-1559 transactions.
TEST(SealedSigner, EvmSharedDerivationChainsByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto wallet = key.wallet(gSealedPassword);
    const std::vector<std::pair<TWCoinType, uint64_t>> chains = {
        {TWCoinTypeSmartChain, 56}, {TWCoinTypePolygon, 137}, {TWCoinTypeArbitrum, 42161}};
    for (const auto& [coin, chainId] : chains) {
        const auto account = key.account(coin, &wallet);
        ASSERT_TRUE(account.has_value());
        const auto pk = wallet.getKey(coin, account->derivationPath);
        for (const bool eip1559 : {false, true}) {
            const auto input = sealedEvmTransferNoKey(chainId, eip1559);
            const auto sealed = sealedSign(key, coin, account->address, gSealedPassword, sealedSerialize(input));
            ASSERT_FALSE(sealed.empty()) << coin << " eip1559=" << eip1559;
            EXPECT_EQ(sealed, sealedEvmBaseline(input, pk.bytes)) << coin << " eip1559=" << eip1559;
        }
    }
}

// A chain on its own derivation path (Ethereum Classic, m/44'/61') signs with its own key, never
// Ethereum's; and its account cannot be reached through Ethereum's address.
TEST(SealedSigner, EvmOwnDerivationChainByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto wallet = key.wallet(gSealedPassword);
    const auto ethAccount = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto account = key.account(TWCoinTypeEthereumClassic, &wallet);
    ASSERT_TRUE(account.has_value());
    ASSERT_NE(account->address, ethAccount.address);
    const auto pk = wallet.getKey(TWCoinTypeEthereumClassic, account->derivationPath);
    const auto ethPk = wallet.getKey(TWCoinTypeEthereum, ethAccount.derivationPath);

    const auto input = sealedEvmTransferNoKey(61, false);
    const auto sealed = sealedSign(key, TWCoinTypeEthereumClassic, account->address, gSealedPassword, sealedSerialize(input));
    EXPECT_EQ(sealed, sealedEvmBaseline(input, pk.bytes));
    EXPECT_NE(sealed, sealedEvmBaseline(input, ethPk.bytes));

    EXPECT_TRUE(sealedSign(key, TWCoinTypeEthereumClassic, ethAccount.address, gSealedPassword, sealedSerialize(input)).empty());
}

// A private-key keystore that holds only a Smart Chain account (no Ethereum account) still signs.
TEST(SealedSigner, EvmPrivateKeyKeystoreWithoutEthereumAccount) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeSmartChain, gSealedRawKey);
    ASSERT_TRUE(key.getAccounts(TWCoinTypeEthereum).empty());
    const auto account = sealedOnlyAccount(key, TWCoinTypeSmartChain);
    const auto input = sealedEvmTransferNoKey(56, true);
    const auto sealed = sealedSign(key, TWCoinTypeSmartChain, account.address, gSealedPassword, sealedSerialize(input));
    ASSERT_FALSE(sealed.empty());
    EXPECT_EQ(sealed, sealedEvmBaseline(input, gSealedRawKey));
}

TEST(SealedSigner, UnsupportedCoinReturnsEmpty) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    EXPECT_TRUE(sealedSign(key, TWCoinTypeSolana, account.address, gSealedPassword, TW::data(std::string("anything"))).empty());
}

// No account for (coin, address): fail closed, and never add one as a side effect of signing.
TEST(SealedSigner, UnknownAddressReturnsEmptyWithoutMutating) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    const auto defaultAddress = key.wallet(gSealedPassword).deriveAddress(TWCoinTypeEthereum);
    const auto out = sealedSign(key, TWCoinTypeEthereum, defaultAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 0ul);
}

// An account record whose path does not reproduce its address is not trusted.
TEST(SealedSigner, AccountPathNotMatchingAddressRejected) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    const auto otherAddress = "0x0000000000000000000000000000000000000001";
    key.addAccount(otherAddress, TWCoinTypeEthereum, TWDerivationDefault, DerivationPath("m/44'/60'/0'/0/0"), "", "");
    const auto out = sealedSign(key, TWCoinTypeEthereum, otherAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_TRUE(out.empty());
}

// The same address recorded under two different paths is ambiguous. addAccount() deduplicates by
// address, but a keystore loaded from JSON can still carry such records.
TEST(SealedSigner, AmbiguousAccountRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    key.accounts.emplace_back(account.address, TWCoinTypeEthereum, TWDerivationDefault, DerivationPath("m/44'/60'/1'/0/0"), "", "");
    const auto out = sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_TRUE(out.empty());
}

// Private-key keystore imported for another coin has no Ethereum account.
TEST(SealedSigner, PrivateKeyKeystoreOtherCoinRejected) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeBitcoin, gSealedRawKey);
    const auto ethAddress = TW::deriveAddress(TWCoinTypeEthereum, PrivateKey(gSealedRawKey));
    const auto out = sealedSign(key, TWCoinTypeEthereum, ethAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// The caller must not supply a key: an input that already carries one is rejected.
TEST(SealedSigner, InputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    auto input = sealedEthTransferNoKey();
    input.set_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_TRUE(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(input)).empty());
}

// Bytes that are not a valid SigningInput are rejected before the keystore is decrypted.
TEST(SealedSigner, MalformedInputRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    // A wrong password would throw if decryption were reached; malformed input must return first.
    EXPECT_TRUE(sealedSign(key, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), parse_hex("ffffffff")).empty());
}

TEST(SealedSigner, WrongPasswordThrows) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    EXPECT_ANY_THROW(sealedSign(key, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), sealedSerialize(sealedEthTransferNoKey())));
}

// ---------------------------------------------------------------------------------------------
// UTXO (Bitcoin family) — inputs mirror the app's BitcoinV2 transfer: every UTXO and the change
// output use the account's single address; the V1 envelope carries `coin_type` and `signing_v2`.
// ---------------------------------------------------------------------------------------------

struct SealedChainInfo {
    uint32_t p2pkh;
    uint32_t p2sh;
    const char* hrp;
    uint32_t sighash = 1; // SIGHASH_ALL; Bitcoin Cash uses ALL | FORKID (0x41)
};

static Bitcoin::Proto::SigningInput sealedUtxoTransferNoKey(TWCoinType coin, const Account& account, const SealedChainInfo& chain, const std::string& toAddress) {
    BitcoinV2::Proto::SigningInput v2;
    const auto publicKey = parse_hex(account.publicKey);
    v2.add_public_keys(publicKey.data(), publicKey.size());
    v2.mutable_chain_info()->set_p2pkh_prefix(chain.p2pkh);
    v2.mutable_chain_info()->set_p2sh_prefix(chain.p2sh);
    v2.mutable_chain_info()->set_hrp(chain.hrp);
    // Taproot (Schnorr) signatures use random aux data; fix it so baseline and sealed are comparable.
    v2.set_dangerous_use_fixed_schnorr_rng(true);

    auto& builder = *v2.mutable_builder();
    builder.set_version(coin == TWCoinTypeBitcoin ? BitcoinV2::Proto::TransactionVersion::V2 : BitcoinV2::Proto::TransactionVersion::UseDefault);
    builder.set_input_selector(BitcoinV2::Proto::InputSelector::SelectDescending);
    builder.set_fee_per_vb(10);
    builder.set_fixed_dust_threshold(546);
    if (coin == TWCoinTypeZcash || coin == TWCoinTypeZelcash) {
        const auto nu6BranchId = parse_hex("5510e7c8");
        builder.mutable_zcash_extra_data()->set_branch_id(nu6BranchId.data(), nu6BranchId.size());
    }

    // Two UTXOs at the account's address — a multi-input transaction signed by the one account key.
    const char* txIds[] = {
        "d15d38de9a619809b575532a235d23947c4ff7d219d3feb6c5b6105d23360f4e",
        "089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e",
    };
    const int64_t values[] = {60000, 50000};
    for (size_t i = 0; i < 2; ++i) {
        auto& in = *builder.add_inputs();
        auto txId = parse_hex(txIds[i]);
        in.mutable_out_point()->set_hash(txId.data(), txId.size());
        in.mutable_out_point()->set_vout(static_cast<uint32_t>(i));
        in.set_value(values[i]);
        in.set_sighash_type(chain.sighash);
        in.mutable_sequence()->set_sequence(0xffffffff);
        in.set_receiver_address(account.address);
    }

    auto& out = *builder.add_outputs();
    out.set_value(90000);
    out.set_to_address(toAddress);
    builder.mutable_change_output()->set_to_address(account.address);

    Bitcoin::Proto::SigningInput legacy;
    legacy.set_coin_type(static_cast<uint32_t>(coin));
    *legacy.mutable_signing_v2() = v2;
    return legacy;
}

// Baseline: what the native bridge does today — inject the extracted key into signing_v2.private_keys.
static Data sealedUtxoBaseline(TWCoinType coin, Bitcoin::Proto::SigningInput input, const Data& keyBytes) {
    input.mutable_signing_v2()->add_private_keys(keyBytes.data(), keyBytes.size());
    Data out;
    anyCoinSign(coin, sealedSerialize(input), out);
    return out;
}

static void expectUtxoByteIdentical(StoredKey& key, TWCoinType coin, const Account& account, const SealedChainInfo& chain, const std::string& toAddress) {
    SCOPED_TRACE(account.address);
    const auto input = sealedUtxoTransferNoKey(coin, account, chain, toAddress);
    const auto pk = key.wallet(gSealedPassword).getKey(coin, account.derivationPath);
    const auto baseline = sealedUtxoBaseline(coin, input, pk.bytes);

    const auto sealed = sealedSign(key, coin, account.address, gSealedPassword, sealedSerialize(input));

    // Decred wraps the V2 result in its own SigningOutput; every other coin uses Bitcoin's.
    BitcoinV2::Proto::SigningOutput result;
    if (coin == TWCoinTypeDecred) {
        Decred::Proto::SigningOutput sealedOut;
        ASSERT_TRUE(sealedOut.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
        EXPECT_EQ(sealedOut.error(), Common::Proto::OK);
        ASSERT_TRUE(sealedOut.has_signing_result_v2());
        result = sealedOut.signing_result_v2();
    } else {
        Bitcoin::Proto::SigningOutput sealedOut;
        ASSERT_TRUE(sealedOut.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
        EXPECT_EQ(sealedOut.error(), Common::Proto::OK);
        ASSERT_TRUE(sealedOut.has_signing_result_v2());
        result = sealedOut.signing_result_v2();
    }
    EXPECT_EQ(result.error(), Common::Proto::OK) << result.error_message();
    EXPECT_FALSE(result.encoded().empty());
    EXPECT_EQ(hex(sealed), hex(baseline));
}

static const SealedChainInfo gBitcoinChain{0, 5, "bc"};

// Bitcoin: SegWit (default), Legacy and Taproot accounts in one keystore, each selected by address.
TEST(SealedSigner, BitcoinAddressTypesByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeBitcoin);
    const auto wallet = key.wallet(gSealedPassword);
    key.account(TWCoinTypeBitcoin, TWDerivationBitcoinLegacy, wallet);
    key.account(TWCoinTypeBitcoin, TWDerivationBitcoinTaproot, wallet);
    const auto accounts = key.getAccounts(TWCoinTypeBitcoin);
    ASSERT_EQ(accounts.size(), 3ul);

    for (const auto& account : accounts) {
        expectUtxoByteIdentical(key, TWCoinTypeBitcoin, account, gBitcoinChain, "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    }
    EXPECT_EQ(key.accounts.size(), 3ul);
}

TEST(SealedSigner, LitecoinByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeLitecoin);
    expectUtxoByteIdentical(key, TWCoinTypeLitecoin, sealedOnlyAccount(key, TWCoinTypeLitecoin), {48, 50, "ltc"},
                            "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu");
}

TEST(SealedSigner, DogecoinByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeDogecoin);
    expectUtxoByteIdentical(key, TWCoinTypeDogecoin, sealedOnlyAccount(key, TWCoinTypeDogecoin), {30, 22, ""},
                            "DLSSSUS3ex7YNDACJDxMER1ZMW579Vy8Zy");
}

// The remaining BitcoinV2 coins the app signs, with the app's chain info (`chainInfoForCoin`).
// Each sends to the account's own address, so no per-coin destination fixture is needed.
TEST(SealedSigner, OtherUtxoCoinsByteIdentical) {
    const std::vector<std::pair<TWCoinType, SealedChainInfo>> coins = {
        {TWCoinTypeBitcoinCash, {0, 5, "bitcoincash", 0x41}},
        {TWCoinTypeDash, {76, 16, ""}},
        {TWCoinTypeViacoin, {71, 33, "via"}},
        {TWCoinTypeDigiByte, {30, 63, "dgb"}},
        {TWCoinTypeRavencoin, {60, 122, ""}},
        {TWCoinTypeFiro, {82, 7, ""}},
        {TWCoinTypeQtum, {58, 50, ""}},
        {TWCoinTypeZcash, {184, 189, ""}},
        {TWCoinTypeZelcash, {184, 189, ""}},
        {TWCoinTypeGroestlcoin, {36, 5, "grs"}},
        {TWCoinTypeDecred, {63, 26, ""}},
    };
    for (const auto& [coin, chain] : coins) {
        SCOPED_TRACE(coin);
        auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, coin);
        const auto account = sealedOnlyAccount(key, coin);
        expectUtxoByteIdentical(key, coin, account, chain, account.address);
    }
}

// coin_type must name the coin being signed — a Bitcoin input signed as Litecoin is rejected
// (the class of bug where one coin's key signs another coin's claiming script).
TEST(SealedSigner, UtxoCoinTypeMismatchRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeLitecoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeLitecoin);
    auto input = sealedUtxoTransferNoKey(TWCoinTypeLitecoin, account, {48, 50, "ltc"}, "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu");
    input.set_coin_type(TWCoinTypeBitcoin);
    EXPECT_TRUE(sealedSign(key, TWCoinTypeLitecoin, account.address, gSealedPassword, sealedSerialize(input)).empty());
}

// Only the BitcoinV2 envelope is accepted: a legacy V1-only input is rejected.
TEST(SealedSigner, UtxoWithoutSigningV2Rejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeBitcoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeBitcoin);
    Bitcoin::Proto::SigningInput input;
    input.set_amount(1000);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address(account.address);
    EXPECT_TRUE(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(input)).empty());
}

// A key supplied in either the V1 `private_key` or `signing_v2.private_keys` is rejected.
TEST(SealedSigner, UtxoInputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeBitcoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeBitcoin);
    const auto clean = sealedUtxoTransferNoKey(TWCoinTypeBitcoin, account, gBitcoinChain, "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");

    auto withV1Key = clean;
    withV1Key.add_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_TRUE(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(withV1Key)).empty());

    auto withV2Key = clean;
    withV2Key.mutable_signing_v2()->add_private_keys(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_TRUE(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(withV2Key)).empty());
}

} // namespace TW::Keystore::tests
