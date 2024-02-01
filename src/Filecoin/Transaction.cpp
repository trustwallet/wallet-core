// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"
#include <nlohmann/json.hpp>
#include "Base64.h"

namespace TW::Filecoin {

using json = nlohmann::json;

// encodeBigInt encodes a Filecoin BigInt to CBOR.
Data encodeBigInt(const uint256_t& value) {
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
        Cbor::Encode::uint(0),                             // version
        Cbor::Encode::bytes(to.toBytes()),                   // to address
        Cbor::Encode::bytes(from.toBytes()),                 // from address
        Cbor::Encode::uint(nonce),                         // nonce
        Cbor::Encode::bytes(encodeBigInt(value)),            // value
        cborGasLimit,                                            // gas limit
        Cbor::Encode::bytes(encodeBigInt(gasFeeCap)),  // gas fee cap
        Cbor::Encode::bytes(encodeBigInt(gasPremium)), // gas premium
        Cbor::Encode::uint(method),                        // abi.MethodNum
        Cbor::Encode::bytes(params)                          // params
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

std::string Transaction::serialize(SignatureType signatureType, const Data& signature) const {
    // clang-format off
    json message = {
        {"To", to.string()},
        {"From", from.string()},
        {"Nonce", nonce},
        {"Value", toString(value)},
        {"GasPremium", toString(gasPremium)},
        {"GasFeeCap", toString(gasFeeCap)},
        {"GasLimit", gasLimit},
        {"Method", method},
    };
    if (!params.empty()) {
        message["Params"] = Base64::encode(params);
    }

    json tx = {
        {"Message", message},
        {"Signature", json{
                {"Type", static_cast<uint8_t>(signatureType)},
                {"Data", Base64::encode(signature)},
            }
        },
    };
    // clang-format on
    return tx.dump();
}

} // namespace TW::Filecoin
