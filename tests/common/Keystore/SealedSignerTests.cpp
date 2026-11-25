// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// S2.1 — real sealed-signing implementation tests (Security Layer, Phase 2).
// Proves the sealed path (key never leaves the library) is byte-identical to the current
// extract-then-sign flow, and that it fails closed whenever it cannot prove which key to use.

#include "Coin.h"
#include "Base58.h"
#include "Data.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "Keystore/EncryptionParameters.h"
#include "Keystore/SealedSigner.h"
#include "Keystore/StoredKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/BitcoinV2.pb.h"
#include "proto/Common.pb.h"
#include "proto/Decred.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Tron.pb.h"
#include "rust/Wrapper.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWDerivation.h>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

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

// The output of a call that must succeed (fails the test, and returns empty, otherwise).
static Data sealedOk(const SealedSignResult& result) {
    EXPECT_EQ(result.error, TWStoredKeySignErrorOk) << result.message;
    EXPECT_STREQ(result.message, "");
    return result.data;
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

    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey())));

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
    EXPECT_EQ(sealedOk(sealedSign(key, TWCoinTypeEthereum, customAddress, gSealedPassword, input)), sealedEthBaseline(customKey.bytes));
    // The default account in the same keystore still signs with the default key.
    const auto defaultKey = key.wallet(gSealedPassword).getKey(TWCoinTypeEthereum, defaultAccount.derivationPath);
    EXPECT_EQ(sealedOk(sealedSign(key, TWCoinTypeEthereum, defaultAccount.address, gSealedPassword, input)), sealedEthBaseline(defaultKey.bytes));
    EXPECT_EQ(key.accounts.size(), 2ul);
}

TEST(SealedSigner, PrivateKeyKeystoreByteIdentical) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeEthereum, gSealedRawKey);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey())));
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
            const auto sealed = sealedOk(sealedSign(key, coin, account->address, gSealedPassword, sealedSerialize(input)));
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
    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeEthereumClassic, account->address, gSealedPassword, sealedSerialize(input)));
    EXPECT_EQ(sealed, sealedEvmBaseline(input, pk.bytes));
    EXPECT_NE(sealed, sealedEvmBaseline(input, ethPk.bytes));

    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereumClassic, ethAccount.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorAccountNotFound);
}

// ERC-4337 UserOperation (v0.7, Biz4337 batch), as the app's `signUserOperation` builds it.
// The full SigningOutput (UserOp JSON in `encoded`, UserOp hash in `pre_hash`) must match.
TEST(SealedSigner, EvmUserOperationV07ByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeEthereum, account.derivationPath);

    auto input = sealedEvmTransferNoKey(11155111, true);
    input.set_tx_mode(Ethereum::Proto::UserOp);
    input.clear_to_address();
    auto& batch = *input.mutable_transaction()->mutable_scw_batch();
    batch.set_wallet_type(Ethereum::Proto::Biz4337);
    auto& call = *batch.add_calls();
    call.set_address("0x3535353535353535353535353535353535353535");
    auto value = store(uint256_t(1000000000000000ULL));
    call.set_amount(value.data(), value.size());
    auto& userOp = *input.mutable_user_operation_v0_7();
    userOp.set_entry_point("0x0000000071727De22E5E9d8BAf0edAc6f37da032");
    userOp.set_sender(account.address);
    auto preVerificationGas = store(uint256_t(400000));
    userOp.set_pre_verification_gas(preVerificationGas.data(), preVerificationGas.size());
    auto verificationGasLimit = store(uint256_t(1000000));
    userOp.set_verification_gas_limit(verificationGasLimit.data(), verificationGasLimit.size());

    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(input)));
    Ethereum::Proto::SigningOutput out;
    ASSERT_TRUE(out.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
    EXPECT_EQ(out.error(), Common::Proto::OK) << out.error_message();
    EXPECT_FALSE(out.pre_hash().empty());
    EXPECT_EQ(sealed, sealedEvmBaseline(input, pk.bytes));
}

// EIP-7702 SetCode transaction whose authorization has no custom signature, as the app's
// non-delegated SetCode path builds it: the injected key signs BOTH the delegation authorization
// and the transaction. Same key, same bytes as today.
TEST(SealedSigner, EvmEip7702SetCodeByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto wallet = key.wallet(gSealedPassword);
    const auto account = key.account(TWCoinTypeSmartChain, &wallet);
    ASSERT_TRUE(account.has_value());
    const auto pk = wallet.getKey(TWCoinTypeSmartChain, account->derivationPath);

    auto input = sealedEvmTransferNoKey(56, true);
    input.set_tx_mode(Ethereum::Proto::SetCode);
    input.set_to_address(account->address);
    input.mutable_eip7702_authorization()->set_address("0x3535353535353535353535353535353535353535");

    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeSmartChain, account->address, gSealedPassword, sealedSerialize(input)));
    Ethereum::Proto::SigningOutput out;
    ASSERT_TRUE(out.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
    EXPECT_EQ(out.error(), Common::Proto::OK) << out.error_message();
    ASSERT_FALSE(out.encoded().empty());
    EXPECT_EQ(static_cast<uint8_t>(out.encoded()[0]), 0x04); // EIP-7702 transaction type
    EXPECT_EQ(sealed, sealedEvmBaseline(input, pk.bytes));
}

