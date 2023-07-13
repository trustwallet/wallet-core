// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinScript.h>
#include "../Bitcoin/Script.h"
#include "../Bitcoin/SigHashType.h"
#include "Data.h"

#include <iterator>

struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreate() {
    auto* script = new TWBitcoinScript{};
    return script;
}

struct TWBitcoinScript *TWBitcoinScriptCreateWithData(TWData *data) {
    auto* script = new TWBitcoinScript{};
    script->impl.bytes.resize(TWDataSize(data));
    TWDataCopyBytes(data, 0, TWDataSize(data), script->impl.bytes.data());
    return script;
}

struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreateWithBytes(uint8_t *_Nonnull bytes, size_t size) {
    auto* script = new TWBitcoinScript{};
    std::copy(bytes, bytes + size, std::back_inserter(script->impl.bytes));
    return script;
}

struct TWBitcoinScript *TWBitcoinScriptCreateCopy(const struct TWBitcoinScript *script) {
    auto* newScript = new TWBitcoinScript{};
    newScript->impl.bytes = script->impl.bytes;
    return newScript;
}

void TWBitcoinScriptDelete(struct TWBitcoinScript *script) {
    delete script;
}

size_t TWBitcoinScriptSize(const struct TWBitcoinScript *script) {
    return script->impl.bytes.size();
}

TWData *TWBitcoinScriptData(const struct TWBitcoinScript *script) {
    return TWDataCreateWithBytes(&script->impl.bytes[0], script->impl.bytes.size());
}

TWData *TWBitcoinScriptScriptHash(const struct TWBitcoinScript *_Nonnull script) {
    auto result = script->impl.hash();
    return TWDataCreateWithBytes(result.data(), result.size());
}

bool TWBitcoinScriptIsPayToScriptHash(const struct TWBitcoinScript *script) {
    return script->impl.isPayToScriptHash();
}

bool TWBitcoinScriptIsPayToWitnessScriptHash(const struct TWBitcoinScript *script) {
    return script->impl.isPayToWitnessScriptHash();
}

bool TWBitcoinScriptIsPayToWitnessPublicKeyHash(const struct TWBitcoinScript *script) {
    return script->impl.isPayToWitnessPublicKeyHash();
}

bool TWBitcoinScriptIsWitnessProgram(const struct TWBitcoinScript *script) {
    return script->impl.isWitnessProgram();
}

bool TWBitcoinScriptEqual(const struct TWBitcoinScript *_Nonnull lhs, const struct TWBitcoinScript *_Nonnull rhs) {
    return lhs->impl.bytes == rhs->impl.bytes;
}

TWData *TWBitcoinScriptMatchPayToPubkey(const struct TWBitcoinScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToPublicKey(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *TWBitcoinScriptMatchPayToPubkeyHash(const struct TWBitcoinScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToPublicKeyHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWBitcoinScriptMatchPayToScriptHash(const struct TWBitcoinScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToScriptHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWBitcoinScriptMatchPayToWitnessPublicKeyHash(const struct TWBitcoinScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToWitnessPublicKeyHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWBitcoinScriptMatchPayToWitnessScriptHash(const struct TWBitcoinScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToWitnessScriptHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *TWBitcoinScriptEncode(const struct TWBitcoinScript *script) {
    auto result = std::vector<uint8_t>{};
    script->impl.encode(result);
    return TWDataCreateWithBytes(result.data(), result.size());
}

struct TWBitcoinScript *TWBitcoinScriptBuildPayToPublicKey(TWData *pubkey) {
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(pubkey);
    auto script = TW::Bitcoin::Script::buildPayToPublicKey(*v);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *TWBitcoinScriptBuildPayToPublicKeyHash(TWData *hash) {
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(hash);
    auto script = TW::Bitcoin::Script::buildPayToPublicKeyHash(*v);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *TWBitcoinScriptBuildPayToScriptHash(TWData *scriptHash) {
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(scriptHash);
    auto script = TW::Bitcoin::Script::buildPayToScriptHash(*v);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *TWBitcoinScriptBuildPayToWitnessPubkeyHash(TWData *hash) {
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(hash);
    auto script = TW::Bitcoin::Script::buildPayToWitnessPublicKeyHash(*v);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *TWBitcoinScriptBuildPayToWitnessScriptHash(TWData *scriptHash) {
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(scriptHash);
    auto script = TW::Bitcoin::Script::buildPayToWitnessScriptHash(*v);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *_Nonnull TWBitcoinScriptLockScriptForAddress(TWString *_Nonnull address, enum TWCoinType coin) {
    auto* s = reinterpret_cast<const std::string*>(address);
    auto script = TW::Bitcoin::Script::lockScriptForAddress(*s, coin);
    return new TWBitcoinScript{ .impl = script };
}

struct TWBitcoinScript *_Nonnull TWBitcoinScriptLockScriptForAddressReplay(TWString *_Nonnull address, enum TWCoinType coin, TWData *blockHash, int64_t blockHeight) {
    auto* s = reinterpret_cast<const std::string*>(address);
    auto* v = reinterpret_cast<const std::vector<uint8_t>*>(blockHash);
    auto script = TW::Bitcoin::Script::lockScriptForAddress(*s, coin, *v, blockHeight);
    return new TWBitcoinScript{ .impl = script };
}

uint32_t TWBitcoinScriptHashTypeForCoin(enum TWCoinType coinType) {
    return TW::Bitcoin::hashTypeForCoin(coinType);
}

TWData *_Nullable TWBitcoinScriptBuildBRC20InscribeTransfer(TWString* ticker, TWString* amount, TWData* pubkey) {
    auto* brcTicker = reinterpret_cast<const std::string*>(ticker);
    auto* brcAmount = reinterpret_cast<const std::string*>(amount);
    auto* brcPubkey = reinterpret_cast<const TW::Data*>(pubkey);
    auto script = TW::Bitcoin::Script::buildBRC20InscribeTransfer(*brcTicker, std::stoull(*brcAmount), *brcPubkey);
    auto serialized = TW::data(script.SerializeAsString());
    return TWDataCreateWithBytes(serialized.data(), serialized.size());
}

TWData *_Nullable TWBitcoinScriptBuildOrdinalNftInscription(TWString* mimeType, TWData* payload, TWData* pubkey) {
    auto* ordMimeType = reinterpret_cast<const std::string*>(mimeType);
    auto* ordPayload = reinterpret_cast<const TW::Data*>(payload);
    auto* ordPubkey = reinterpret_cast<const TW::Data*>(pubkey);
    auto script = TW::Bitcoin::Script::buildOrdinalNftInscription(*ordMimeType, *ordPayload, *ordPubkey);
    auto serialized = TW::data(script.SerializeAsString());
    return TWDataCreateWithBytes(serialized.data(), serialized.size());
}
