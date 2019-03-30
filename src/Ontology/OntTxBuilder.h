// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Ont.h"

#include "../proto/Ontology.pb.h"

#include <TrustWalletCore/TWOntologyProto.h>

namespace TW::Ontology {

class OntTxBuilder {

  public:
    static TW_Ontology_Proto_SigningOutput decimals(const Ontology::Proto::SigningInput& input) {
        auto queryAddress = Address(input.query_address());
        auto transaction = Ont().balanceOf(queryAddress);
        auto encoded = transaction.serialize();
        auto protoOutput = Proto::SigningOutput();
        protoOutput.set_encoded(encoded.data(), encoded.size());
        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    }

    static TW_Ontology_Proto_SigningOutput balanceOf(const Ontology::Proto::SigningInput& input) {
        auto queryAddress = Address(input.query_address());
        auto transaction = Ont().balanceOf(queryAddress);
        auto encoded = transaction.serialize();
        auto protoOutput = Proto::SigningOutput();
        protoOutput.set_encoded(encoded.data(), encoded.size());
        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    }

    static TW_Ontology_Proto_SigningOutput transfer(const Ontology::Proto::SigningInput& input) {
        auto payerSigner = Signer(input.payer_private_key());
        auto fromSigner = Signer(input.owner_private_key());
        auto toAddress = Address(input.to_address());
        auto tranferTx = Ont().transfer(fromSigner, toAddress, input.amount(), payerSigner,
                                        input.gas_price(), input.gas_limit());
        auto encoded = tranferTx.serialize();
        auto protoOutput = Proto::SigningOutput();
        protoOutput.set_encoded(encoded.data(), encoded.size());
        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    }

    static TW_Ontology_Proto_SigningOutput build(const Ontology::Proto::SigningInput& input) {
        auto method = std::string(input.method().begin(), input.method().end());
        if (method == "transfer") {
            return OntTxBuilder::transfer(input);
        }
        if (method == "balanceOf") {
            return OntTxBuilder::balanceOf(input);
        }
        if (method == "decimals") {
            return OntTxBuilder::decimals(input);
        }
        std::vector<uint8_t> nullData;
        return TWDataCreateWithBytes(nullData.data(), nullData.size());
    }
};

} // namespace TW::Ontology
