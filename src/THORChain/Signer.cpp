// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Cosmos/Signer.h"
#include "../proto/THORChain.pb.h"
#include "../proto/Cosmos.pb.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::THORChain;

const std::string TYPE_PREFIX_MSG_SEND = "thorchain/MsgSend";

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    // transfer input into Cosmos input
    Cosmos::Proto::SigningInput inputC;
    inputC.set_account_number(input.account_number());
    inputC.set_chain_id(input.chain_id());
    for (auto i = 0; i < input.fee().amounts_size(); ++i) {
        auto newAmount = inputC.mutable_fee()->add_amounts();
        newAmount->set_denom(input.fee().amounts(i).denom());
        newAmount->set_amount(input.fee().amounts(i).amount());
    }
    inputC.mutable_fee()->set_gas(input.fee().gas());
    inputC.set_memo(input.memo());
    inputC.set_sequence(input.sequence());
    inputC.set_private_key(input.private_key());
    inputC.set_mode(input.mode());
    for (auto i = 0; i < input.messages_size(); ++i) {
        if (input.messages(i).has_send_coins_message()) {
            *(inputC.add_messages()->mutable_send_coins_message()) = input.messages(i).send_coins_message();
            inputC.mutable_messages(i)->mutable_send_coins_message()->set_type_prefix(TYPE_PREFIX_MSG_SEND);
        }
    }
    
    auto outputC = Cosmos::Signer::sign(inputC);

    // transfer output into THORChain output
    Proto::SigningOutput output;
    output.set_signature(outputC.signature());
    output.set_json(outputC.json());
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}
