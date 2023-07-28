// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Cosmos/Signer.h"
#include "../proto/Cosmos.pb.h"

#include <TrustWalletCore/TWCoinType.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;

namespace TW::THORChain {
const std::string TYPE_PREFIX_MSG_SEND_THOR = "thorchain/MsgSend";
const std::string TYPE_PREFIX_MSG_SEND_MAYA = "mayachain/MsgSend";
const std::string FROM_ADDRESS = "fromAddress";
const std::string TO_ADDRESS = "toAddress";
const std::string MAYACHAIN = "mayachain";
const std::string MAYA_ADDRESS = "maya1";

Cosmos::Proto::SigningOutput Signer::sign(Cosmos::Proto::SigningInput& input) noexcept {
    std::string TypePrefixMsgSend;
    TWCoinType TWCoinType;

    if(input.chain_id().find(MAYACHAIN) != std::string::npos){
        TypePrefixMsgSend = TYPE_PREFIX_MSG_SEND_MAYA;
        TWCoinType = TWCoinTypeMAYAChain;
    } else{
        TypePrefixMsgSend = TYPE_PREFIX_MSG_SEND_THOR;
        TWCoinType = TWCoinTypeTHORChain;
    }

    for (auto i = 0; i < input.messages_size(); ++i) {
        if (input.messages(i).has_send_coins_message()) {
            input.mutable_messages(i)->mutable_send_coins_message()->set_type_prefix(TypePrefixMsgSend);
        }
    }
    return Cosmos::Signer::sign(input, TWCoinType);
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Cosmos::Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());

    if(input.chain_id().empty() && checkJSONMayaAddress(json)){
        input.set_chain_id(MAYACHAIN);
    }

    auto output = Signer::sign(input);
    return output.json();
}

bool Signer::checkJSONMayaAddress(const std::string& json) {
    size_t begin = json.find(FROM_ADDRESS);
    size_t end = json.find(TO_ADDRESS);
    size_t maya = json.find(MAYA_ADDRESS);
    
    if(((begin != std::string::npos) && (end != std::string::npos) && (maya != std::string::npos)) && ((maya > begin) && (maya < end))) {
        return TRUE;
    }

    return FALSE;
}

} // namespace TW::THORChain
