// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Tendermint/Address.h"
#include <TrustWalletCore/TWHRP.h>

using json = nlohmann::json;

const std::string AMINO_PREFIX_SEND_MESSAGE = "cosmos-sdk/MsgSend";
const std::string AMINO_PREFIX_TRANSACTION = "auth/StdTx";

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
            jsonCoins.push_back(
                amountJSON(std::to_string(coin.amount()), coin.denom()));
        }

        jsonMsg["amount"] = jsonCoins;
        jsonMsg["from_address"] = input.message().from_address();
        jsonMsg["to_address"] = input.message().to_address();
    }

    return wrapperJSON(AMINO_PREFIX_SEND_MESSAGE, jsonMsg);
}

json txJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonTx;
    
    jsonTx["memo"] = input.memo();
    jsonTx["fee"] = feeJSON(input.fee());
    jsonTx["msg"] = json::array({messageJSON(input)});
    
    return jsonTx;  
}

json TW::Cosmos::signatureJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonTx = txJSON(input);
    return wrapperJSON(AMINO_PREFIX_TRANSACTION, jsonTx);
}