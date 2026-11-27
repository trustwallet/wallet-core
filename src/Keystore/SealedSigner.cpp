// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SealedSigner.h"

#include "Coin.h"
#include "Ethereum/Address.h"
#include "Ethereum/MessageSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Keystore/EncryptionParameters.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Common.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Tron.pb.h"
#include "rust/Wrapper.h"

#include <TrezorCrypto/memzero.h>

#include <algorithm>
#include <functional>
#include <optional>

namespace TW::Keystore {

using Result = SealedSignResult;

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

// Runs a public sealed function body so that nothing escapes as an exception: an exception that
// no step mapped to its own error is a bug, reported as `internal` (its text is not kept).
static Result guarded(const std::function<Result()>& body) {
    try {
        return body();
    } catch (...) {
        return Result::failure(TWStoredKeySignErrorInternal, "unexpected exception");
    }
}

// Decrypts the stored key with `password` and runs `use` on the plaintext key material. Maps
// decryption failures to their errors: MAC mismatch → wrong password; unsupported KDF / cipher →
// unsupported key format; anything else wrong with the payload (corrupt cipher data, a decrypted
// mnemonic or private key that is not valid) → decryption failed.
static Result decryptKey(const StoredKey& storedKey, TWCoinType coin, const Account& account, const Data& password, std::optional<PrivateKey>& key) {
    try {
        if (storedKey.type == StoredKeyType::privateKey) {
            Data secret = storedKey.payload.decrypt(password);
            try {
                key.emplace(secret, TWCoinTypeCurve(coin));
            } catch (...) {
                wipe(secret);
                throw;
            }
            wipe(secret);
        } else {
            const auto wallet = storedKey.wallet(password);
            key.emplace(wallet.getKey(coin, account.derivationPath));
        }
    } catch (const DecryptionError& error) {
        switch (error) {
        case DecryptionError::invalidPassword:
            return Result::failure(TWStoredKeySignErrorWrongPassword, "wrong password");
        case DecryptionError::unsupportedKDF:
        case DecryptionError::unsupportedCipher:
            return Result::failure(TWStoredKeySignErrorUnsupportedKeyFormat, "unsupported keystore KDF or cipher");
        default:
            return Result::failure(TWStoredKeySignErrorDecryptionFailed, "keystore payload is invalid");
        }
    } catch (const std::exception&) {
        // The MAC check passed (or the payload never got that far) but the plaintext is not a valid
        // mnemonic or private key.
        return Result::failure(TWStoredKeySignErrorDecryptionFailed, "keystore payload is invalid");
    }
    return Result::success({});
}

// Derives the key of the account identified by (`coin`, `address`) into `key`. Never mutates the
// stored key (no accounts are added or updated).
//
// The account's stored derivation path is only trusted after the key derived from it is proven to
// produce `address` — so a custom path or a non-default account signs with its own key, and a
// tampered or stale account record fails closed. The account lookup runs before the (expensive,
// scrypt-based) decryption, so a call that cannot succeed never runs the KDF.
static Result deriveSigningKey(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, std::optional<PrivateKey>& key) {
    std::optional<Account> account;
    for (const auto& candidate : storedKey.getAccounts(coin)) {
        if (candidate.address != address) {
            continue;
        }
        if (account.has_value() && account->derivationPath != candidate.derivationPath) {
            return Result::failure(TWStoredKeySignErrorAccountMismatch, "address is stored under several derivation paths");
        }
        account = candidate;
    }
    if (!account.has_value()) {
        return Result::failure(TWStoredKeySignErrorAccountNotFound, "no account for coin and address");
    }

    if (auto decrypted = decryptKey(storedKey, coin, *account, password, key); !decrypted.ok()) {
        return decrypted;
    }

    if (TW::deriveAddress(coin, *key, account->derivation) != address) {
        key.reset();
        return Result::failure(TWStoredKeySignErrorAccountMismatch, "derived address does not match the account");
    }
    return Result::success({});
}

// The error a chain signer reported in its own SigningOutput. A BitcoinV2 envelope reports it in
// `signing_result_v2`.
template <typename Output>
static Common::Proto::SigningError outputError(const Output& out) {
    return out.error();
}

static Common::Proto::SigningError outputError(const Bitcoin::Proto::SigningOutput& out) {
    return out.error() != Common::Proto::OK ? out.error() : out.signing_result_v2().error();
}

// Serializes the key-injected input straight into a Data buffer (no intermediate std::string),
// signs it through the normal internal dispatcher, and wipes the serialized copy.
//
// A signer that ran but reported an error in its SigningOutput (e.g. a Tron `raw_json` whose txID does
// not match) produced no signature. That is `SigningFailed`, not a successful call, so a caller that
// checks only the result code can never mistake the output for a signed transaction.
template <typename Output, typename Input>
static Result serializeAndSign(TWCoinType coin, const Input& in) {
    Data injected(in.ByteSizeLong());
    in.SerializeToArray(injected.data(), static_cast<int>(injected.size()));
    Data output;
    try {
        anyCoinSign(coin, injected, output);
    } catch (...) {
        wipe(injected);
        return Result::failure(TWStoredKeySignErrorSigningFailed, "signer threw");
    }
    wipe(injected);
    if (output.empty()) {
        return Result::failure(TWStoredKeySignErrorSigningFailed, "signer returned no output");
    }
    Output parsed;
    if (!parsed.ParseFromArray(output.data(), static_cast<int>(output.size()))) {
        return Result::failure(TWStoredKeySignErrorSigningFailed, "signer output does not parse");
    }
    if (outputError(parsed) != Common::Proto::OK) {
        return Result::failure(TWStoredKeySignErrorSigningFailed, "signer reported an error in its output");
    }
    return Result::success(std::move(output));
}

template <typename Input>
static bool parseInput(Input& in, const Data& signingInput) {
    return in.ParseFromArray(signingInput.data(), static_cast<int>(signingInput.size()));
}

static Result invalidInput(const char* message) {
    return Result::failure(TWStoredKeySignErrorInvalidInput, message);
}

// EVM chains share the Ethereum SigningInput, which carries a singular `private_key` field. The key
// comes from the account of `coin` itself (e.g. Smart Chain, or Ethereum Classic on its own path),
// but signing always runs under Ethereum, exactly like the app does today (the chain is selected by
// the input's `chain_id`, not by the coin).
static Result signEthereum(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Ethereum::Proto::SigningInput in;
    if (!parseInput(in, signingInput)) {
        return invalidInput("signing input does not parse");
    }
    if (!in.private_key().empty()) {
        return invalidInput("private_key must be empty");
    }
    std::optional<PrivateKey> key;
    if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
        return derived;
    }
    in.set_private_key(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign<Ethereum::Proto::SigningOutput>(TWCoinTypeEthereum, in);
    wipe(*in.mutable_private_key()); // clear_* alone does not zero memory
    return output;
}

// UTXO chains: only the BitcoinV2 envelope the app signs with is accepted — a `Bitcoin.SigningInput`
// whose `signing_v2` carries the transaction and whose `coin_type` names `coin`. The single account
// key goes into `signing_v2.private_keys` (the app's single-address model: every input and the change
// output use the account's one address, so one key signs the whole transaction).
static Result signUtxo(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Bitcoin::Proto::SigningInput in;
    if (!parseInput(in, signingInput)) {
        return invalidInput("signing input does not parse");
    }
    if (!in.has_signing_v2()) {
        return invalidInput("signing_v2 must be set");
    }
    if (in.coin_type() != static_cast<uint32_t>(coin)) {
        return invalidInput("coin_type does not match coin");
    }
    if (in.private_key_size() != 0 || in.signing_v2().private_keys_size() != 0) {
        return invalidInput("private_key and signing_v2.private_keys must be empty");
    }
    std::optional<PrivateKey> key;
    if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
        return derived;
    }
    auto* v2 = in.mutable_signing_v2();
    v2->add_private_keys(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign<Bitcoin::Proto::SigningOutput>(coin, in);
    wipe(*v2->mutable_private_keys(0));
    return output;
}

// Tron: `Tron.SigningInput` with either `transaction` or `raw_json` (a dApp transaction) and an empty
// `private_key`. The normal Tron signer keeps its `raw_json` checks: the transaction is rebuilt from
// `raw_data`, and its SHA-256 must equal `txID` (and `sha256(raw_data_hex)` must equal `txID` when
// `raw_data_hex` is present), so the key only ever signs the transaction the JSON describes.
static Result signTron(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Tron::Proto::SigningInput in;
    if (!parseInput(in, signingInput)) {
        return invalidInput("signing input does not parse");
    }
    if (!in.private_key().empty()) {
        return invalidInput("private_key must be empty");
    }
    std::optional<PrivateKey> key;
    if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
        return derived;
    }
    in.set_private_key(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign<Tron::Proto::SigningOutput>(TWCoinTypeTron, in);
    wipe(*in.mutable_private_key());
    return output;
}

// Solana: `Solana.SigningInput` for a transaction the library builds, or with `raw_message` (a dApp
// transaction). Every key field must be empty — `private_key`, `fee_payer_private_key` and
// `create_nonce_account.nonce_account_private_key` — so the account key is the only key that signs;
// signatures of other signers may still come in `raw_message.signatures`.
static Result signSolana(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    Solana::Proto::SigningInput in;
    if (!parseInput(in, signingInput)) {
        return invalidInput("signing input does not parse");
    }
    if (!in.private_key().empty() || !in.fee_payer_private_key().empty() ||
        !in.create_nonce_account().nonce_account_private_key().empty()) {
        return invalidInput("private_key, fee_payer_private_key and nonce_account_private_key must be empty");
    }
    std::optional<PrivateKey> key;
    if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
        return derived;
    }
    in.set_private_key(key->bytes.data(), key->bytes.size());
    auto output = serializeAndSign<Solana::Proto::SigningOutput>(TWCoinTypeSolana, in);
    wipe(*in.mutable_private_key());
    return output;
}

