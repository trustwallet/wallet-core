// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Cbor.h"
#include "../Data.h"
#include "../Base58.h"
#include "../Crc.h"
#include "../HexCoding.h"
//#include "../Hash.h"

#include <array>
#include <iostream>
#include <vector>

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
        auto array0 = elems[0];
        auto tag = array0.getTagValue();
        if (tag != 24) {
            // wrong tag value
            return false;
        }
        Data addressAsArray = array0.getTagElement().getStringData();
        // debug
        uint64_t crcPresent = (uint32_t)elems[1].getValue();
        uint32_t crcComputed = TW::Crc::crc32(addressAsArray);
        if (crcPresent != crcComputed) {
            // CRC mismatch
            return false;
        }
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
                Data addressAsArray = array0.getTagElement().getStringData();
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
                    root = elems2[0].getStringData();
                    attrs = elems2[1].getData();
                    type = (TW::byte)(elems2[2].getValue());
                }
            }
        }
    } catch (exception& ex) {
        throw std::invalid_argument("Invalid address string EXcEP");
    }
}

string Address::string() const {
    // put together string represenatation (CBOR encopde, Base58 encode)
    // inner data: pubkey, attrs, type
    Cbor::Encode cbor1;
    cbor1.addArray(vector<Cbor::Encode>{
        Cbor::Encode().addString(root),
        Cbor::Encode(attrs),
        Cbor::Encode().addPInt(type),
    });
    auto cborbase = cbor1.getData();
    
    // crc checksum 
    auto crc = TW::Crc::crc32(cborbase);
    // second pack: tag, base, crc
    Cbor::Encode cbor2;
    cbor2.addArray(vector<Cbor::Encode>{
        Cbor::Encode().addTag(24, Cbor::Encode().addString(cborbase)),
        Cbor::Encode().addPInt(crc),
    });
    auto cbor2Data = cbor2.getData();
    return Base58::bitcoin.encode(cbor2Data);
}
