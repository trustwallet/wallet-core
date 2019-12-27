// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"

#include "../proto/Nebulas.pb.h"
#include "../Nebulas/Signer.h"
#include "../Nebulas/Address.h"

#include <TrustWalletCore/TWNebulasSigner.h>

using namespace TW;
using namespace TW::Nebulas;

TW_Nebulas_Proto_SigningOutput TWNebulasSignerSign(TW_Nebulas_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign(input);
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWNebulasSignerMessage(TW_Nebulas_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = Signer(load(input.chain_id()));
    auto preImage = signer.getPreImage(input);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(preImage.data()), preImage.size());
}

TWData *_Nonnull TWNebulasSignerTransaction(TW_Nebulas_Proto_SigningInput data, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = Signer(load(input.chain_id()));
    auto preImage = signer.getPreImage(input);
    auto sigHash = Hash::sha3_256(preImage);

    Transaction tx(Address(input.from_address()),
                   load(input.nonce()),
                   load(input.gas_price()),
                   load(input.gas_limit()),
                   Address(input.to_address()),
                   load(input.amount()),
                   load(input.timestamp()),
                   input.payload()
    );

    tx.hash = sigHash;
    tx.chainID = signer.chainID;
    tx.algorithm = 1;
    tx.signature = *reinterpret_cast<const TW::Data *>(signature);
    tx.serializeToRaw();

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_algorithm(tx.algorithm);
    protoOutput.set_signature(reinterpret_cast<const char *>(tx.signature.data()), tx.signature.size());
    protoOutput.set_raw(TW::Base64::encode(tx.raw));
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