Result sealedSign(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput) {
    // Coins supported by the sealed path so far. Each must have a per-coin byte-identical test
    // proving flag-ON output == flag-OFF output before it is added here.
    //
    // The PrivateKey and HDWallet destructors zero their own buffers; the serialized input and the
    // proto's copy of the key are wiped by the per-family signers above.
    // (Copies made inside the per-coin signer itself are a later hardening step.)
    return guarded([&]() -> Result {
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
        case TWCoinTypeTron:
            return signTron(storedKey, coin, address, password, signingInput);
        case TWCoinTypeSolana:
            return signSolana(storedKey, coin, address, password, signingInput);
        default:
            return Result::failure(TWStoredKeySignErrorUnsupportedCoin, "coin is not supported by sealed signing");
        }
    });
}

// ---------------------------------------------------------------------------------------------
// Message signing
//
// Each function checks the coin, then computes the digest itself from structured input (never signs
// a caller-supplied raw hash), so a sealed key can only produce the kind of signature the function
// is named for. Digests that wallet-core computes in Rust (Biz, EIP-7702) are obtained without the
// key; the key only ever signs inside C++, where `PrivateKey` wipes itself.
// ---------------------------------------------------------------------------------------------

static bool isEvm(TWCoinType coin) {
    return TWCoinTypeBlockchain(coin) == TWBlockchainEthereum;
}

