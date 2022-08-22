
#include "OepTxBuilder.h"

using namespace TW;
namespace TW::Ontology {

Data OepTxBuilder::decimals(const Ontology::Proto::SigningInput &input) {
    auto transaction = Oep(input.contract()).decimals(input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OepTxBuilder::balanceOf(const Ontology::Proto::SigningInput &input) {
    auto queryAddress = Address(input.query_address());
    auto transaction = Oep(input.contract()).balanceOf(queryAddress, input.nonce());
    auto encoded = transaction.serialize();
    return encoded;
}

Data OepTxBuilder::transfer(const Ontology::Proto::SigningInput &input) {
    auto payerSigner = Signer(PrivateKey(input.payer_private_key()));
    auto fromSigner = Signer(PrivateKey(input.owner_private_key()));
    auto toAddress = Address(input.to_address());
    auto tranferTx = Oep(input.contract()).transfer(fromSigner, toAddress, input.amount(), payerSigner,
                                    input.gas_price(), input.gas_limit(), input.nonce());
    auto encoded = tranferTx.serialize();
    return encoded;
}

Data OepTxBuilder::build(const Ontology::Proto::SigningInput &input) {
    auto method = std::string(input.method().begin(), input.method().end());
    if (method == "transfer") {
        return OepTxBuilder::transfer(input);
    } else if (method == "balanceOf") {
        return OepTxBuilder::balanceOf(input);
    } else if (method == "decimals") {
        return OepTxBuilder::decimals(input);
    }
    return Data();
}

Transaction OepTxBuilder::buildTransferTx(const Ontology::Proto::TransactionInput &input) {
    auto fromSigner = Address(input.owner_address());
    auto toAddress = Address(input.to_address());
    auto transferTx = Oep(input.contract()).unsignedTransfer(fromSigner, toAddress, input.amount(),
                                    input.gas_price(), input.gas_limit(), input.nonce());
    return transferTx;
}

} // namespace TW::Ontology