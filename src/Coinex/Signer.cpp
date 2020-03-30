// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../Data.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

using namespace TW;
using namespace TW::Coinex;

using json = nlohmann::json;

Signer::Signer(Proto::SigningInput&& input) {
    if (input.type_prefix().empty()) {
        input.set_type_prefix(COINEX_PREFIX_SEND_COIN_MESSAGE);
    }

    if (input.has_send_coins_message()) {
        auto message = input.send_coins_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_SEND_COIN_MESSAGE);
        }
        *input.mutable_send_coins_message() = message;
    } else if (input.has_stake_message()) {
        auto message = input.stake_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_STAKE_MESSAGE);
        }
        *input.mutable_stake_message() = message;
    } else if(input.has_unstake_message()) {
        auto message = input.unstake_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_UNSTAKE_MESSAGE);
        }
        *input.mutable_unstake_message() = message;
    } else if(input.has_redelegate_message()) {
        auto message = input.redelegate_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_REDELEGATE_MESSAGE);
        }
        *input.mutable_redelegate_message() = message;
    } else if(input.has_withdraw_stake_reward_message()) {
        auto message = input.withdraw_stake_reward_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_WITHDRAW_STAKE_MESSAGE);
        }
        *input.mutable_withdraw_stake_reward_message() = message;
    } else if(input.has_withdraw_stake_reward_and_restake_message()) {
        auto message = input.withdraw_stake_reward_and_restake_message();
        if (message.type_prefix_1().empty()) {
            message.set_type_prefix_1(COINEX_PREFIX_WITHDRAW_STAKE_MESSAGE);
        }
        if (message.type_prefix_2().empty()) {
            message.set_type_prefix_2(COINEX_PREFIX_STAKE_MESSAGE);
        }
        *input.mutable_withdraw_stake_reward_and_restake_message() = message;
    } else if(input.has_set_alias_message()) {
        auto message = input.set_alias_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_SET_ALIAS_MESSAGE);
        }
        *input.mutable_set_alias_message() = message;
    } else if(input.has_create_order_message()) {
        auto message = input.create_order_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_CREATE_ORDER_MESSAGE);
        }
        *input.mutable_create_order_message() = message;
    } else if(input.has_cancel_order_message()) {
        auto message = input.cancel_order_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_CANCEL_ORDER_MESSAGE);
        }
        *input.mutable_cancel_order_message() = message;
    } else if(input.has_proposal_vote_message()) {
        auto message = input.proposal_vote_message();
        if (message.type_prefix().empty()) {
            message.set_type_prefix(COINEX_PREFIX_PROPOSAL_VOTE_MESSAGE);
        }
        *input.mutable_proposal_vote_message() = message;
    }else if(input.has_create_order_and_set_referee_message()) {
        auto message = input.create_order_and_set_referee_message();
        if (message.type_prefix_1().empty()) {
            message.set_type_prefix_1(COINEX_PREFIX_SET_REFEREE_MESSAGE);
        }
        if (message.type_prefix_2().empty()) {
            message.set_type_prefix_2(COINEX_PREFIX_CREATE_ORDER_MESSAGE);
        }
        *input.mutable_create_order_and_set_referee_message() = message;
    }
    this->input = input;
}

std::vector<uint8_t> Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(signaturePreimage());
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}

std::string Signer::signaturePreimage() const {
    return signaturePreimageJSON(input).dump();
}

json Signer::buildTransactionJSON(const Data& signature) const {
    auto sig = Coinex::Proto::Signature();
    sig.set_signature(signature.data(), signature.size());
    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    sig.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto transaction = Coinex::Proto::Transaction();
    *transaction.mutable_fee() = input.fee();
    transaction.set_memo(input.memo());

    if (input.has_send_coins_message()) {
        *transaction.mutable_send_coins_message() = input.send_coins_message();
    } else if (input.has_stake_message()) {
        *transaction.mutable_stake_message() = input.stake_message();
    } else if (input.has_unstake_message()) {
        *transaction.mutable_unstake_message() = input.unstake_message();
    } else if (input.has_redelegate_message()) {
        *transaction.mutable_redelegate_message() = input.redelegate_message();
    } else if (input.has_withdraw_stake_reward_message()) {
        *transaction.mutable_withdraw_stake_reward_message() = input.withdraw_stake_reward_message();
    } else if (input.has_withdraw_stake_reward_and_restake_message()) {
        *transaction.mutable_withdraw_stake_reward_and_restake_message() = input.withdraw_stake_reward_and_restake_message();
    } else if (input.has_set_alias_message()) {
        *transaction.mutable_set_alias_message() = input.set_alias_message();
    } else if (input.has_create_order_message()) {
        *transaction.mutable_create_order_message() = input.create_order_message();
    } else if (input.has_cancel_order_message()) {
        *transaction.mutable_cancel_order_message() = input.cancel_order_message();
    } else if (input.has_proposal_vote_message()) {
        *transaction.mutable_proposal_vote_message() = input.proposal_vote_message();
    } else if (input.has_create_order_and_set_referee_message()) {
        *transaction.mutable_create_order_and_set_referee_message() = input.create_order_and_set_referee_message();
    }

    *transaction.mutable_signature() = sig;

    return transactionJSON(transaction, input.type_prefix());
}

std::string Signer::buildTransaction() const {
    auto signature = sign();
    return buildTransactionJSON(signature).dump();
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();

    auto signature = sign();
    auto txJson = buildTransactionJSON(signature);

    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());

    return output;
}