static Result unsupportedCoin() {
    return Result::failure(TWStoredKeySignErrorUnsupportedCoin, "coin is not supported by this function");
}

// Signs a 32-byte digest with the secp256k1 key of (`coin`, `address`) and returns r || s || v with
// v = 27 + yParity. Callers check the coin and build the digest first.
static Result signSecp256k1Digest(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& digest) {
    if (digest.size() != 32) {
        return invalidInput("digest could not be computed from the input");
    }
    if (TWCoinTypeCurve(coin) != TWCurveSECP256k1) {
        return unsupportedCoin();
    }
    std::optional<PrivateKey> key;
    if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
        return derived;
    }
    auto signature = key->sign(digest, TWCurveSECP256k1);
    if (signature.size() != 65 || signature[64] > 1) {
        return Result::failure(TWStoredKeySignErrorSigningFailed, "signature has the wrong length");
    }
    signature[64] += 27;
    return Result::success(std::move(signature));
}

Result sealedSignEthereumPersonalMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message) {
    return guarded([&]() -> Result {
        if (!isEvm(coin)) {
            return unsupportedCoin();
        }
        const std::string prefix = "\x19" "Ethereum Signed Message:\n" + std::to_string(message.size());
        Data prefixed(prefix.begin(), prefix.end());
        append(prefixed, message);
        return signSecp256k1Digest(storedKey, coin, address, password, Hash::keccak256(prefixed));
    });
}

