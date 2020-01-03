// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Coinex.pb.h"
#include <nlohmann/json.hpp>

extern const std::string COINEX_PREFIX_SEND_COIN_MESSAGE;
extern const std::string COINEX_PREFIX_STAKE_MESSAGE;
extern const std::string COINEX_PREFIX_UNSTAKE_MESSAGE;
extern const std::string COINEX_PREFIX_REDELEGATE_MESSAGE;
extern const std::string COINEX_PREFIX_WITHDRAW_STAKE_MESSAGE;

extern const std::string COINEX_PREFIX_SET_ALIAS_MESSAGE;

extern const std::string COINEX_PREFIX_CREATE_ORDER_MESSAGE;
extern const std::string COINEX_PREFIX_CANCEL_ORDER_MESSAGE;

extern const std::string COINEX_PREFIX_TRANSACTION;
extern const std::string COINEX_PREFIX_PUBLIC_KEY;

extern const std::string COINEX_PREFIX_PROPOSAL_VOTE_MESSAGE;

namespace TW::Coinex{

nlohmann::json signaturePreimageJSON(const Proto::SigningInput& input);
nlohmann::json transactionJSON(const Proto::Transaction& transaction, std::string type_prefix);

} // namespace