// A private-key keystore that holds only a Smart Chain account (no Ethereum account) still signs.
TEST(SealedSigner, EvmPrivateKeyKeystoreWithoutEthereumAccount) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeSmartChain, gSealedRawKey);
    ASSERT_TRUE(key.getAccounts(TWCoinTypeEthereum).empty());
    const auto account = sealedOnlyAccount(key, TWCoinTypeSmartChain);
    const auto input = sealedEvmTransferNoKey(56, true);
    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeSmartChain, account.address, gSealedPassword, sealedSerialize(input)));
    ASSERT_FALSE(sealed.empty());
    EXPECT_EQ(sealed, sealedEvmBaseline(input, gSealedRawKey));
}

TEST(SealedSigner, UnsupportedCoinReturnsEmpty) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeCosmos);
    const auto account = sealedOnlyAccount(key, TWCoinTypeCosmos);
    EXPECT_EQ(sealedSign(key, TWCoinTypeCosmos, account.address, gSealedPassword, TW::data(std::string("anything"))).error, TWStoredKeySignErrorUnsupportedCoin);
}

// No account for (coin, address): fail closed, and never add one as a side effect of signing.
TEST(SealedSigner, UnknownAddressReturnsEmptyWithoutMutating) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    const auto defaultAddress = key.wallet(gSealedPassword).deriveAddress(TWCoinTypeEthereum);
    const auto out = sealedSign(key, TWCoinTypeEthereum, defaultAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_EQ(out.error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_EQ(key.accounts.size(), 0ul);
}

// An account record whose path does not reproduce its address is not trusted.
TEST(SealedSigner, AccountPathNotMatchingAddressRejected) {
    auto key = StoredKey::createWithMnemonic("n", gSealedPassword, gSealedMnemonic, TWStoredKeyEncryptionLevelDefault);
    const auto otherAddress = "0x0000000000000000000000000000000000000001";
    key.addAccount(otherAddress, TWCoinTypeEthereum, TWDerivationDefault, DerivationPath("m/44'/60'/0'/0/0"), "", "");
    const auto out = sealedSign(key, TWCoinTypeEthereum, otherAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_EQ(out.error, TWStoredKeySignErrorAccountMismatch);
}

// The same address recorded under two different paths is ambiguous. addAccount() deduplicates by
// address, but a keystore loaded from JSON can still carry such records.
TEST(SealedSigner, AmbiguousAccountRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    key.accounts.emplace_back(account.address, TWCoinTypeEthereum, TWDerivationDefault, DerivationPath("m/44'/60'/1'/0/0"), "", "");
    const auto out = sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_EQ(out.error, TWStoredKeySignErrorAccountMismatch);
}

// Private-key keystore imported for another coin has no Ethereum account.
TEST(SealedSigner, PrivateKeyKeystoreOtherCoinRejected) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeBitcoin, gSealedRawKey);
    const auto ethAddress = TW::deriveAddress(TWCoinTypeEthereum, PrivateKey(gSealedRawKey));
    const auto out = sealedSign(key, TWCoinTypeEthereum, ethAddress, gSealedPassword, sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_EQ(out.error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// The caller must not supply a key: an input that already carries one is rejected.
TEST(SealedSigner, InputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    auto input = sealedEthTransferNoKey();
    input.set_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorInvalidInput);
}

// Bytes that are not a valid SigningInput are rejected before the keystore is decrypted.
TEST(SealedSigner, MalformedInputRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    // A wrong password would throw if decryption were reached; malformed input must return first.
    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), parse_hex("ffffffff")).error, TWStoredKeySignErrorInvalidInput);
}

TEST(SealedSigner, WrongPasswordReported) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    EXPECT_EQ(sealedSign(key, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), sealedSerialize(sealedEthTransferNoKey())).error, TWStoredKeySignErrorWrongPassword);
}

// The account lookup runs before decryption: an unknown address is reported as such even with a
// wrong password (no KDF run for a call that cannot succeed).
TEST(SealedSigner, AccountNotFoundCheckedBeforePassword) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto out = sealedSign(key, TWCoinTypeEthereum, "0x0000000000000000000000000000000000000001", TW::data(std::string("wrong")),
                                sealedSerialize(sealedEthTransferNoKey()));
    EXPECT_EQ(out.error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_TRUE(out.data.empty());
    EXPECT_STRNE(out.message, "");
}

