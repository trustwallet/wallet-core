// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.


#include "Cbor.h"
#include "PublicKey.h"

#include <utility>
#include <algorithm>
#include <optional>

namespace TW::WebAuthn {

// 32 + 1 flags + 4 counter
static const std::size_t gAuthDataMinSize = 37;
// 16 aaguid + 2 credIDLen
static const std::size_t gAuthCredentialDataMinSize = 18;

// https://www.w3.org/TR/webauthn-2/#authenticator-data
struct AuthData {
    Data rpIdHash;
    std::array<std::uint8_t, 1> flagsBuf;
    struct {
        bool up;
        bool uv;
        bool at;
        bool ed;
        std::uint8_t flagsInt;
    } flags;
    std::uint32_t counter;
    Data counterBuf;
    Data aaguid;
    Data credID;
    Data COSEPublicKey;
};

std::optional<AuthData> parseAuthData(const Data& buffer) {
    if (buffer.size() < gAuthDataMinSize) {
        return std::nullopt;
    }

    AuthData authData;

    authData.rpIdHash = subData(buffer, 0, 32);

    auto it = buffer.begin() + 32;
    authData.flagsBuf = { *it };
    ++it;
    std::uint8_t flagsInt = authData.flagsBuf[0];
    authData.flags.up = !!(flagsInt & 0x01);
    authData.flags.uv = !!(flagsInt & 0x04);
    authData.flags.at = !!(flagsInt & 0x40);
    authData.flags.ed = !!(flagsInt & 0x80);
    authData.flags.flagsInt = flagsInt;

    authData.counterBuf = Data(it, it + 4);
    authData.counter = static_cast<std::uint32_t>((authData.counterBuf[0] << 24) |
                                                  (authData.counterBuf[1] << 16) |
                                                  (authData.counterBuf[2] << 8) |
                                                  authData.counterBuf[3]);
    it += 4;

    if (authData.flags.at) {
        if (static_cast<size_t>(buffer.end() - it) < gAuthCredentialDataMinSize) {
            return std::nullopt;
        }

        authData.aaguid = Data(it, it + 16);
        it += 16;

        std::array<std::uint8_t, 2> credIDLenBuf = { *(it), *(it + 1) };
        std::uint16_t credIDLen = static_cast<std::uint16_t>((credIDLenBuf[0] << 8) |
                                                             credIDLenBuf[1]);
        it += 2;

        if (static_cast<size_t>(buffer.end() - it) < static_cast<size_t>(credIDLen)) {
            return std::nullopt;
        }

        authData.credID = Data(it, it + credIDLen);
        it += credIDLen;

        authData.COSEPublicKey = Data(it, buffer.end());
    }

    return authData;
}

auto findIntKey = [](const auto& map, const auto& key) {
    return std::find_if(map.begin(), map.end(), [&](const auto& p) {
        return p.first.dumpToString() == key;
    });
};

auto findStringKey = [](const auto& map, const auto& key) {
    return std::find_if(map.begin(), map.end(), [&](const auto& p) {
        return p.first.getString() == key;
    });
};

std::optional<PublicKey> getPublicKey(const Data& attestationObject) {
    try {
        const auto attestationObjectElements = TW::Cbor::Decode(attestationObject).getMapElements();
        const auto authDataIter = findStringKey(attestationObjectElements, "authData");
        if (authDataIter == attestationObjectElements.end()) {
            return std::nullopt;
        }

        const Data authData = authDataIter->second.getBytes();
        if (authData.empty()) {
            return std::nullopt;
        }

        const auto authDataParsed = parseAuthData(authData);
        if (!authDataParsed.has_value()) {
            return std::nullopt;
        }
        const auto COSEPublicKey = TW::Cbor::Decode(authDataParsed->COSEPublicKey).getMapElements();

        if (COSEPublicKey.empty()) {
            return std::nullopt;
        }

        // https://www.w3.org/TR/webauthn-2/#sctn-encoded-credPubKey-examples
        const std::string xKey = "-2";
        const std::string yKey = "-3";

        const auto x = findIntKey(COSEPublicKey, xKey);
        const auto y = findIntKey(COSEPublicKey, yKey);
        if (x == COSEPublicKey.end() || y == COSEPublicKey.end()) {
            return std::nullopt;
        }

        Data publicKey;
        append(publicKey, 0x04);
        append(publicKey, x->second.getBytes());
        append(publicKey, y->second.getBytes());

        return PublicKey(publicKey, TWPublicKeyTypeNIST256p1Extended);
    } catch (...) {
        return std::nullopt;
    }
}

Data reconstructSignedMessage(const Data& authenticatorData, const Data& clientDataJSON) {
    const auto& clientHash = Hash::sha256(clientDataJSON);

    Data message;
    append(message, authenticatorData);
    append(message, clientHash);

    return Hash::sha256(message);
}

} // namespace TW::Webauthn
