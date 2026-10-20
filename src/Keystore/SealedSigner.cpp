// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SealedSigner.h"

#include "Coin.h"
#include "PrivateKey.h"
#include "proto/Ethereum.pb.h"

#include <TrezorCrypto/memzero.h>

#include <optional>

namespace TW::Keystore {

static void wipe(Data& d) {
    if (!d.empty()) {
        memzero(d.data(), d.size());
    }
}

// Derives the signing key for `coin`, or returns std::nullopt when the sealed path cannot pick the
// key unambiguously. Never mutates the stored key (no accounts are added or updated).
//
// Fails closed unless the stored key holds exactly one account for `coin`, that account is at the
// coin's DEFAULT derivation path, and the derived key's address matches the account's address —
// otherwise we could sign with a key that does not belong to the caller's account.
// All cheap checks run before the (expensive, scrypt-based) decryption.
//
// TODO(S2.1 follow-up): accept a derivation/path parameter to support custom paths and
//   multi-account wallets (see claude-handoff-S2-UTXO-sealed-sign-findings.md).
static std::optional<PrivateKey> deriveSigningKey(const StoredKey& storedKey, TWCoinType coin, const Data& password) {
    const auto accounts = storedKey.getAccounts(coin);
    if (accounts.size() != 1) {
        return std::nullopt;
    }
    const auto& account = accounts.front();
    const auto defaultPath = TW::derivationPath(coin);

    std::optional<PrivateKey> key;
    if (storedKey.type == StoredKeyType::privateKey) {
        Data secret = storedKey.payload.decrypt(password);
        key.emplace(secret, TWCoinTypeCurve(coin));
        wipe(secret);
    } else {
        if (account.derivationPath != defaultPath) {
            return std::nullopt;
        }
        const auto wallet = storedKey.wallet(password);
        key.emplace(wallet.getKey(coin, defaultPath));
    }

    // Bind the key to the caller's account.
    if (TW::deriveAddress(coin, *key) != account.address) {
        return std::nullopt;
    }
    return key;
}

Data sealedSign(const StoredKey& storedKey, TWCoinType coin, const Data& password, const Data& signingInput) {
    // Coins supported by the sealed path so far. Each must have a per-coin byte-identical test
    // proving flag-ON output == flag-OFF output before it is added here.
    switch (coin) {
    case TWCoinTypeEthereum:
        break;
    default:
        return {}; // not yet supported by sealed signing
    }

    // (1) Validate the input before touching the keystore. EVM chains share the Ethereum
    //     SigningInput, which carries a singular `private_key` field: the caller must leave it empty.
    Ethereum::Proto::SigningInput in;
    if (!in.ParseFromArray(signingInput.data(), static_cast<int>(signingInput.size())) || !in.private_key().empty()) {
        return {};
    }

    // (2) Decrypt + derive the signing key ONCE, inside the library.
    //     (Account-based coins need a single key; the UTXO multi-key path — decrypt once, derive
    //      many from the in-memory HDWallet — is added in a later increment.)
    const auto privateKey = deriveSigningKey(storedKey, coin, password);
    if (!privateKey.has_value()) {
        return {};
    }

    // (3) Inject the key and serialize straight into a Data buffer (no intermediate std::string).
    in.set_private_key(privateKey->bytes.data(), privateKey->bytes.size());
    Data injected(in.ByteSizeLong());
    in.SerializeToArray(injected.data(), static_cast<int>(injected.size()));
    auto* protoKey = in.mutable_private_key();
    memzero(protoKey->data(), protoKey->size()); // clear_* alone does not zero memory
    in.clear_private_key();

    // (4) Sign through the normal internal dispatcher.
    Data output;
    anyCoinSign(coin, injected, output);

    // (5) Zeroize key material — it never leaves this function. The PrivateKey and HDWallet
    //     destructors zero their own buffers; the serialized input copy is wiped here.
    //     (Copies made inside the per-coin signer itself are a later hardening step.)
    wipe(injected);

    return output;
}

} // namespace TW::Keystore
