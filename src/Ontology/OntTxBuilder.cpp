// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OntTxBuilder.h"

namespace TW::Ontology {

Data OntTxBuilder::decimals(const Ontology::Proto::SigningInput& input) {
    auto transaction = Ont().decimals(input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OntTxBuilder::balanceOf(const Ontology::Proto::SigningInput& input) {
    auto queryAddress = Address(input.query_address());
    auto transaction = Ont().balanceOf(queryAddress, input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OntTxBuilder::transfer(const Ontology::Proto::SigningInput& input) {
    auto payerSigner = Signer(PrivateKey(input.payer_private_key()));
    auto fromSigner = Signer(PrivateKey(input.owner_private_key()));
    auto toAddress = Address(input.to_address());
    auto tranferTx = Ont().transfer(fromSigner, toAddress, input.amount(), payerSigner,
                                    input.gas_price(), input.gas_limit(), input.nonce());
    auto encoded = tranferTx.serialize();
    return encoded;
}

Data OntTxBuilder::build(const Ontology::Proto::SigningInput& input) {
    auto method = std::string(input.method().begin(), input.method().end());
    if (method == "transfer") {
        return OntTxBuilder::transfer(input);
    } else if (method == "balanceOf") {
        return OntTxBuilder::balanceOf(input);
    } else if (method == "decimals") {
        return OntTxBuilder::decimals(input);
    }
    return Data();
}

Transaction OntTxBuilder::buildTransferTx(const Ontology::Proto::SigningInput &input) {
    auto fromSigner = Address(input.owner_address());
    auto toAddress = Address(input.to_address());
    auto payerAddress = Address(input.payer_address());
    auto transferTx = Ont().unsignedTransfer(fromSigner, toAddress, input.amount(), payerAddress,
                                    input.gas_price(), input.gas_limit(), input.nonce());
    return transferTx;
}

} // namespace TW::Ontology
