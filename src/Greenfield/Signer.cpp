// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "ProtobufSerialization.h"
#include "PublicKey.h"
#include "SignerEip712.h"

namespace TW::Greenfield {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    Data signature = SignerEip712::sign(input);

    PrivateKey privateKey(data(input.private_key()));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    return compile(input, publicKey, signature);
}

TxCompiler::Proto::PreSigningOutput Signer::preImageHashes(const Proto::SigningInput& input) {
    TxCompiler::Proto::PreSigningOutput output;

    // At this moment, we support Eip712 signing mode only.
    const auto preImage = SignerEip712::preImageHash(input);
    auto preImageData = data(preImage.typedData.dump());

    output.set_data_hash(preImage.typedDataHash.data(), preImage.typedDataHash.size());
    output.set_data(preImageData.data(), preImageData.size());
    return output;
}

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature) {
    Proto::SigningOutput output;

    // At this moment, we support Protobuf serialization only.
    auto jsonResult = ProtobufSerialization::encodeTxProtobuf(input, publicKey, signature);
    if (jsonResult.isFailure()) {
        output.set_error(jsonResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(jsonResult.error()));
        return output;
    }

    output.set_signature(signature.data(), signature.size());
    output.set_serialized(jsonResult.payload().dump());
    return output;
}

} // namespace TW::Greenfield