Result sealedSignEthereumTypedData(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const std::string& typedDataJson) {
    return guarded([&]() -> Result {
        if (!isEvm(coin)) {
            return unsupportedCoin();
        }
        Data digest;
        try {
            digest = Ethereum::MessageSigner::typedDataPreImageHash(typedDataJson);
        } catch (...) {
            return invalidInput("typed data does not parse");
        }
        if (digest.size() != 32) {
            return invalidInput("typed data does not parse");
        }
        return signSecp256k1Digest(storedKey, coin, address, password, digest);
    });
}

Result sealedSignBizUserOpHash(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                               const Data& chainId, const std::string& codeAddress, const std::string& codeName, const std::string& codeVersion,
                               const std::string& typeHash, const std::string& domainSeparatorHash, const std::string& userOpHash) {
    return guarded([&]() -> Result {
        if (!isEvm(coin)) {
            return unsupportedCoin();
        }
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
            return invalidInput("Biz hash could not be computed from the input");
        }
        return signSecp256k1Digest(storedKey, coin, address, password, encoded.toDataOrDefault());
    });
}

// Minimal big-endian hex with a "0x" prefix and at least one byte, as the Rust serializer writes U256.
static std::string compactHex(const Data& bigEndian) {
    auto first = std::find_if(bigEndian.begin(), bigEndian.end(), [](byte b) { return b != 0; });
    if (first == bigEndian.end()) {
        return "0x00";
    }
    return hexEncoded(Data(first, bigEndian.end()));
}

Result sealedSignEip7702Authorization(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                                      const Data& chainId, const std::string& contractAddress, const Data& nonce) {
    return guarded([&]() -> Result {
        if (!isEvm(coin)) {
            return unsupportedCoin();
        }
        if (!Ethereum::Address::isValid(contractAddress)) {
            return invalidInput("contract address is not valid");
        }
        const Rust::TWDataWrapper chainIdRust = chainId;
        const Rust::TWStringWrapper contractAddressRust = contractAddress;
        const Rust::TWDataWrapper nonceRust = nonce;
        const Rust::TWDataWrapper hash = Rust::tw_eip7702_get_authorization_hash(chainIdRust.get(), contractAddressRust.get(), nonceRust.get());
        if (!hash.ptr) {
            return invalidInput("authorization hash could not be computed from the input");
        }
        auto signedDigest = signSecp256k1Digest(storedKey, coin, address, password, hash.toDataOrDefault());
        if (!signedDigest.ok()) {
            return signedDigest;
        }
        const auto& signature = signedDigest.data;
        const auto yParity = static_cast<byte>(signature[64] - 27);
        // Same JSON as `TWEip7702SignAuthorization` (Rust `SignedAuthorization`, field order preserved).
        const std::string json = "{\"chainId\":\"" + compactHex(chainId) +
                                 "\",\"address\":\"" + Ethereum::Address(contractAddress).string() +
                                 "\",\"nonce\":\"" + compactHex(nonce) +
                                 "\",\"yParity\":\"" + hexEncoded(Data{yParity}) +
                                 "\",\"r\":\"" + compactHex(Data(signature.begin(), signature.begin() + 32)) +
                                 "\",\"s\":\"" + compactHex(Data(signature.begin() + 32, signature.begin() + 64)) + "\"}";
        return Result::success(Data(json.begin(), json.end()));
    });
}