// The MAC check passes but the plaintext is not a valid mnemonic / private key: decryption failed,
// distinct from a wrong password.
TEST(SealedSigner, CorruptPayloadReportedAsDecryptionFailed) {
    const auto cipher = AESParameters::AESParametersFromEncryption(TWStoredKeyEncryptionAes128Ctr);

    auto mnemonicKey = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(mnemonicKey, TWCoinTypeEthereum);
    mnemonicKey.payload = EncryptedPayload(gSealedPassword, TW::data(std::string("not a valid mnemonic")), cipher, ScryptParameters::minimal());
    const auto input = sealedSerialize(sealedEthTransferNoKey());
    EXPECT_EQ(sealedSign(mnemonicKey, TWCoinTypeEthereum, account.address, gSealedPassword, input).error, TWStoredKeySignErrorDecryptionFailed);
    EXPECT_EQ(sealedSign(mnemonicKey, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), input).error, TWStoredKeySignErrorWrongPassword);

    auto rawKey = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeEthereum, gSealedRawKey);
    const auto rawAccount = sealedOnlyAccount(rawKey, TWCoinTypeEthereum);
    rawKey.payload = EncryptedPayload(gSealedPassword, Data(32, 0), cipher, ScryptParameters::minimal()); // zero key: invalid
    EXPECT_EQ(sealedSign(rawKey, TWCoinTypeEthereum, rawAccount.address, gSealedPassword, input).error, TWStoredKeySignErrorDecryptionFailed);
}

// Every failure carries a fixed, non-empty description; it never echoes the input or the address.
TEST(SealedSigner, ErrorMessagesAreFixedText) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeEthereum);
    const auto account = sealedOnlyAccount(key, TWCoinTypeEthereum);
    auto withKey = sealedEthTransferNoKey();
    withKey.set_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    const std::vector<SealedSignResult> failures = {
        sealedSign(key, TWCoinTypeCosmos, account.address, gSealedPassword, Data()),
        sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, sealedSerialize(withKey)),
        sealedSign(key, TWCoinTypeEthereum, "0x0000000000000000000000000000000000000001", gSealedPassword, sealedSerialize(sealedEthTransferNoKey())),
        sealedSign(key, TWCoinTypeEthereum, account.address, TW::data(std::string("wrong")), sealedSerialize(sealedEthTransferNoKey())),
    };
    for (const auto& failure : failures) {
        EXPECT_NE(failure.error, TWStoredKeySignErrorOk);
        EXPECT_TRUE(failure.data.empty());
        const std::string message = failure.message;
        EXPECT_FALSE(message.empty());
        EXPECT_EQ(message.find("0x"), std::string::npos) << message;
        EXPECT_EQ(message.find(account.address), std::string::npos) << message;
    }
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

    const auto sealed = sealedOk(sealedSign(key, coin, account.address, gSealedPassword, sealedSerialize(input)));

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

// PSBT (swap), as the app's `signPsbt` builds it: V1 envelope, `signing_v2.psbt`, Bitcoin chain info,
// the account's public key. Fixture: the THORChain swap PSBT from TWBitcoinPsbtTests, whose input
// belongs to the default SegWit address of this private key.
TEST(SealedSigner, BitcoinPsbtByteIdentical) {
    const auto psbtKey = parse_hex("f00ffbe44c5c2838c13d2778854ac66b75e04eb6054f0241989e223223ad5e55");
    const auto psbt = parse_hex("70736274ff0100bc0200000001147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d000000000001011f6603010000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d00000000");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeBitcoin, psbtKey);
    const auto account = sealedOnlyAccount(key, TWCoinTypeBitcoin);

    BitcoinV2::Proto::SigningInput v2;
    v2.mutable_chain_info()->set_p2pkh_prefix(gBitcoinChain.p2pkh);
    v2.mutable_chain_info()->set_p2sh_prefix(gBitcoinChain.p2sh);
    v2.mutable_chain_info()->set_hrp(gBitcoinChain.hrp);
    const auto publicKey = parse_hex(account.publicKey);
    ASSERT_FALSE(publicKey.empty());
    v2.add_public_keys(publicKey.data(), publicKey.size());
    v2.mutable_psbt()->set_psbt(psbt.data(), psbt.size());
    Bitcoin::Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeBitcoin);
    *input.mutable_signing_v2() = v2;

    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(input)));
    Bitcoin::Proto::SigningOutput out;
    ASSERT_TRUE(out.ParseFromArray(sealed.data(), static_cast<int>(sealed.size())));
    ASSERT_EQ(out.signing_result_v2().error(), Common::Proto::OK) << out.signing_result_v2().error_message();
    EXPECT_EQ(hex(out.signing_result_v2().encoded()), "02000000000101147010db5fbcf619067c1090fec65c131443fbc80fb4aaeebe940e44206098c60000000000ffffffff0360ea000000000000160014f22a703617035ef7f490743d50f26ae08c30d0a70000000000000000426a403d3a474149412e41544f4d3a636f736d6f7331737377797a666d743675396a373437773537753438746778646575393573757a666c6d7175753a303a743a35303e12000000000000160014b139199ec796f36fc42e637f42da8e3e6720aa9d02483045022100b1229a008f20691639767bf925d6b8956ea957ccc633ad6b5de3618733a55e6b02205774d3320489b8a57a6f8de07f561de3e660ff8e587f6ac5422c49020cd4dc9101210306d8c664ea8fd2683eebea1d3114d90e0a5429e5783ba49b80ddabce04ff28f300000000");
    EXPECT_EQ(sealed, sealedUtxoBaseline(TWCoinTypeBitcoin, input, psbtKey));
}

