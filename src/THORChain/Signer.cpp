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
const std::string TYPE_PREFIX_MSG_SEND = "thorchain/MsgSend";

Cosmos::Proto::SigningOutput Signer::sign(Cosmos::Proto::SigningInput& input) noexcept {
    for (auto i = 0; i < input.messages_size(); ++i) {
        if (input.messages(i).has_send_coins_message()) {
            input.mutable_messages(i)->mutable_send_coins_message()->set_type_prefix(TYPE_PREFIX_MSG_SEND);
        }
    }
    return Cosmos::Signer::sign(input, TWCoinTypeTHORChain);
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Cosmos::Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}

} // namespace TW::THORChain
