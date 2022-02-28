// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include <nlohmann/json.hpp>
#include "Base64.h"

using json = nlohmann::json;
using namespace TW;
using namespace TW::Filecoin;

// encodeBigInt encodes a Filecoin BigInt to CBOR.
Data TW::Filecoin::encodeBigInt(const uint256_t& value) {
    if (value.is_zero()) {
        return {};
    }
    Data buf;
    buf.push_back(0); // positive sign
    export_bits(value, std::back_inserter(buf), 8);
    return buf;
}

// cidPrefix is the CID + Multihash prefix of transaction CIDs.
const Data cidPrefix = {
    // CIDv1 with CBOR codec
    0x01,
    0x71,
    // Blake2b-256 with 32 byte output
    0xa0,
    0xe4,
    0x02,
    0x20,
};

Cbor::Encode Transaction::message() const {
    Cbor::Encode cborGasLimit = gasLimit >= 0 ? Cbor::Encode::uint((uint64_t)gasLimit)
                                              : Cbor::Encode::negInt((uint64_t)(-gasLimit - 1));
    return Cbor::Encode::array({
        Cbor::Encode::uint(0),                         // version
        Cbor::Encode::bytes(to.bytes),                 // to address
        Cbor::Encode::bytes(from.bytes),               // from address
        Cbor::Encode::uint(nonce),                     // nonce
        Cbor::Encode::bytes(encodeBigInt(value)),      // value
        cborGasLimit,                                  // gas limit
        Cbor::Encode::bytes(encodeBigInt(gasFeeCap)),  // gas fee cap
        Cbor::Encode::bytes(encodeBigInt(gasPremium)), // gas premium
        Cbor::Encode::uint(0),                         // abi.MethodNum (0 => send)
        Cbor::Encode::bytes(Data())                    // data (empty)
    });
}

Data Transaction::cid() const {
    Data cid;
    cid.reserve(cidPrefix.size() + 32);
    cid.insert(cid.end(), cidPrefix.begin(), cidPrefix.end());
    Data hash = Hash::blake2b(message().encoded(), 32);
    cid.insert(cid.end(), hash.begin(), hash.end());
    return cid;
}
std::string Transaction::serialize(Data& signature) const {
    json tx = {
        {"Message", json{
                {"To", to.string()},
                {"From", from.string()},
                {"Nonce", nonce},
                {"Value", toString(value)},
                {"GasPremium", toString(gasPremium)},
                {"GasFeeCap", toString(gasFeeCap)},
                {"GasLimit", gasLimit},
            }
        },
        {"Signature", json{
                {"Type", 1},
                {"Data", Base64::encode(signature)},
            }
        },
    };
    return tx.dump();
}
