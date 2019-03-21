// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWOntologySigner.h>

#include "../Ontology/Ont.h"
#include "../Ontology/Ong.h"
#include "../Ontology/Account.h"
#include "../Ontology/Address.h"
#include "../proto/Ontology.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Ontology;

TW_Ontology_Proto_SigningOutput TWOntologyOntBalanceOf(const Ontology::Proto::SigningInput &input) {
    auto queryAddress = input.query_address();
    auto transaction = Ont().balanceOf(queryAddress);
    auto encoded = transaction.serialize();
    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Ontology_Proto_SigningOutput TWOntologyOntTransfer(const Ontology::Proto::SigningInput &input) {
    auto payerAccount = Account(Data(input.payer_private_key().begin(), input.payer_private_key().end()));
    auto fromAccount = Account(Data(input.from_private_key().begin(), input.from_private_key().end()));
    auto toAddress = Address(Data(input.to_address().begin(), input.to_address().end())).string();
    auto tranferTx = Ont().transfer(fromAccount, toAddress, input.amount(), payerAccount, input.gas_price(), input.gas_limit());
    auto encoded = tranferTx.serialize();
    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Ontology_Proto_SigningOutput TWOntologyOngBalanceOf(const Ontology::Proto::SigningInput &input) {
    auto queryAddress = input.query_address();
    auto transaction = Ong().balanceOf(queryAddress);
    auto encoded = transaction.serialize();
    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Ontology_Proto_SigningOutput TWOntologyOngTransfer(const Ontology::Proto::SigningInput &input) {
    auto payerAccount = Account(Data(input.payer_private_key().begin(), input.payer_private_key().end()));
    auto fromAccount = Account(Data(input.from_private_key().begin(), input.from_private_key().end()));
    auto toAddress = Address(Data(input.to_address().begin(), input.to_address().end())).string();
    auto transaction = Ong().transfer(fromAccount, toAddress, input.amount(), payerAccount, input.gas_price(), input.gas_limit());
    auto encoded = transaction.serialize();
    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Ontology_Proto_SigningOutput TWOntologyOnt(const Ontology::Proto::SigningInput &input) {
    auto method = std::string(input.method().begin(), input.method().end());
    if (method == "transfer") {
        return TWOntologyOntTransfer(input);
    }
    if (method == "balanceOf") {
        return TWOntologyOntBalanceOf(input);
    }
    std::vector<uint8_t> nullData;
    return TWDataCreateWithBytes(nullData.data(), nullData.size());
}

TW_Ontology_Proto_SigningOutput TWOntologyOng(const Ontology::Proto::SigningInput &input) {
    auto method = std::string(input.method().begin(), input.method().end());
    if (method == "transfer") {
        return TWOntologyOngTransfer(input);
    }
    if (method == "balanceOf") {
        return TWOntologyOngBalanceOf(input);
    }
    std::vector<uint8_t> nullData;
    return TWDataCreateWithBytes(nullData.data(), nullData.size());
}

TW_Ontology_Proto_SigningOutput TWOntologySignerSign(TW_Ontology_Proto_SigningInput data) {
    Ontology::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto contract = std::string(input.contract().begin(), input.contract().end());
    if (contract == "ONT") {
        return TWOntologyOnt(input);
    }
    if (contract == "ONG") {
        return TWOntologyOng(input);
    }
    std::vector<uint8_t> nullData;
    return TWDataCreateWithBytes(nullData.data(), nullData.size());
}
