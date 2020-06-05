// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFreeCashScript.h>

#include "../FreeCash/Script.h"

using namespace TW::FreeCash;

struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreate() {
    auto script = new TWFreeCashScript{};
    return script;
}

struct TWFreeCashScript *TWFreeCashScriptCreateWithData(TWData *data) {
    auto script = new TWFreeCashScript{};
    script->impl.bytes.resize(TWDataSize(data));
    TWDataCopyBytes(data, 0, TWDataSize(data), script->impl.bytes.data());
    return script;
}

struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreateWithBytes(uint8_t *_Nonnull bytes, size_t size) {
    auto script = new TWFreeCashScript{};
    std::copy(bytes, bytes + size, std::back_inserter(script->impl.bytes));
    return script;
}

struct TWFreeCashScript *TWFreeCashScriptCreateCopy(const struct TWFreeCashScript *script) {
    auto newScript = new TWFreeCashScript{};
    newScript->impl.bytes = script->impl.bytes;
    return newScript;
}

void TWFreeCashScriptDelete(struct TWFreeCashScript *script) {
    delete script;
}

size_t TWFreeCashScriptSize(const struct TWFreeCashScript *script) {
    return script->impl.bytes.size();
}

TWData *TWFreeCashScriptData(const struct TWFreeCashScript *script) {
    return TWDataCreateWithBytes(&script->impl.bytes[0], script->impl.bytes.size());
}

TWData *TWFreeCashScriptScriptHash(const struct TWFreeCashScript *_Nonnull script) {
    auto result = script->impl.hash();
    return TWDataCreateWithBytes(result.data(), result.size());
}

bool TWFreeCashScriptIsPayToScriptHash(const struct TWFreeCashScript *script) {
    return script->impl.isPayToScriptHash();
}

bool TWFreeCashScriptIsPayToWitnessScriptHash(const struct TWFreeCashScript *script) {
    return script->impl.isPayToWitnessScriptHash();
}

bool TWFreeCashScriptIsWitnessProgram(const struct TWFreeCashScript *script) {
    return script->impl.isWitnessProgram();
}

bool TWFreeCashScriptEqual(const struct TWFreeCashScript *_Nonnull lhs, const struct TWFreeCashScript *_Nonnull rhs) {
    return lhs->impl.bytes == rhs->impl.bytes;
}

TWData *TWFreeCashScriptMatchPayToPubkey(const struct TWFreeCashScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToPubkey(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *TWFreeCashScriptMatchPayToPubkeyHash(const struct TWFreeCashScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToPubkeyHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWFreeCashScriptMatchPayToScriptHash(const struct TWFreeCashScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToScriptHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWFreeCashScriptMatchPayToWitnessPublicKeyHash(const struct TWFreeCashScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToWitnessPublicKeyHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *_Nullable TWFreeCashScriptMatchPayToWitnessScriptHash(const struct TWFreeCashScript *script) {
    std::vector<uint8_t> data;
    if (script->impl.matchPayToWitnessScriptHash(data)) {
        return TWDataCreateWithBytes(data.data(), data.size());
    }
    return nullptr;
}

TWData *TWFreeCashScriptEncode(const struct TWFreeCashScript *script) {
    auto result = std::vector<uint8_t>{};
    script->impl.encode(result);
    return TWDataCreateWithBytes(result.data(), result.size());
}

struct TWFreeCashScript *TWFreeCashScriptBuildPayToPublicKeyHash(TWData *hash) {
    auto v = reinterpret_cast<const std::vector<uint8_t>*>(hash);
    auto script = Script::buildPayToPublicKeyHash(*v);
    return new TWFreeCashScript{ .impl = script };
}

struct TWFreeCashScript *TWFreeCashScriptBuildPayToScriptHash(TWData *scriptHash) {
    auto v = reinterpret_cast<const std::vector<uint8_t>*>(scriptHash);
    auto script = Script::buildPayToScriptHash(*v);
    return new TWFreeCashScript{ .impl = script };
}

struct TWFreeCashScript *TWFreeCashScriptBuildPayToWitnessPubkeyHash(TWData *hash) {
    auto v = reinterpret_cast<const std::vector<uint8_t>*>(hash);
    auto script = Script::buildPayToWitnessPubkeyHash(*v);
    return new TWFreeCashScript{ .impl = script };
}

struct TWFreeCashScript *TWFreeCashScriptBuildPayToWitnessScriptHash(TWData *scriptHash) {
    auto v = reinterpret_cast<const std::vector<uint8_t>*>(scriptHash);
    auto script = Script::buildPayToWitnessScriptHash(*v);
    return new TWFreeCashScript{ .impl = script };
}

struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildForAddress(TWString *_Nonnull address, enum TWCoinType coin) {
    auto s = reinterpret_cast<const std::string*>(address);
    auto script = Script::buildForAddress(*s, coin);
    return new TWFreeCashScript{ .impl = script };
}
