// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AddressV2.h"
#include "../Base58.h"
#include "../Cbor.h"
#include "../Crc.h"

#include <array>

namespace TW::Cardano {

bool AddressV2::parseAndCheck(const std::string& addr, Data& root_out, Data& attrs_out, byte& type_out) {
    // Decode Bas58, decode payload + crc, decode root, attr
    Data base58decoded = Base58::decode(addr);
    if (base58decoded.empty()) {
        return false;
    }
    auto elems = Cbor::Decode(base58decoded).getArrayElements();
    if (elems.size() < 2) {
        return false;
    }
    auto tag = elems[0].getTagValue();
    if (tag != PayloadTag) {
        return false;
    }
    Data payload = elems[0].getTagElement().getBytes();
    uint64_t crcPresent = (uint32_t)elems[1].getValue();
    uint32_t crcComputed = TW::Crc::crc32(payload);
    if (crcPresent != crcComputed) {
        return false;
    }
    // parse payload, 3 elements
    auto payloadElems = Cbor::Decode(payload).getArrayElements();
    if (payloadElems.size() < 3) {
        return false;
    }
    root_out = payloadElems[0].getBytes();
    attrs_out = payloadElems[1].encoded(); // map, but encoded as bytes
    type_out = (byte)payloadElems[2].getValue();
    return true;
}

bool AddressV2::isValid(const std::string& string) {
    try {
        Data root;
        Data attrs;
        byte type = 0;
        if (!parseAndCheck(string, root, attrs, type)) {
            return false;
        }
        // valid
        return true;
    } catch (std::exception& ex) {
        return false;
    }
}

AddressV2::AddressV2(const std::string& string) {
    if (!parseAndCheck(string, root, attrs, type)) {
        throw std::invalid_argument("Invalid address string");
    }
    // values stored
}

AddressV2::AddressV2(const PublicKey& publicKey) {
    // input is extended pubkey, 64-byte
    if (publicKey.type != TWPublicKeyTypeED25519Cardano || publicKey.bytes.size() != PublicKey::cardanoKeySize) {
        throw std::invalid_argument("Invalid public key type");
    }
    type = 0; // public key
    root = keyHash(subData(publicKey.bytes, 0, 64));
    // address attributes: empty map for V2, for V1 encrypted derivation path
    Cbor::Encode emptyMap = Cbor::Encode::map({});
    attrs = emptyMap.encoded();
}

Data AddressV2::getCborData() const {
    // put together string representation, CBOR representation
    // inner data: pubkey, attrs, type
    auto cbor1 = Cbor::Encode::array({
        Cbor::Encode::bytes(root),
        Cbor::Encode::fromRaw(attrs),
        Cbor::Encode::uint(type),
    });
    auto payloadData = cbor1.encoded();

    // crc checksum
    auto crc = TW::Crc::crc32(payloadData);
    // second pack: tag, base, crc
    auto cbor2 = Cbor::Encode::array({
        Cbor::Encode::tag(PayloadTag, Cbor::Encode::bytes(payloadData)),
        Cbor::Encode::uint(crc),
    });
    return cbor2.encoded();
}

std::string AddressV2::string() const {
    // Base58 encode the CBOR data
    return Base58::encode(getCborData());
}

Data AddressV2::keyHash(const TW::Data& xpub) {
    if (xpub.size() != 64) {
        return {};
    }
    // hash of following Cbor-array: [0, [0, xpub], {} ]
    // 3rd entry map is empty map for V2, contains derivation path for V1
    // clang-format off
    Data cborData = Cbor::Encode::array({
        Cbor::Encode::uint(0),
        Cbor::Encode::array({Cbor::Encode::uint(0),
                                Cbor::Encode::bytes(xpub)}),
        Cbor::Encode::map({}),
    }).encoded();
    // clang-format on
    // SHA3 hash, then blake
    Data firstHash = Hash::sha3_256(cborData);
    Data blake = Hash::blake2b(firstHash, 28);
    return blake;
}

} // namespace TW::Cardano