// coin_type must name the coin being signed — a Bitcoin input signed as Litecoin is rejected
// (the class of bug where one coin's key signs another coin's claiming script).
TEST(SealedSigner, UtxoCoinTypeMismatchRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeLitecoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeLitecoin);
    auto input = sealedUtxoTransferNoKey(TWCoinTypeLitecoin, account, {48, 50, "ltc"}, "ltc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z0tamvsu");
    input.set_coin_type(TWCoinTypeBitcoin);
    EXPECT_EQ(sealedSign(key, TWCoinTypeLitecoin, account.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorInvalidInput);
}

// Only the BitcoinV2 envelope is accepted: a legacy V1-only input is rejected.
TEST(SealedSigner, UtxoWithoutSigningV2Rejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeBitcoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeBitcoin);
    Bitcoin::Proto::SigningInput input;
    input.set_amount(1000);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address(account.address);
    EXPECT_EQ(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorInvalidInput);
}

// A key supplied in either the V1 `private_key` or `signing_v2.private_keys` is rejected.
TEST(SealedSigner, UtxoInputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeBitcoin);
    const auto account = sealedOnlyAccount(key, TWCoinTypeBitcoin);
    const auto clean = sealedUtxoTransferNoKey(TWCoinTypeBitcoin, account, gBitcoinChain, "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");

    auto withV1Key = clean;
    withV1Key.add_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_EQ(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(withV1Key)).error, TWStoredKeySignErrorInvalidInput);

    auto withV2Key = clean;
    withV2Key.mutable_signing_v2()->add_private_keys(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_EQ(sealedSign(key, TWCoinTypeBitcoin, account.address, gSealedPassword, sealedSerialize(withV2Key)).error, TWStoredKeySignErrorInvalidInput);
}

// ---------------------------------------------------------------------------------------------
// Tron — native transfer, TRC20 transfer and dApp `raw_json` transactions
// ---------------------------------------------------------------------------------------------

// Fixed timestamps and block reference (from TronSigner.SignTransfer) so the output is deterministic.
static void sealedTronSetBlock(Tron::Proto::Transaction& transaction) {
    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);
    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);
}

static Tron::Proto::SigningInput sealedTronTransferNoKey(const std::string& owner) {
    Tron::Proto::SigningInput in;
    auto& transaction = *in.mutable_transaction();
    auto& transfer = *transaction.mutable_transfer();
    transfer.set_owner_address(owner);
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(2000000);
    sealedTronSetBlock(transaction);
    return in;
}

static Tron::Proto::SigningInput sealedTronTrc20NoKey(const std::string& owner) {
    Tron::Proto::SigningInput in;
    auto& transaction = *in.mutable_transaction();
    auto& transfer = *transaction.mutable_transfer_trc20_contract();
    transfer.set_contract_address("TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t"); // USDT
    transfer.set_owner_address(owner);
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    const auto amount = store(uint256_t(1000000));
    transfer.set_amount(amount.data(), amount.size());
    transaction.set_fee_limit(10000000);
    sealedTronSetBlock(transaction);
    return in;
}

static Data sealedTronBaseline(Tron::Proto::SigningInput in, const Data& keyBytes) {
    in.set_private_key(keyBytes.data(), keyBytes.size());
    Data out;
    anyCoinSign(TWCoinTypeTron, sealedSerialize(in), out);
    return out;
}

static Tron::Proto::SigningOutput sealedTronParse(const Data& out) {
    Tron::Proto::SigningOutput parsed;
    EXPECT_TRUE(parsed.ParseFromArray(out.data(), static_cast<int>(out.size())));
    return parsed;
}

// dApp form of a transaction: the node JSON (txID, raw_data, raw_data_hex) without a signature.
static std::string sealedTronRawJson(const Data& signedOutput) {
    auto json = nlohmann::json::parse(sealedTronParse(signedOutput).json());
    json.erase("signature");
    return json.dump();
}

TEST(SealedSigner, TronByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeTron);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeTron, account.derivationPath);

    for (const auto& input : {sealedTronTransferNoKey(account.address), sealedTronTrc20NoKey(account.address)}) {
        const auto sealed = sealedOk(sealedSign(key, TWCoinTypeTron, account.address, gSealedPassword, sealedSerialize(input)));
        const auto parsed = sealedTronParse(sealed);
        EXPECT_EQ(parsed.error(), Common::Proto::OK);
        EXPECT_EQ(parsed.signature().size(), 65ul);
        EXPECT_EQ(sealed, sealedTronBaseline(input, pk.bytes));
    }
    EXPECT_EQ(key.accounts.size(), 1ul);
}

TEST(SealedSigner, TronPrivateKeyKeystoreByteIdentical) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeTron, gSealedRawKey);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    const auto input = sealedTronTransferNoKey(account.address);
    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeTron, account.address, gSealedPassword, sealedSerialize(input)));
    EXPECT_EQ(sealedTronParse(sealed).error(), Common::Proto::OK);
    EXPECT_EQ(sealed, sealedTronBaseline(input, gSealedRawKey));
}

