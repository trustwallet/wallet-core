// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Cbor.h"
#include "../Data.h"
//#include "../PublicKey.h"
#include "../Base58.h"
#include "../Crc.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <array>
#include <iostream>

using namespace TW;
using namespace TW::Cardano;
using namespace std;

bool Address::isValid(const std::string& string) {
    try {
        Data base58decoded = Base58::bitcoin.decode(string);
        //std::cerr << base58decoded.size() << " " << (int)base58decoded[0] << " " << (int)base58decoded[0]/32 << std::endl;
        //cerr << Cbor::Decode(base58decoded).dumpToString() << endl;
        auto elems = Cbor::Decode(base58decoded).getArrayElements();
        if (elems.size() < 2) { return false; }
        auto tag = elems[0].getTagValue();
        if (tag != 24) {
            // wrong tag value
            return false;
        }
        Data payload = elems[0].getTagElement().getBytes();
        // debug
        uint64_t crcPresent = (uint32_t)elems[1].getValue();
        uint32_t crcComputed = TW::Crc::crc32(payload);
        if (crcPresent != crcComputed) {
            // CRC mismatch
            return false;
        }
        // check that payload is Cbor-encoded, array, with 3 elements
        auto payloadElems = Cbor::Decode(payload).getArrayElements();
        return true;
    } catch (exception& ex) {
        return false;
    }
}

Address::Address(const std::string& string) {
    try
    {
        if (!isValid(string)) {
            throw std::invalid_argument("Invalid address string");
        }
        Data base58decoded = Base58::bitcoin.decode(string);
        auto elems = Cbor::Decode(base58decoded).getArrayElements();
        if (elems.size() >= 2) { // checked in isValid
            auto array0 = elems[0];
            auto tag = array0.getTagValue();
            if (tag == 24) { // checked in isValid
                Data addressAsArray = array0.getTagElement().getBytes();
                // debug
                uint64_t crcPresent = (uint32_t)elems[1].getValue();
                uint32_t crcComputed = TW::Crc::crc32(addressAsArray);
                if (crcPresent == crcComputed) { // checked in isValid
                    // parse 2nd CBOR
                    auto elems2 = Cbor::Decode(addressAsArray).getArrayElements();
                    if (elems2.size() < 3) {
                        // a 3-elem array is expected
                        throw std::invalid_argument("Invalid address string, inner array too short");
                    }
                    // store values
                    root = elems2[0].getBytes();
                    attrs = elems2[1].encoded();
                    type = (TW::byte)(elems2[2].getValue());
                }
            }
        }
    } catch (exception& ex) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const PublicKey& publicKey) {
    // input is extended pubkey, 64-byte
    if (publicKey.type != TWPublicKeyTypeED25519Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    type = 0; // public key
    root = keyHash(publicKey.bytes);
    // address attributes: empty map for V2, for V1 encrypted derivation path
    Cbor::Encode emptyMap = Cbor::Encode::map({});
    attrs = emptyMap.encoded();
}

string Address::string() const {
    // put together string represenatation (CBOR encopde, Base58 encode)
    // inner data: pubkey, attrs, type
    auto cbor1 = Cbor::Encode::array({
        Cbor::Encode::bytes(root),
        Cbor::Encode(attrs),
        Cbor::Encode::uint(type),
    });
    auto payloadData = cbor1.encoded();
    
    // crc checksum 
    auto crc = TW::Crc::crc32(payloadData);
    // second pack: tag, base, crc
    auto cbor2 = Cbor::Encode::array({
        Cbor::Encode::tag(24, Cbor::Encode::bytes(payloadData)),
        Cbor::Encode::uint(crc),
    });
    auto cbor2Data = cbor2.encoded();
    return Base58::bitcoin.encode(cbor2Data);
}

Data Address::keyHash(const TW::Data& xpub) {
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
    Data firstHash = Hash::sha3_256(cborData.data(), cborData.data() + cborData.size());
    Data blake = Hash::blake2b(firstHash.data(), firstHash.data() + firstHash.size(), 28);
    return blake;
}
