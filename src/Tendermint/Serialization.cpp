// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Tendermint/Address.h"
#include <TrustWalletCore/TWHRP.h>

using json = nlohmann::json;

json messageWrapperJSON(json& jsonMsg) {
    json jsonMsgWrapper;

    jsonMsgWrapper["type"] = "cosmos-sdk/MsgSend";
    jsonMsgWrapper["value"] = jsonMsg;

    return jsonMsgWrapper;
}

json feeJSON(const TW::Cosmos::Proto::Fee& fee) {
    json jsonFee;

    json jsonAmounts = json::array();
    for (auto& amount : fee.amount()) {
        json jsonAmount;
        jsonAmount["amount"] = std::to_string(amount.amount());
        jsonAmount["denom"] = amount.denom();
        jsonAmounts.push_back(jsonAmount);
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
            json jsonCoin;
            jsonCoin["amount"] = std::to_string(coin.amount());
            jsonCoin["denom"] = coin.denom();
            jsonCoins.push_back(jsonCoin);
        }

        jsonMsg["amount"] = jsonCoins;
        jsonMsg["from_address"] = input.message().from_address();
        jsonMsg["to_address"] = input.message().to_address();
    }

    return messageWrapperJSON(jsonMsg);;
}

json TW::Cosmos::signatureJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json jsonTx;
    
    jsonTx["account_number"] = std::to_string(input.account_number());
    jsonTx["chain_id"] = input.chain_id();
    jsonTx["fee"] = feeJSON(input.fee());
    jsonTx["memo"] = input.memo();
    jsonTx["msgs"] = json::array({messageJSON(input)});
    jsonTx["sequence"] = std::to_string(input.sequence());
    
    return jsonTx;
}
