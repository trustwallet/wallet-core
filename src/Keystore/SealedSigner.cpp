// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SealedSigner.h"

#include "Coin.h"
#include "Ethereum/Address.h"
#include "Ethereum/MessageSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"
#include "rust/Wrapper.h"

#include <TrezorCrypto/memzero.h>

#include <algorithm>
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

// EVM chains share the Ethereum SigningInput, which carries a singular `private_key` field. The key
// comes from the account of `coin` itself (e.g. Smart Chain, or Ethereum Classic on its own path),
// but signing always runs under Ethereum, exactly like the app does today (the chain is selected by
// the input's `chain_id`, not by the coin).
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
    auto output = serializeAndSign(TWCoinTypeEthereum, in);
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
    if (TWCoinTypeBlockchain(coin) == TWBlockchainEthereum) {
        return signEthereum(storedKey, coin, address, password, signingInput);
    }
    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
    case TWCoinTypeDogecoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeDash:
    case TWCoinTypeViacoin:
    case TWCoinTypeDigiByte:
    case TWCoinTypeRavencoin:
    case TWCoinTypeFiro:
    case TWCoinTypeQtum:
    case TWCoinTypeZcash:
    case TWCoinTypeZelcash:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeDecred:
        return signUtxo(storedKey, coin, address, password, signingInput);
    default:
        return {}; // not yet supported by sealed signing
    }
}

// ---------------------------------------------------------------------------------------------
// EVM message signing
//
// Each function computes the digest itself from structured input (never signs a caller-supplied
// raw hash), so a sealed key can only produce the kind of signature the function is named for.
// Digests that wallet-core computes in Rust (Biz, EIP-7702) are obtained without the key; the key
// only ever signs inside C++, where `PrivateKey` wipes itself.
// ---------------------------------------------------------------------------------------------

static bool isEvm(TWCoinType coin) {
    return TWCoinTypeBlockchain(coin) == TWBlockchainEthereum;
}

// Signs a 32-byte digest and returns r || s || v with the Ethereum v offset (27/28).
static Data signEvmDigest(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& digest) {
    if (!isEvm(coin) || digest.size() != 32) {
        return {};
    }
    const auto key = deriveSigningKey(storedKey, coin, address, password);
    if (!key.has_value()) {
        return {};
    }
    auto signature = key->sign(digest, TWCurveSECP256k1);
    if (signature.size() != 65) {
        return {};
    }
    signature[64] += 27;
    return signature;
}

Data sealedSignEthereumPersonalMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message) {
    const std::string prefix = "\x19" "Ethereum Signed Message:\n" + std::to_string(message.size());
    Data prefixed(prefix.begin(), prefix.end());
    append(prefixed, message);
    return signEvmDigest(storedKey, coin, address, password, Hash::keccak256(prefixed));
}

Data sealedSignEthereumTypedData(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const std::string& typedDataJson) {
    Data digest;
    try {
        digest = Ethereum::MessageSigner::typedDataPreImageHash(typedDataJson);
    } catch (...) {
        return {};
    }
    return signEvmDigest(storedKey, coin, address, password, digest);
}

Data sealedSignBizUserOpHash(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                             const Data& chainId, const std::string& codeAddress, const std::string& codeName, const std::string& codeVersion,
                             const std::string& typeHash, const std::string& domainSeparatorHash, const std::string& userOpHash) {
    // The signed Biz hash always names the signing account itself as `sender`.
    const Rust::TWDataWrapper chainIdRust = chainId;
    const Rust::TWStringWrapper codeAddressRust = codeAddress;
    const Rust::TWStringWrapper codeNameRust = codeName;
    const Rust::TWStringWrapper codeVersionRust = codeVersion;
    const Rust::TWStringWrapper typeHashRust = typeHash;
    const Rust::TWStringWrapper domainSeparatorHashRust = domainSeparatorHash;
    const Rust::TWStringWrapper senderRust = address;
    const Rust::TWStringWrapper userOpHashRust = userOpHash;
    const Rust::TWDataWrapper encoded = Rust::tw_biz_get_encoded_hash(chainIdRust.get(), codeAddressRust.get(), codeNameRust.get(), codeVersionRust.get(),
                                                                      typeHashRust.get(), domainSeparatorHashRust.get(), senderRust.get(), userOpHashRust.get());
    if (!encoded.ptr) {
        return {};
    }
    return signEvmDigest(storedKey, coin, address, password, encoded.toDataOrDefault());
}

// Minimal big-endian hex with a "0x" prefix and at least one byte, as the Rust serializer writes U256.
static std::string compactHex(const Data& bigEndian) {
    auto first = std::find_if(bigEndian.begin(), bigEndian.end(), [](byte b) { return b != 0; });
    if (first == bigEndian.end()) {
        return "0x00";
    }
    return hexEncoded(Data(first, bigEndian.end()));
}

std::string sealedSignEip7702Authorization(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                                           const Data& chainId, const std::string& contractAddress, const Data& nonce) {
    if (!Ethereum::Address::isValid(contractAddress)) {
        return {};
    }
    const Rust::TWDataWrapper chainIdRust = chainId;
    const Rust::TWStringWrapper contractAddressRust = contractAddress;
    const Rust::TWDataWrapper nonceRust = nonce;
    const Rust::TWDataWrapper hash = Rust::tw_eip7702_get_authorization_hash(chainIdRust.get(), contractAddressRust.get(), nonceRust.get());
    if (!hash.ptr) {
        return {};
    }
    auto signature = signEvmDigest(storedKey, coin, address, password, hash.toDataOrDefault());
    if (signature.empty()) {
        return {};
    }
    const auto yParity = static_cast<byte>(signature[64] - 27);
    // Same JSON as `TWEip7702SignAuthorization` (Rust `SignedAuthorization`, field order preserved).
    return "{\"chainId\":\"" + compactHex(chainId) +
           "\",\"address\":\"" + Ethereum::Address(contractAddress).string() +
           "\",\"nonce\":\"" + compactHex(nonce) +
           "\",\"yParity\":\"" + hexEncoded(Data{yParity}) +
           "\",\"r\":\"" + compactHex(Data(signature.begin(), signature.begin() + 32)) +
           "\",\"s\":\"" + compactHex(Data(signature.begin() + 32, signature.begin() + 64)) + "\"}";
}

} // namespace TW::Keystore
