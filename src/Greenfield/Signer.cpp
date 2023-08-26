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
    Proto::SigningOutput output;

    const auto signatureResult = SignerEip712::sign(input);
    if (signatureResult.isFailure()) {
        output.set_error(signatureResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(signatureResult.error()));
        return output;
    }

    const auto signature = signatureResult.payload();

    PrivateKey privateKey(data(input.private_key()));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

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

TxCompiler::Proto::PreSigningOutput Signer::preImageHashes(const Proto::SigningInput& input) {
    TxCompiler::Proto::PreSigningOutput output;

    // At this moment, we support Eip712 signing mode only.
    const auto preImageResult = SignerEip712::preImageHash(input);
    if (preImageResult.isFailure()) {
        output.set_error(preImageResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(preImageResult.error()));
        return output;
    }

    const auto preImage = preImageResult.payload();
    auto preImageData = data(preImage.typedData.dump());

    output.set_data_hash(preImage.typedDataHash.data(), preImage.typedDataHash.size());
    output.set_data(preImageData.data(), preImageData.size());
    return output;
}

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature) {
    Proto::SigningOutput output;

    Data sign = signature;
    if (input.signing_mode() == Proto::SigningMode::Eip712) {
        // Prepare the signature to be compiled.
        SignerEip712::prepareSignature(sign);
    }

    // At this moment, we support Protobuf serialization only.
    auto jsonResult = ProtobufSerialization::encodeTxProtobuf(input, publicKey, sign);
    if (jsonResult.isFailure()) {
        output.set_error(jsonResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(jsonResult.error()));
        return output;
    }

    output.set_signature(sign.data(), sign.size());
    output.set_serialized(jsonResult.payload().dump());
    return output;
}

} // namespace TW::Greenfield