// ---------------------------------------------------------------------------------------------
// Tron message signing
// ---------------------------------------------------------------------------------------------

// Same as `Tron::MessageSigner::signMessage` (TronWeb signMessageV2):
// keccak256("\x19TRON Signed Message:\n" + len + message), v = 27/28. Only Tron accounts sign.
Result sealedSignTronMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message) {
    return guarded([&]() -> Result {
        if (coin != TWCoinTypeTron) {
            return unsupportedCoin();
        }
        const std::string prefix = "\x19" "TRON Signed Message:\n" + std::to_string(message.size());
        Data prefixed(prefix.begin(), prefix.end());
        append(prefixed, message);
        return signSecp256k1Digest(storedKey, coin, address, password, Hash::keccak256(prefixed));
    });
}

// ---------------------------------------------------------------------------------------------
// Solana message signing
// ---------------------------------------------------------------------------------------------

// Domain separation for Solana message signing — a line-for-line port of the App's rule
// (SC-140951: mobile-monorepo `solana-transaction-message.ts`, from Kotlin `SolanaTransactionMessage.kt`),
// so the sealed path refuses exactly the messages the App refuses.
//
// A validator verifies a transaction signature as ed25519 over the serialized transaction message,
// and message bytes are signed raw — so a "message" that is exactly a transaction message would yield
// a broadcastable transaction signature. Such bytes are recognised as a complete legacy or v0 message
// that satisfies the header / index rules a validator enforces and uses up every byte. Padding or
// truncating the bytes defeats the attack, so those are not transaction messages; off-chain messages
// (0xff prefix) and other versions never match. Pure byte parsing; no key involved.
namespace {

class SolanaMessageReader {
public:
    explicit SolanaMessageReader(const Data& bytes) : bytes(bytes) {}

    bool isExhausted() const { return offset == bytes.size(); }

    // Each read returns false when out of bytes (the TS reader throws; any throw means "not a transaction").
    bool u8(uint32_t& out) {
        if (offset >= bytes.size()) {
            return false;
        }
        out = bytes[offset++];
        return true;
    }

    bool skip(uint64_t count) {
        if (count > bytes.size() - offset) {
            return false;
        }
        offset += static_cast<size_t>(count);
        return true;
    }

