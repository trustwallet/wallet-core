// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Hash.h"
#include "PrivateKey.h"

using namespace TW;

namespace TW::IoTeX {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    return signer.build();
}

Data Signer::sign() const {
    auto key = PrivateKey(input.privatekey());
    return key.sign(hash(), TWCurveSECP256k1);
}

Proto::SigningOutput Signer::build() const {
    auto signedAction = Proto::Action();
    signedAction.mutable_core()->MergeFrom(action);
    auto key = PrivateKey(input.privatekey());
    auto pk = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended).bytes;
    signedAction.set_senderpubkey(pk.data(), pk.size());
    auto sig = key.sign(hash(), TWCurveSECP256k1);
    signedAction.set_signature(sig.data(), sig.size());

    auto output = Proto::SigningOutput();
    auto serialized = signedAction.SerializeAsString();
    output.set_encoded(serialized);
    auto h = Hash::keccak256(serialized);
    output.set_hash(h.data(), h.size());
    return output;
}

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input, const Data& signature,
                                     const TW::PublicKey& pubKey) noexcept {
    auto signer = Signer(input);
    auto signedAction = Proto::Action();
    signedAction.mutable_core()->MergeFrom(signer.action);

    signedAction.set_senderpubkey(pubKey.bytes.data(), pubKey.bytes.size());
    signedAction.set_signature(signature.data(), signature.size());
    // build output
    auto output = Proto::SigningOutput();
    auto serialized = signedAction.SerializeAsString();
    output.set_encoded(serialized);
    auto h = Hash::keccak256(serialized);
    output.set_hash(h.data(), h.size());
    return output;
}

Data Signer::hash() const {
    return Hash::keccak256(action.SerializeAsString());
}

std::string Signer::signaturePreimage() const {
    return action.SerializeAsString();
}

void Signer::toActionCore() {
    action.ParseFromString(input.SerializeAsString());
    action.DiscardUnknownFields();
}

} // namespace TW::IoTeX