// A dApp transaction (raw_json) signs to the same bytes as the normal path — and as the structured
// transaction it was built from.
TEST(SealedSigner, TronRawJsonByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeTron);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeTron, account.derivationPath);

    for (const auto& structured : {sealedTronTransferNoKey(account.address), sealedTronTrc20NoKey(account.address)}) {
        const auto structuredOut = sealedTronBaseline(structured, pk.bytes);
        Tron::Proto::SigningInput input;
        input.set_raw_json(sealedTronRawJson(structuredOut));

        const auto sealed = sealedOk(sealedSign(key, TWCoinTypeTron, account.address, gSealedPassword, sealedSerialize(input)));
        const auto parsed = sealedTronParse(sealed);
        EXPECT_EQ(parsed.error(), Common::Proto::OK);
        EXPECT_EQ(sealed, sealedTronBaseline(input, pk.bytes));
        EXPECT_EQ(hex(parsed.signature()), hex(sealedTronParse(structuredOut).signature()));
        EXPECT_EQ(hex(parsed.id()), hex(sealedTronParse(structuredOut).id()));
    }
}

// raw_json whose txID does not match the transaction (raw_data, or raw_data_hex) is refused with
// Error_tx_hash_mismatch and no signature — the same output as the normal path.
TEST(SealedSigner, TronRawJsonTxIdMismatchRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeTron);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeTron, account.derivationPath);
    const auto json = nlohmann::json::parse(sealedTronRawJson(sealedTronBaseline(sealedTronTransferNoKey(account.address), pk.bytes)));

    // (1) txID of another transaction; (2) raw_data_hex of another transaction (txID still matches raw_data).
    auto wrongTxId = json;
    wrongTxId["txID"] = "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb";
    auto wrongRawDataHex = json;
    auto otherHex = json["raw_data_hex"].get<std::string>();
    otherHex.back() = otherHex.back() == '0' ? '1' : '0';
    wrongRawDataHex["raw_data_hex"] = otherHex;

    for (const auto& tampered : {wrongTxId, wrongRawDataHex}) {
        Tron::Proto::SigningInput input;
        input.set_raw_json(tampered.dump());
        const auto sealed = sealedOk(sealedSign(key, TWCoinTypeTron, account.address, gSealedPassword, sealedSerialize(input)));
        const auto parsed = sealedTronParse(sealed);
        EXPECT_EQ(parsed.error(), Common::Proto::Error_tx_hash_mismatch);
        EXPECT_TRUE(parsed.signature().empty());
        EXPECT_EQ(sealed, sealedTronBaseline(input, pk.bytes));
    }
}

TEST(SealedSigner, TronInputWithPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeTron);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    auto input = sealedTronTransferNoKey(account.address);
    input.set_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    EXPECT_EQ(sealedSign(key, TWCoinTypeTron, account.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorInvalidInput);
}

