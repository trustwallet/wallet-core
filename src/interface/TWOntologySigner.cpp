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
    Ontology::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto contract = std::string(input.contract().begin(), input.contract().end());
    if (contract == "ONT") {
        return OntTxBuilder::build(input);
    }
    if (contract == "ONG") {
        return OngTxBuilder::build(input);
    }
    std::vector<uint8_t> nullData;
    return TWDataCreateWithBytes(nullData.data(), nullData.size());
}
