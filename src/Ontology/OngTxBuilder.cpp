// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OngTxBuilder.h"

namespace TW::Ontology {

Data OngTxBuilder::decimals(const Ontology::Proto::SigningInput& input) {
    auto transaction = Ong().decimals(input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OngTxBuilder::balanceOf(const Ontology::Proto::SigningInput& input) {
    auto queryAddress = Address(input.query_address());
    auto transaction = Ong().balanceOf(queryAddress, input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OngTxBuilder::transfer(const Ontology::Proto::SigningInput& input) {
    auto payer = Signer(PrivateKey(input.payer_private_key()));
    auto owner = Signer(PrivateKey(input.owner_private_key()));
    auto toAddress = Address(input.to_address());
    auto transaction = Ong().transfer(owner, toAddress, input.amount(), payer, input.gas_price(),
                                      input.gas_limit(), input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OngTxBuilder::withdraw(const Ontology::Proto::SigningInput& input) {
    auto payer = Signer(PrivateKey(input.payer_private_key()));
    auto owner = Signer(PrivateKey(input.owner_private_key()));
    auto toAddress = Address(input.to_address());
    auto transaction = Ong().withdraw(owner, toAddress, input.amount(), payer, input.gas_price(),
                                      input.gas_limit(), input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OngTxBuilder::build(const Ontology::Proto::SigningInput& input) {
    auto method = std::string(input.method().begin(), input.method().end());
    if (method == "transfer") {
        return OngTxBuilder::transfer(input);
    } else if (method == "balanceOf") {
        return OngTxBuilder::balanceOf(input);
    } else if (method == "decimals") {
        return OngTxBuilder::decimals(input);
    } else if (method == "withdraw") {
        return OngTxBuilder::withdraw(input);
    }
    return Data();
}

Transaction OngTxBuilder::buildTransferTx(const Ontology::Proto::SigningInput &input) {
    auto fromSigner = Address(input.owner_address());
    auto toAddress = Address(input.to_address());
    auto payerAddress = Address(input.payer_address());
    auto transferTx = Ong().unsignedTransfer(fromSigner, toAddress, input.amount(), payerAddress,
                                             input.gas_price(), input.gas_limit(), input.nonce());
    return transferTx;
}

} // namespace TW::Ontology