// Unknown address (no Tron account with it), and a Tron address used with another coin's account.
TEST(SealedSigner, TronUnknownAddressRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeTron);
    const auto account = sealedOnlyAccount(key, TWCoinTypeTron);
    const auto input = sealedSerialize(sealedTronTransferNoKey(account.address));
    EXPECT_EQ(sealedSign(key, TWCoinTypeTron, "THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV", gSealedPassword, input).error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_NE(sealedSign(key, TWCoinTypeEthereum, account.address, gSealedPassword, input).error, TWStoredKeySignErrorOk);
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// ---------------------------------------------------------------------------------------------
// Solana — library-built transactions, dApp `raw_message` transactions, and off-chain messages
// ---------------------------------------------------------------------------------------------

static const char* gSealedSolRecipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd";
static const char* gSealedSolBlockhash = "8oFmGuWUpsy8h8WP8AXTp3yhcLt4gQJRG5GxNcK7jfhX";

static Solana::Proto::SigningInput sealedSolTransferNoKey(bool v0) {
    Solana::Proto::SigningInput in;
    in.set_recent_blockhash(gSealedSolBlockhash);
    in.set_v0_msg(v0);
    auto& transfer = *in.mutable_transfer_transaction();
    transfer.set_recipient(gSealedSolRecipient);
    transfer.set_value(42000);
    return in;
}

static Solana::Proto::SigningInput sealedSolTokenTransferNoKey() {
    Solana::Proto::SigningInput in;
    in.set_recent_blockhash(gSealedSolBlockhash);
    auto& transfer = *in.mutable_token_transfer_transaction();
    transfer.set_token_mint_address("SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt");
    transfer.set_sender_token_address("EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP");
    transfer.set_recipient_token_address("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    transfer.set_amount(4000);
    transfer.set_decimals(6);
    return in;
}

static Data sealedSolBaseline(Solana::Proto::SigningInput in, const Data& keyBytes) {
    in.set_private_key(keyBytes.data(), keyBytes.size());
    Data out;
    anyCoinSign(TWCoinTypeSolana, sealedSerialize(in), out);
    return out;
}

static Solana::Proto::SigningOutput sealedSolParse(const Data& out) {
    Solana::Proto::SigningOutput parsed;
    EXPECT_TRUE(parsed.ParseFromArray(out.data(), static_cast<int>(out.size())));
    return parsed;
}

// dApp form of a signed transaction: its message decoded into `raw_message`, without signatures.
static Solana::Proto::SigningInput sealedSolRawMessageInput(const Data& signedOutput) {
    const auto txBytes = Base58::decode(sealedSolParse(signedOutput).encoded());
    const Rust::TWDataWrapper txData(txBytes);
    const Rust::TWDataWrapper decoded = Rust::tw_transaction_decoder_decode(static_cast<uint32_t>(TWCoinTypeSolana), txData.get());
    const auto decodedBytes = decoded.toDataOrDefault();
    Solana::Proto::DecodingTransactionOutput decodedOut;
    EXPECT_TRUE(decodedOut.ParseFromArray(decodedBytes.data(), static_cast<int>(decodedBytes.size())));
    EXPECT_EQ(decodedOut.error(), Common::Proto::OK);
    Solana::Proto::SigningInput in;
    *in.mutable_raw_message() = decodedOut.transaction();
    in.mutable_raw_message()->clear_signatures();
    return in;
}

// The unsigned message bytes of a library-built transaction (what its signature covers).
static Data sealedSolTxMessageBytes(const Data& signedOutput) {
    return Base58::decode(sealedSolParse(signedOutput).unsigned_tx());
}

TEST(SealedSigner, SolanaByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeSolana, account.derivationPath);

    for (const auto& input : {sealedSolTransferNoKey(false), sealedSolTransferNoKey(true), sealedSolTokenTransferNoKey()}) {
        const auto sealed = sealedOk(sealedSign(key, TWCoinTypeSolana, account.address, gSealedPassword, sealedSerialize(input)));
        const auto parsed = sealedSolParse(sealed);
        EXPECT_EQ(parsed.error(), Common::Proto::OK) << parsed.error_message();
        EXPECT_FALSE(parsed.encoded().empty());
        EXPECT_EQ(sealed, sealedSolBaseline(input, pk.bytes));
    }
    EXPECT_EQ(key.accounts.size(), 1ul);
}

TEST(SealedSigner, SolanaPrivateKeyKeystoreByteIdentical) {
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("n", gSealedPassword, TWCoinTypeSolana, gSealedRawKey);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto input = sealedSolTransferNoKey(false);
    const auto sealed = sealedOk(sealedSign(key, TWCoinTypeSolana, account.address, gSealedPassword, sealedSerialize(input)));
    EXPECT_EQ(sealedSolParse(sealed).error(), Common::Proto::OK);
    EXPECT_EQ(sealed, sealedSolBaseline(input, gSealedRawKey));
}

// dApp transactions (raw_message, legacy and v0) sign to the same bytes as the normal path, and to
// the same transaction as the library-built one they were decoded from.
TEST(SealedSigner, SolanaRawMessageByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeSolana, account.derivationPath);

    for (const bool v0 : {false, true}) {
        const auto built = sealedSolBaseline(sealedSolTransferNoKey(v0), pk.bytes);
        const auto input = sealedSolRawMessageInput(built);
        ASSERT_EQ(input.raw_message().has_v0(), v0);

        const auto sealed = sealedOk(sealedSign(key, TWCoinTypeSolana, account.address, gSealedPassword, sealedSerialize(input)));
        const auto parsed = sealedSolParse(sealed);
        EXPECT_EQ(parsed.error(), Common::Proto::OK) << parsed.error_message();
        EXPECT_EQ(sealed, sealedSolBaseline(input, pk.bytes)) << v0;
        EXPECT_EQ(parsed.encoded(), sealedSolParse(built).encoded()) << v0;
    }
}

// Every key field must be empty: private_key, fee_payer_private_key, nonce_account_private_key.
TEST(SealedSigner, SolanaInputWithAnyPrivateKeyRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);

    auto withKey = sealedSolTransferNoKey(false);
    withKey.set_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    auto withFeePayerKey = sealedSolTokenTransferNoKey();
    withFeePayerKey.set_fee_payer_private_key(gSealedRawKey.data(), gSealedRawKey.size());
    Solana::Proto::SigningInput withNonceKey;
    withNonceKey.set_recent_blockhash(gSealedSolBlockhash);
    auto& createNonce = *withNonceKey.mutable_create_nonce_account();
    createNonce.set_rent(10000000);
    createNonce.set_nonce_account_private_key(gSealedRawKey.data(), gSealedRawKey.size());

    for (const auto& input : {withKey, withFeePayerKey, withNonceKey}) {
        EXPECT_EQ(sealedSign(key, TWCoinTypeSolana, account.address, gSealedPassword, sealedSerialize(input)).error, TWStoredKeySignErrorInvalidInput);
    }
}

