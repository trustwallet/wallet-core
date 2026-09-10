// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SealedSigner.h"

#include "Coin.h"
#include "PrivateKey.h"
#include "proto/Ethereum.pb.h"

#include <TrezorCrypto/memzero.h>

namespace TW::Keystore {

static void wipe(Data& d) {
    if (!d.empty()) {
        memzero(d.data(), d.size());
    }
}

// EVM chains share the Ethereum SigningInput, which carries a singular `private_key` field.
// Returns the input bytes with the derived key injected.
static Data injectKeyEthereum(const Data& input, const PrivateKey& key) {
    Ethereum::Proto::SigningInput in;
    in.ParseFromArray(input.data(), static_cast<int>(input.size()));
    in.set_private_key(key.bytes.data(), key.bytes.size());
    Data out = TW::data(in.SerializeAsString());
    in.clear_private_key(); // drop protobuf's copy of the key
    return out;
}

Data sealedSign(StoredKey& storedKey, TWCoinType coin, const Data& password, const Data& signingInput) {
    // Coins supported by the sealed path so far. Each must have a per-coin byte-identical test
    // proving flag-ON output == flag-OFF output before it is added here.
    switch (coin) {
    case TWCoinTypeEthereum:
        break;
    default:
        return {}; // not yet supported by sealed signing
    }

    // (1) Decrypt + derive the signing key ONCE, inside the library.
    //     (Account-based coins need a single key; the UTXO multi-key path — decrypt once, derive
    //      many from the in-memory HDWallet — is added in a later increment.)
    //
    // TODO(S2.1 follow-up): this derives at the coin's DEFAULT derivation and DEFAULT account only.
    //   A custom derivation path or a non-default account is not yet threaded through, so signing
    //   such a wallet uses the default account's key and won't match its address. Add a
    //   derivation/path parameter (see claude-handoff-S2-UTXO-sealed-sign-findings.md) so callers on
    //   custom paths / multi-account wallets are supported.
    PrivateKey privateKey = storedKey.privateKey(coin, password);

    // (2) Inject the key into the coin's SigningInput proto.
    Data injected = injectKeyEthereum(signingInput, privateKey);

    // (3) Sign through the normal internal dispatcher.
    Data output;
    anyCoinSign(coin, injected, output);

    // (4) Zeroize key material — it never leaves this function.
    //     (Best-effort here; full SecureBytes-based wiping of PrivateKey internals is a later
    //      hardening step.)
    Data keyCopy = privateKey.bytes;
    wipe(keyCopy);
    wipe(injected);

    return output;
}

} // namespace TW::Keystore
