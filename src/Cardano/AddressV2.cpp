// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressV2.h"
#include "../Cbor.h"
#include "../Data.h"
#include "../Base58.h"
#include "../Crc.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <array>

using namespace TW;
using namespace TW::Cardano;
using namespace std;

bool AddressV2::parseAndCheck(const std::string& addr, Data& root_out, Data& attrs_out, byte& type_out) {
    // Decode Bas58, decode payload + crc, decode root, attr
    Data base58decoded = Base58::bitcoin.decode(addr);
    if (base58decoded.size() == 0) {
        throw invalid_argument("Invalid address: could not Base58 decode");
    }
    auto elems = Cbor::Decode(base58decoded).getArrayElements();
    if (elems.size() < 2) {
        throw invalid_argument("Could not parse address payload from CBOR data");
    }
    auto tag = elems[0].getTagValue();
    if (tag != PayloadTag) {
        throw invalid_argument("wrong tag value");
    }
    Data payload = elems[0].getTagElement().getBytes();
    uint64_t crcPresent = (uint32_t)elems[1].getValue();
    uint32_t crcComputed = TW::Crc::crc32(payload);
    if (crcPresent != crcComputed) {
        throw invalid_argument("CRC mismatch");
    }
    // parse payload, 3 elements
    auto payloadElems = Cbor::Decode(payload).getArrayElements();
    if (payloadElems.size() < 3) {
        throw invalid_argument("Could not parse address root and attrs from CBOR data");
    }
    root_out = payloadElems[0].getBytes();
    attrs_out = payloadElems[1].encoded(); // map, but encoded as bytes
    type_out = (TW::byte)payloadElems[2].getValue();
    return true;
}

bool AddressV2::isValid(const std::string& string) {
    try {
        Data root;
        Data attrs;
        byte type = 0;
        if (!parseAndCheck(string, root, attrs, type)) { return false; }
        // valid
        return true;
    } catch (exception& ex) {
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
    if (publicKey.type != TWPublicKeyTypeED25519Extended || publicKey.bytes.size() != PublicKey::ed25519DoubleExtendedSize) {
        throw std::invalid_argument("Invalid public key type");
    }
    type = 0; // public key
    root = keyHash(subData(publicKey.bytes, 0, 64));
    // address attributes: empty map for V2, for V1 encrypted derivation path
    Cbor::Encode emptyMap = Cbor::Encode::map({});
    attrs = emptyMap.encoded();
}

Data AddressV2::getCborData() const {
    // put together string represenatation, CBOR representation
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

string AddressV2::string() const {
    // Base58 encode the CBOR data
    return Base58::bitcoin.encode(getCborData());
}

Data AddressV2::keyHash(const TW::Data& xpub) {
    if (xpub.size() != 64) { throw invalid_argument("invalid xbub length"); }
    // hash of follwoing Cbor-array: [0, [0, xbub], {} ]
    // 3rd entry map is empty map for V2, contains derivation path for V1
    Data cborData = Cbor::Encode::array({
        Cbor::Encode::uint(0),
        Cbor::Encode::array({
            Cbor::Encode::uint(0),
            Cbor::Encode::bytes(xpub)
        }),
        Cbor::Encode::map({}),
    }).encoded();
    // SHA3 hash, then blake
    Data firstHash = Hash::sha3_256(cborData);
    Data blake = Hash::blake2b(firstHash, 28);
    return blake;
}