TEST(SealedSigner, SolanaUnknownAddressRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto input = sealedSerialize(sealedSolTransferNoKey(false));
    EXPECT_EQ(sealedSign(key, TWCoinTypeSolana, gSealedSolRecipient, gSealedPassword, input).error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_EQ(key.accounts.size(), 1ul);
}

// Off-chain messages: same signature as the Solana message signer (UTF-8), and the raw ed25519
// signature for any other bytes.
TEST(SealedSigner, SolanaMessageByteIdentical) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeSolana, account.derivationPath);

    for (const std::string text : {"Hello world", "Sign in to example.com\nNonce: 8f2a", "", "你好"}) {
        const auto sealed = sealedOk(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, gSealedPassword, TW::data(text)));

        Solana::Proto::MessageSigningInput in;
        in.set_private_key(pk.bytes.data(), pk.bytes.size());
        in.set_message(text);
        const Rust::TWDataWrapper inData(sealedSerialize(in));
        const Rust::TWDataWrapper outPtr = Rust::tw_message_signer_sign(static_cast<uint32_t>(TWCoinTypeSolana), inData.get());
        const auto outBytes = outPtr.toDataOrDefault();
        Solana::Proto::MessageSigningOutput out;
        ASSERT_TRUE(out.ParseFromArray(outBytes.data(), static_cast<int>(outBytes.size())));
        ASSERT_EQ(out.error(), Common::Proto::OK);

        ASSERT_EQ(sealed.size(), 64ul) << text;
        EXPECT_EQ(hex(sealed), hex(Base58::decode(out.signature()))) << text;
    }

    // Non-UTF-8 bytes and an off-chain-message-format (0xff "solana offchain") payload are signed raw.
    for (const auto& bytes : {parse_hex("00ff80fe"), TW::data(std::string("\xff" "solana offchain\x00\x00", 18))}) {
        const auto sealed = sealedOk(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, gSealedPassword, bytes));
        ASSERT_EQ(sealed.size(), 64ul) << hex(bytes);
        EXPECT_EQ(hex(sealed), hex(pk.sign(bytes, TWCurveED25519))) << hex(bytes);
    }
}

// A "message" that is exactly a transaction message (legacy, v0, SPL) is refused before the keystore
// is decrypted, so a dApp cannot get a transaction signed this way. The same bytes padded or truncated
// are not a transaction message and are signed, as in the App.
TEST(SealedSigner, SolanaMessageThatIsTransactionRejected) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto pk = key.wallet(gSealedPassword).getKey(TWCoinTypeSolana, account.derivationPath);

    for (const auto& input : {sealedSolTransferNoKey(false), sealedSolTransferNoKey(true), sealedSolTokenTransferNoKey()}) {
        const auto txMessage = sealedSolTxMessageBytes(sealedSolBaseline(input, pk.bytes));
        ASSERT_FALSE(txMessage.empty());
        EXPECT_TRUE(isSolanaTransactionMessage(txMessage));
        EXPECT_EQ(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, gSealedPassword, txMessage).error, TWStoredKeySignErrorTransactionBytesRefused);
        // Wrong password would throw if decryption were reached.
        EXPECT_EQ(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, TW::data(std::string("wrong")), txMessage).error, TWStoredKeySignErrorTransactionBytesRefused);

        auto padded = txMessage;
        padded.push_back(0);
        const auto truncated = Data(txMessage.begin(), txMessage.end() - 1);
        for (const auto& bytes : {padded, truncated}) {
            EXPECT_FALSE(isSolanaTransactionMessage(bytes));
            const auto sealed = sealedOk(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, gSealedPassword, bytes));
            EXPECT_EQ(hex(sealed), hex(pk.sign(bytes, TWCurveED25519)));
        }
    }
}

// Parity with the App rule (SC-140951): the vectors of mobile-monorepo
// `solana-transaction-message.parity.test.ts` / `.test-support.ts` (Kotlin TestSolanaTransactionMessage).
static Data sealedSolLegacyTransferMessage(byte numRequiredSignatures = 1, byte programIndex = 2) {
    Data m{numRequiredSignatures, 0, 1, 3};
    for (const byte fill : {1, 2, 0, 3}) {
        m.insert(m.end(), 32, fill);
    }
    append(m, Data{1, programIndex, 2, 0, 1, 12});
    append(m, Data{2, 0, 0, 0, 0xe8, 3, 0, 0, 0, 0, 0, 0});
    return m;
}

static Data sealedSolV0MessageWithLookup(byte accountIndex = 2, byte programIndex = 1) {
    Data m{0x80, 1, 0, 1, 2};
    for (const byte fill : {1, 0, 3}) {
        m.insert(m.end(), 32, fill);
    }
    append(m, Data{1, programIndex, 2, 0, accountIndex, 0});
    m.push_back(1);
    m.insert(m.end(), 32, 9);
    append(m, Data{1, 0, 0});
    return m;
}