    // Solana `short_vec` length: 7 bits per byte, at most 3 bytes.
    bool compactU16(uint32_t& out) {
        uint32_t value = 0;
        for (int i = 0; i < 3; i++) {
            uint32_t byte = 0;
            if (!u8(byte)) {
                return false;
            }
            value |= (byte & 0x7f) << (7 * i);
            if ((byte & 0x80) == 0) {
                out = value;
                return true;
            }
        }
        return false; // compact-u16 too long
    }

private:
    const Data& bytes;
    size_t offset = 0;
};

constexpr uint32_t gSolanaVersionPrefix = 0x80;
constexpr uint64_t gSolanaPublicKeyLength = 32;
constexpr uint64_t gSolanaBlockhashLength = 32;

bool solanaMessageParses(SolanaMessageReader& reader) {
    uint32_t first = 0;
    if (!reader.u8(first)) {
        return false;
    }
    const bool versioned = (first & gSolanaVersionPrefix) != 0;
    if (versioned) {
        // Only v0 exists; 0xff (the off-chain message prefix) is not a version.
        if (first != gSolanaVersionPrefix || !reader.u8(first)) {
            return false;
        }
    }
    const uint32_t numRequiredSignatures = first;
    uint32_t numReadonlySigned = 0;
    uint32_t numReadonlyUnsigned = 0;
    uint32_t numStaticKeys = 0;
    if (!reader.u8(numReadonlySigned) || !reader.u8(numReadonlyUnsigned) || !reader.compactU16(numStaticKeys) ||
        !reader.skip(numStaticKeys * gSolanaPublicKeyLength) || !reader.skip(gSolanaBlockhashLength)) {
        return false;
    }

    int64_t maxProgramIndex = -1;
    int64_t maxAccountIndex = -1;
    uint32_t numInstructions = 0;
    if (!reader.compactU16(numInstructions)) {
        return false;
    }
    for (uint32_t i = numInstructions; i > 0; i--) {
        uint32_t programIndex = 0;
        uint32_t numAccounts = 0;
        if (!reader.u8(programIndex) || !reader.compactU16(numAccounts)) {
            return false;
        }
        maxProgramIndex = std::max<int64_t>(maxProgramIndex, programIndex);
        for (uint32_t j = numAccounts; j > 0; j--) {
            uint32_t accountIndex = 0;
            if (!reader.u8(accountIndex)) {
                return false;
            }
            maxAccountIndex = std::max<int64_t>(maxAccountIndex, accountIndex);
        }
        uint32_t dataLength = 0;
        if (!reader.compactU16(dataLength) || !reader.skip(dataLength)) {
            return false;
        }
    }

    uint64_t numLookupKeys = 0;
    if (versioned) {
        uint32_t numLookups = 0;
        if (!reader.compactU16(numLookups)) {
            return false;
        }
        for (uint32_t i = numLookups; i > 0; i--) {
            if (!reader.skip(gSolanaPublicKeyLength)) {
                return false;
            }
            // writable, then readonly indexes
            for (int k = 0; k < 2; k++) {
                uint32_t count = 0;
                if (!reader.compactU16(count) || !reader.skip(count)) {
                    return false;
                }
                numLookupKeys += count;
            }
        }
    }

    const uint64_t numKeys = numStaticKeys + numLookupKeys;
    return reader.isExhausted() &&
           numReadonlySigned < numRequiredSignatures &&
           static_cast<uint64_t>(numRequiredSignatures) + numReadonlyUnsigned <= numStaticKeys &&
           maxProgramIndex < static_cast<int64_t>(numStaticKeys) &&
           maxAccountIndex < static_cast<int64_t>(numKeys);
}

} // namespace

bool isSolanaTransactionMessage(const Data& message) {
    SolanaMessageReader reader(message);
    return solanaMessageParses(reader);
}

// ed25519 signature over the raw message bytes, as the Solana off-chain message signer does for a
// UTF-8 message (sign-message cookbook) and as the App signs dApp messages (`PrivateKey.sign`).
// Refuses bytes that are a transaction message (see above), so this function can never produce a
// transaction signature.
Result sealedSignSolanaMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message) {
    return guarded([&]() -> Result {
        if (coin != TWCoinTypeSolana) {
            return unsupportedCoin();
        }
        if (isSolanaTransactionMessage(message)) {
            return Result::failure(TWStoredKeySignErrorTransactionBytesRefused, "message is a Solana transaction message");
        }
        std::optional<PrivateKey> key;
        if (auto derived = deriveSigningKey(storedKey, coin, address, password, key); !derived.ok()) {
            return derived;
        }
        auto signature = key->sign(message, TWCurveED25519);
        if (signature.size() != 64) {
            return Result::failure(TWStoredKeySignErrorSigningFailed, "signature has the wrong length");
        }
        return Result::success(std::move(signature));
    });
}

} // namespace TW::Keystore
