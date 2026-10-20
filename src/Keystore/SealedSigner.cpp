// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SealedSigner.h"

#include "Coin.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"

#include <TrezorCrypto/memzero.h>

#include <optional>

namespace TW::Keystore {

static void wipe(Data& d) {
    if (!d.empty()) {
        memzero(d.data(), d.size());
    }
}

static void wipe(std::string& s) {
    if (!s.empty()) {
        memzero(s.data(), s.size());
    }
}

// Derives the key of the account identified by (`coin`, `address`), or returns std::nullopt when
// the stored key has no such account or the derived key does not belong to it. Never mutates the
// stored key (no accounts are added or updated).
//
// The account's stored derivation path is only trusted after the key derived from it is proven to
// produce `address` — so a custom path or a non-default account signs with its own key, and a
// tampered or stale account record fails closed. The account lookup runs before the (expensive,
// scrypt-based) decryption.
static std::optional<PrivateKey> deriveSigningKey(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password) {
    std::optional<Account> account;
    for (const auto& candidate : storedKey.getAccounts(coin)) {
        if (candidate.address != address) {
            continue;
        }
        if (account.has_value() && account->derivationPath != candidate.derivationPath) {
            return std::nullopt; // same address recorded under different paths: ambiguous
        }
        account = candidate;
    }
    if (!account.has_value()) {
        return std::nullopt;
    }

    std::optional<PrivateKey> key;
    if (storedKey.type == StoredKeyType::privateKey) {
        Data secret = storedKey.payload.decrypt(password);
        key.emplace(secret, TWCoinTypeCurve(coin));
        wipe(secret);
    } else {
        const auto wallet = storedKey.wallet(password);
        key.emplace(wallet.getKey(coin, account->derivationPath));
    }

    if (TW::deriveAddress(coin, *key, account->derivation) != address) {
        return std::nullopt;
    }
    return key;
}

// Serializes the key-injected input straight into a Data buffer (no intermediate std::string),
// signs it through the normal internal dispatcher, and wipes the serialized copy.
template <typename Input>
static Data serializeAndSign(TWCoinType coin, const Input& in) {
    Data injected(in.ByteSizeLong());
    in.SerializeToArray(injected.data(), static_cast<int>(injected.size()));
    Data output;
    anyCoinSign(coin, injected, output);
    wipe(injected);
    return output;
}

// EVM chains share the Ethereum SigningInput, which carries a singular `private_key` field.
static Data signEthereum(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Ethereum::Proto::SigningInput in;
    if (!in.ParseFromArray(signingInput.data(), static_cast<int>(signingInput.size())) || !in.private_key().empty()) {
        return {};
    }
    const auto key = deriveSigningKey(storedKey, coin, address, password);
    if (!key.has_value()) {
        return {};
    }
    in.set_private_key(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign(coin, in);
    wipe(*in.mutable_private_key()); // clear_* alone does not zero memory
    return output;
}

// UTXO chains: only the BitcoinV2 envelope the app signs with is accepted — a `Bitcoin.SigningInput`
// whose `signing_v2` carries the transaction and whose `coin_type` names `coin`. The single account
// key goes into `signing_v2.private_keys` (the app's single-address model: every input and the change
// output use the account's one address, so one key signs the whole transaction).
static Data signUtxo(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Bitcoin::Proto::SigningInput in;
    if (!in.ParseFromArray(signingInput.data(), static_cast<int>(signingInput.size()))) {
        return {};
    }
    if (!in.has_signing_v2() || in.coin_type() != static_cast<uint32_t>(coin) ||
        in.private_key_size() != 0 || in.signing_v2().private_keys_size() != 0) {
        return {};
    }
    const auto key = deriveSigningKey(storedKey, coin, address, password);
    if (!key.has_value()) {
        return {};
    }
    auto* v2 = in.mutable_signing_v2();
    v2->add_private_keys(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign(coin, in);
    wipe(*v2->mutable_private_keys(0));
    return output;
}

Data sealedSign(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    // Coins supported by the sealed path so far. Each must have a per-coin byte-identical test
    // proving flag-ON output == flag-OFF output before it is added here.
    //
    // The PrivateKey and HDWallet destructors zero their own buffers; the serialized input and the
    // proto's copy of the key are wiped by the per-family signers above.
    // (Copies made inside the per-coin signer itself are a later hardening step.)
    switch (coin) {
    case TWCoinTypeEthereum:
        return signEthereum(storedKey, coin, address, password, signingInput);
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
    case TWCoinTypeDogecoin:
        return signUtxo(storedKey, coin, address, password, signingInput);
    default:
        return {}; // not yet supported by sealed signing
    }
}

} // namespace TW::Keystore