static const char* gSealedSolDappRawData =
    "hRVNTcYhAHz6jnDZ77Vyz998wVUKdtREqYLt5dMDk1Qg7JNgVC3Tf8ZH1AsRbdQSufdXvCjbYRoBjGwNQNMwXZYkBFgRcfYFxyTvTxkhSEw537AUkEHC7Qc5rrQK1b65QEnpn5z4vqeuhzBWiJ4c8J3x6HRyS5WLA8CyeTegbs7hGpBJkgNxahKTYVXKeZBA1F6NoZ31cGyJih68Myf4qZVo5x7JL8Vzic6JZXwP6855tRRKb7ctWdqT7MNCJcJEFiapnpfYZZEfUqok9hGr7dgFkohoE8MdS6zHhBc9qqAPCmHwzbkL6NC9o61hVd5joUvKtAw43c93RByzueSoUv8BeLMBLgdKgufh3j2EFVjh5QTNmXzLSPBLj2qdDTyoqgPujc4rneUBHdTZPCcb5AzzWssvs6hoJ5U9ewjjR216ZB7DQnU2f4xPuHrUqUzh4MU2Geqj7YZKVeRDWmLhihEpeV5ie4UBZxCkZfAvYv4rhskMdmG24zyB29DWAarkbztWNS6JF88Piit7dHb4KBNzP4ZtyoBoTk9Nqt9LQBAS7B498wy7mFnepsvXrc919HvSMj4XNnf3DPvKNN1UUKejv9p2wudmn9UL1sHQTnvJKsduRBVhjGAUCZKx6B41mJHr44iBDy7cjYPu2jmeq7gDMsC4kDeLDFbPLFgVEQr4kT6JeYDUZmyvLj5KNS85BHi8czckhJMf1Z8gr5pJgMGgdiBxEubau6NU1jEwC4zb9dbNCR9kLAibPj9HPcRW46jANvsEojawJ2MCU2HLyaHS";

TEST(SealedSigner, SolanaTransactionMessageParity) {
    // Transaction messages.
    EXPECT_TRUE(isSolanaTransactionMessage(sealedSolLegacyTransferMessage()));
    const auto dappMessage = Base58::decode(gSealedSolDappRawData);
    ASSERT_FALSE(dappMessage.empty());
    EXPECT_TRUE(isSolanaTransactionMessage(dappMessage));
    EXPECT_TRUE(isSolanaTransactionMessage(sealedSolV0MessageWithLookup()));

    // Not transaction messages.
    EXPECT_FALSE(isSolanaTransactionMessage(sealedSolV0MessageWithLookup(/*accountIndex*/ 3)));          // past lookup keys
    EXPECT_FALSE(isSolanaTransactionMessage(sealedSolV0MessageWithLookup(2, /*programIndex*/ 2)));       // program in lookup table
    auto v1 = sealedSolV0MessageWithLookup();
    v1[0] = 0x81;
    EXPECT_FALSE(isSolanaTransactionMessage(v1));                                                        // unsupported version
    EXPECT_FALSE(isSolanaTransactionMessage(TW::data(std::string("Sign in to example.com"))));
    EXPECT_FALSE(isSolanaTransactionMessage(Data()));
    EXPECT_FALSE(isSolanaTransactionMessage(Data(32, 7)));
    const auto wcMessage = Base58::decode("X3CUgCGzyn43DTAbUKnTMDzcGWMooJT2hPSZinjfN1QUgVNYYfeoJ5zg6i4MY6ZgL16UZzMCKew");
    ASSERT_FALSE(wcMessage.empty());
    EXPECT_FALSE(isSolanaTransactionMessage(wcMessage));                                                 // WalletConnect signMessage
    auto offchain = TW::data(std::string("\xff" "solana offchain"));
    append(offchain, sealedSolLegacyTransferMessage());
    EXPECT_FALSE(isSolanaTransactionMessage(offchain));                                                  // off-chain message
    auto trailing = sealedSolLegacyTransferMessage();
    trailing.push_back(0);
    EXPECT_FALSE(isSolanaTransactionMessage(trailing));                                                  // trailing byte
    const auto legacy = sealedSolLegacyTransferMessage();
    EXPECT_FALSE(isSolanaTransactionMessage(Data(legacy.begin(), legacy.end() - 1)));                    // missing byte
    EXPECT_FALSE(isSolanaTransactionMessage(sealedSolLegacyTransferMessage(/*numRequiredSignatures*/ 0)));
    EXPECT_FALSE(isSolanaTransactionMessage(sealedSolLegacyTransferMessage(1, /*programIndex*/ 3)));
}

TEST(SealedSigner, SolanaMessageRejectsOtherCoinAndUnknownAddress) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("n", gSealedPassword, gSealedMnemonic, TWCoinTypeSolana);
    const auto account = sealedOnlyAccount(key, TWCoinTypeSolana);
    const auto message = TW::data(std::string("hello"));
    EXPECT_EQ(sealedSignSolanaMessage(key, TWCoinTypeSolana, gSealedSolRecipient, gSealedPassword, message).error, TWStoredKeySignErrorAccountNotFound);
    EXPECT_EQ(sealedSignSolanaMessage(key, TWCoinTypeEthereum, account.address, gSealedPassword, message).error, TWStoredKeySignErrorUnsupportedCoin);
    EXPECT_EQ(sealedSignSolanaMessage(key, TWCoinTypeSolana, account.address, TW::data(std::string("wrong")), message).error, TWStoredKeySignErrorWrongPassword);
}

} // namespace TW::Keystore::tests
