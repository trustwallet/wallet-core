// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWOntologySigner.h>

#include "../Ontology/OngTxBuilder.h"
#include "../Ontology/OntTxBuilder.h"

using namespace TW;
using namespace TW::Ontology;

TW_Ontology_Proto_SigningOutput TWOntologySignerSign(TW_Ontology_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto contract = std::string(input.contract().begin(), input.contract().end());
    auto output = Proto::SigningOutput();
    try {
        if (contract == "ONT") {
            auto encoded = OntTxBuilder::build(input);
            output.set_encoded(encoded.data(), encoded.size());
        } else if (contract == "ONG") {
            auto encoded = OngTxBuilder::build(input);
            output.set_encoded(encoded.data(), encoded.size());
        }
    } catch (...) {
    }

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
