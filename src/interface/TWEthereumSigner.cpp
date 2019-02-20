// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumSigner.h>

#include "../Ethereum/Signer.h"
#include "../proto/Ethereum.pb.h"

using namespace TW;
using namespace TW::Ethereum;

using boost::multiprecision::uint256_t;

static inline uint256_t parse_uint256(const std::string& data) {
    uint256_t result;
    import_bits(result, data.begin(), data.end());
    return result;
}

static inline Data store_uint256(const uint256_t& v) {
    Data bytes;
    bytes.reserve(32);
    export_bits(v, std::back_inserter(bytes), 8);
    return bytes;
}

TW_Ethereum_Proto_SigningOutput TWEthereumSignerSign(TW_Ethereum_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        /* nonce: */ parse_uint256(input.nonce()),
        /* gasPrice: */ parse_uint256(input.gas_price()),
        /* gasLimit: */ parse_uint256(input.gas_limit()),
        /* to: */ Address(input.to_address()),
        /* amount: */ parse_uint256(input.amount()),
        /* payload: */ Data(input.payload().begin(), input.payload().end())
    );

    auto signer = Signer(parse_uint256(input.chain_id()));
    signer.sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();

    auto encoded = RLP::encode(transaction);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto v = store_uint256(transaction.v);
    protoOutput.set_v(v.data(), v.size());

    auto r = store_uint256(transaction.r);
    protoOutput.set_r(r.data(), r.size());

    auto s = store_uint256(transaction.s);
    protoOutput.set_s(s.data(), s.size());
    
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
