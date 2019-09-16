// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Cosmos/Address.h"
#include <TrustWalletCore/TWHRP.h>
#include "../HexCoding.h"

using namespace TW;

using json = nlohmann::json;

static inline std::string addressString(const std::string& bytes) {
    auto data = std::vector<uint8_t>(bytes.begin(), bytes.end());
    auto address = Cosmos::Address(HRP_BINANCE, data);
    return address.string();
}

json Binance::signatureJSON(const Binance::Proto::SigningInput& input) {
    json j;
    j["account_number"] = std::to_string(input.account_number());
    j["chain_id"] = input.chain_id();
    j["data"] = nullptr;
    j["memo"] = input.memo();
    j["msgs"] = json::array({orderJSON(input)});
    j["sequence"] = std::to_string(input.sequence());
    j["source"] = std::to_string(input.source());
    return j;
}

json Binance::orderJSON(const Binance::Proto::SigningInput& input) {
    json j;
    if (input.has_trade_order()) {
        j["id"] = input.trade_order().id();
        j["ordertype"] = 2;
        j["price"] = input.trade_order().price();
        j["quantity"] = input.trade_order().quantity();
        j["sender"] = addressString(input.trade_order().sender());
        j["side"] = input.trade_order().side();
        j["symbol"] = input.trade_order().symbol();
        j["timeinforce"] = input.trade_order().timeinforce();
    } else if (input.has_cancel_trade_order()) {
        j["refid"] = input.cancel_trade_order().refid();
        j["sender"] = addressString(input.cancel_trade_order().sender());
        j["symbol"] = input.cancel_trade_order().symbol();
    } else if (input.has_send_order()) {
        j["inputs"] = inputsJSON(input.send_order());
        j["outputs"] = outputsJSON(input.send_order());
    } else if (input.has_freeze_order()) {
        j["from"] = addressString(input.freeze_order().from());
        j["symbol"] = input.freeze_order().symbol();
        j["amount"] = input.freeze_order().amount();
    } else if (input.has_unfreeze_order()) {
        j["from"] = addressString(input.unfreeze_order().from());
        j["symbol"] = input.unfreeze_order().symbol();
        j["amount"] = input.unfreeze_order().amount();
    } else if (input.has_htlt_order()) {
        j["from"] = addressString(input.htlt_order().from());
        j["to"] = addressString(input.htlt_order().to());
        j["recipient_other_chain"] = input.htlt_order().recipient_other_chain();
        j["sender_other_chain"] = input.htlt_order().sender_other_chain();
        j["random_number_hash"] = hex(input.htlt_order().random_number_hash());
        j["timestamp"] = input.htlt_order().timestamp();
        j["amount"] = tokensJSON(input.htlt_order().amount());
        j["expected_income"] = input.htlt_order().expected_income();
        j["height_span"] = input.htlt_order().height_span();
        j["cross_chain"] = input.htlt_order().cross_chain();
    } else if (input.has_deposithtlt_order()) {
        j["from"] = addressString(input.deposithtlt_order().from());
        j["swap_id"] = hex(input.deposithtlt_order().swap_id());
        j["amount"] = tokensJSON(input.deposithtlt_order().amount());
    } else if (input.has_claimhtlt_order()) {
        j["from"] = addressString(input.claimhtlt_order().from());
        j["swap_id"] = hex(input.claimhtlt_order().swap_id());
        j["random_number"] = hex(input.claimhtlt_order().random_number());
    } else if (input.has_refundhtlt_order()) {
        j["from"] = addressString(input.refundhtlt_order().from());
        j["swap_id"] = hex(input.refundhtlt_order().swap_id());
    }
    return j;
}

json Binance::inputsJSON(const Binance::Proto::SendOrder& order) {
    json j = json::array();
    for (auto& input : order.inputs()) {
        json sj;
        sj["address"] = addressString(input.address());
        sj["coins"] = tokensJSON(input.coins());
        j.push_back(sj);
    }
    return j;
}

json Binance::outputsJSON(const Binance::Proto::SendOrder& order) {
    json j = json::array();
    for (auto& output : order.outputs()) {
        json sj;
        sj["address"] = addressString(output.address());
        sj["coins"] = tokensJSON(output.coins());
        j.push_back(sj);
    }
    return j;
}

json Binance::tokensJSON(
    const ::google::protobuf::RepeatedPtrField<Binance::Proto::SendOrder_Token>& tokens) {
    json j = json::array();
    for (auto& token : tokens) {
        json sj;
        sj["denom"] = token.denom();
        sj["amount"] = token.amount();
        j.push_back(sj);
    }
    return j;
}
