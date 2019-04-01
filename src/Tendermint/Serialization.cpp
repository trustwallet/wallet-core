// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Tendermint/Address.h"
#include <TrustWalletCore/TWHRP.h>
#include "../Base64.h"

using json = nlohmann::json;

const std::string AMINO_PREFIX_SEND_MESSAGE = "cosmos-sdk/MsgSend";
const std::string AMINO_PREFIX_TRANSACTION = "auth/StdTx";
const std::string AMINO_PREFIX_PUBLIC_KEY = "tendermint/PubKeySecp256k1";

json wrapperJSON(std::string type, json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = type;
    jsonMsgWrapper["value"] = jsonObj;

    return jsonMsgWrapper;
}

json amountJSON(std::string amount, std::string denom) {
    json jsonAmount;

    jsonAmount["amount"] = amount;
    jsonAmount["denom"] = denom;
    
    return jsonAmount;
}

json feeJSON(const TW::Cosmos::Proto::Fee& fee) {
    json jsonFee;

    json jsonAmounts = json::array();
    for (auto& amount : fee.amount()) {
        jsonAmounts.push_back(
            amountJSON(std::to_string(amount.amount()), amount.denom()));
    }

    jsonFee["amount"] = jsonAmounts;
    jsonFee["gas"] = std::to_string(fee.gas());

    return jsonFee;
}

json messageJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonMsg;
    
    if (input.has_message()) {
        json jsonCoins = json::array();
        for (auto& coin : input.message().amount()) {
            jsonCoins.push_back(amountJSON(std::to_string(coin.amount()), coin.denom()));
        }

        jsonMsg["amount"] = jsonCoins;
        jsonMsg["from_address"] = input.message().from_address();
        jsonMsg["to_address"] = input.message().to_address();
    }

    return wrapperJSON(AMINO_PREFIX_SEND_MESSAGE, jsonMsg);
}

json messageJSON(const TW::Cosmos::Proto::Transaction& transaction) {
    json jsonMsg;

    json jsonCoins = json::array();
    for (auto& coin : transaction.message().amount()) {
        jsonCoins.push_back(amountJSON(std::to_string(coin.amount()), coin.denom()));
    }

    jsonMsg["amount"] = jsonCoins;
    jsonMsg["from_address"] = transaction.message().from_address();
    jsonMsg["to_address"] = transaction.message().to_address();

    return wrapperJSON(AMINO_PREFIX_SEND_MESSAGE, jsonMsg);
}

json messageJSON(json& coins, std::string from_address, std::string to_address) {
    json jsonMsg;

    jsonMsg["amount"] = coins;
    jsonMsg["from_address"] = from_address;
    jsonMsg["to_address"] = to_address;

    return wrapperJSON(AMINO_PREFIX_SEND_MESSAGE, jsonMsg);
}

json txJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonTx;
    
    jsonTx["fee"] = feeJSON(input.fee());
    jsonTx["memo"] = input.memo();
    jsonTx["msg"] = json::array({messageJSON(input)});
    
    return jsonTx;  
}

json signatureJSON(const TW::Cosmos::Proto::Signature& signature) {
    json jsonSignature;

    jsonSignature["pub_key"]["type"] = AMINO_PREFIX_PUBLIC_KEY;
    jsonSignature["pub_key"]["value"] = TW::Base64::encode(TW::Data(signature.public_key().begin(), signature.public_key().end()));
    jsonSignature["signature"] = TW::Base64::encode(TW::Data(signature.signature().begin(), signature.signature().end()));

    return jsonSignature;
}

json TW::Cosmos::signingJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonTx = txJSON(input);
    return wrapperJSON(AMINO_PREFIX_TRANSACTION, jsonTx);
}

json TW::Cosmos::transactionJSON(const TW::Cosmos::Proto::Transaction& transaction) {
    json jsonTx;
    
    jsonTx["fee"] = feeJSON(transaction.fee());
    jsonTx["memo"] = transaction.memo();
    jsonTx["msg"] = json::array({messageJSON(transaction)});
    jsonTx["signatures"] = json::array({signatureJSON(transaction.signature())});
    
    return wrapperJSON(AMINO_PREFIX_TRANSACTION, jsonTx);  
}