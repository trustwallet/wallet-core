// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../proto/Nebulas.pb.h"
#include "../Nebulas/Signer.h"
#include "../Nebulas/Address.h"

#include <TrustWalletCore/TWNebulasSigner.h>

using namespace TW;
using namespace TW::Nebulas;

TW_Nebulas_Proto_SigningOutput TWNebulasSignerSign(TW_Nebulas_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Transaction tx(Address(input.from_address()),
        load(input.nonce()),
        load(input.gas_price()),
        load(input.gas_limit()),
        Address(input.to_address()),
        load(input.amount()),
        load(input.timestamp()),
        Data(input.payload().begin(), input.payload().end())
    );
    
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto signer = Signer(load(input.chain_id()));
    signer.sign(key,tx);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_algorithm(tx.algorithm);
    protoOutput.set_signature(reinterpret_cast<const char *>(tx.signature.data()), tx.signature.size());
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
