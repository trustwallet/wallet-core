// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StakingSigner.h"
#include "../Ethereum/RLP.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Harmony;

std::tuple<uint256_t, uint256_t, uint256_t> StakingSigner::values(const uint256_t &chainID,
                                                                  const Data &signature) noexcept {
    auto r = load(Data(signature.begin(), signature.begin() + 32));
    auto s = load(Data(signature.begin() + 32, signature.begin() + 64));
    auto v = load(Data(signature.begin() + 64, signature.begin() + 65));
    v += 35 + chainID + chainID;
    return std::make_tuple(r, s, v);
}

std::tuple<uint256_t, uint256_t, uint256_t> StakingSigner::sign(const uint256_t &chainID,
                                                                const PrivateKey &privateKey,
                                                                const Data &hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
}

Proto::StakingTransactionOutput
StakingSigner::sign(const TW::Harmony::Proto::StakingTransactionInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto signer = StakingSigner(uint256_t(load(input.chain_id())));

    auto protoOutput = Proto::StakingTransactionOutput();

    Data encoded, v, r, s;
    if (input.has_create_validator_message()) {
        auto description = Description(
            /* name */ input.create_validator_message().description().name(),
            /* identity */ input.create_validator_message().description().identity(),
            /* website */ input.create_validator_message().description().website(),
            /* securityContact */ input.create_validator_message().description().security_contact(),
            /* details */ input.create_validator_message().description().details());
        auto rate = Decimal(
            /* value */ input.create_validator_message().commission_rates().rate().value());
        auto maxRate = Decimal(
            /* value */
            input.create_validator_message().commission_rates().max_rate().value());
        auto maxChangeRate = Decimal(
            /* value */
            input.create_validator_message().commission_rates().max_change_rate().value());

        auto commissionRates = CommissionRate(rate, maxRate, maxChangeRate);
        std::vector<Data> slotPubKeys;
        for (auto pk = input.create_validator_message().slot_pub_keys().begin();
             pk != input.create_validator_message().slot_pub_keys().end(); ++pk) {
            slotPubKeys.push_back(Data((*pk).begin(), (*pk).end()));
        }
        auto newValidator = CreateValidator(
            /* ValidatorAddress */ Address(input.create_validator_message().validator_address()),
            /* Description */ description,
            /* Commission */ commissionRates,
            /* MinSelfDelegation */ load(input.create_validator_message().min_self_delegation()),
            /* MaxTotalDelegation */ load(input.create_validator_message().max_total_delegation()),
            /* PubKey */ slotPubKeys,
            /* Amount */ load(input.create_validator_message().amount()));

        auto stakingTx = StakingTransaction<CreateValidator>(
            DirectiveCreateValidator, newValidator, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<CreateValidator>(key, stakingTx);

        encoded = signer.rlpNoHash<CreateValidator>(stakingTx, true);
        v = store(stakingTx.v);
        r = store(stakingTx.r);
        s = store(stakingTx.s);
    } else if (input.has_edit_validator_message()) {
        auto description = Description(
            /* name */ input.edit_validator_message().description().name(),
            /* identity */ input.edit_validator_message().description().identity(),
            /* website */ input.edit_validator_message().description().website(),
            /* securityContact */ input.edit_validator_message().description().security_contact(),
            /* details */ input.edit_validator_message().description().details());

        auto commissionRate = Decimal(
            /* value */ input.edit_validator_message().commission_rate().value());

        auto editValidator = EditValidator(
            /* ValidatorAddress */ Address(input.edit_validator_message().validator_address()),
            /* Description */ description,
            /* CommissionRate */ commissionRate,
            /* MinSelfDelegation */ load(input.edit_validator_message().min_self_delegation()),
            /* MaxTotalDelegation */ load(input.edit_validator_message().max_total_delegation()),
            /* SlotKeyToRemove */
            Data(input.edit_validator_message().slot_key_to_remove().begin(),
                 input.edit_validator_message().slot_key_to_remove().end()),
            /* SlotKeyToRemove */
            Data(input.edit_validator_message().slot_key_to_add().begin(),
                 input.edit_validator_message().slot_key_to_add().end()));

        auto stakingTx = StakingTransaction<EditValidator>(
            DirectiveEditValidator, editValidator, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<EditValidator>(key, stakingTx);

        encoded = signer.rlpNoHash<EditValidator>(stakingTx, true);
        v = store(stakingTx.v);
        r = store(stakingTx.r);
        s = store(stakingTx.s);
    } else if (input.has_delegate_message()) {
        auto delegate = Delegate(Address(input.delegate_message().delegator_address()),
                                 Address(input.delegate_message().validator_address()),
                                 load(input.delegate_message().amount()));
        auto stakingTx = StakingTransaction<Delegate>(
            DirectiveDelegate, delegate, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<Delegate>(key, stakingTx);

        encoded = signer.rlpNoHash<Delegate>(stakingTx, true);
        v = store(stakingTx.v);
        r = store(stakingTx.r);
        s = store(stakingTx.s);
    } else if (input.has_undelegate_message()) {
        auto undelegate = Undelegate(Address(input.undelegate_message().delegator_address()),
                                     Address(input.undelegate_message().validator_address()),
                                     load(input.undelegate_message().amount()));
        auto stakingTx = StakingTransaction<Undelegate>(
            DirectiveUndelegate, undelegate, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<Undelegate>(key, stakingTx);

        encoded = signer.rlpNoHash<Undelegate>(stakingTx, true);
        v = store(stakingTx.v);
        r = store(stakingTx.r);
        s = store(stakingTx.s);
    } else if (input.has_collect_rewards()) {
        auto collectRewards = CollectRewards(Address(input.collect_rewards().delegator_address()));
        auto stakingTx = StakingTransaction<CollectRewards>(
            DirectiveCollectRewards, collectRewards, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<CollectRewards>(key, stakingTx);

        encoded = signer.rlpNoHash<CollectRewards>(stakingTx, true);
        v = store(stakingTx.v);
        r = store(stakingTx.r);
        s = store(stakingTx.s);
    } else {
        // should never come here for valid input
    }

    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_v(v.data(), v.size());
    protoOutput.set_r(r.data(), r.size());
    protoOutput.set_s(s.data(), s.size());

    return protoOutput;
}

template <typename Directive>
void StakingSigner::sign(const PrivateKey &privateKey,
                         StakingTransaction<Directive> &transaction) const noexcept {
    auto hash = this->hash<Directive>(transaction);
    auto tuple = StakingSigner::sign(chainID, privateKey, hash);
    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<CreateValidator> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.bytes));

    auto descriptionEncoded = Data();
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.name));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.identity));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.website));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.securityContact));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.details));
    append(encoded, RLP::encodeList(descriptionEncoded));

    auto commissionEncoded = Data();

    auto rateEncoded = Data();
    append(rateEncoded, RLP::encode(transaction.stakeMsg.commissionRates.rate.value));
    append(commissionEncoded, RLP::encodeList(rateEncoded));

    auto maxRateEncoded = Data();
    append(maxRateEncoded, RLP::encode(transaction.stakeMsg.commissionRates.maxRate.value));
    append(commissionEncoded, RLP::encodeList(maxRateEncoded));

    auto maxChangeRateEncoded = Data();
    append(maxChangeRateEncoded,
           RLP::encode(transaction.stakeMsg.commissionRates.maxChangeRate.value));
    append(commissionEncoded, RLP::encodeList(maxChangeRateEncoded));

    append(encoded, RLP::encodeList(commissionEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.minSelfDelegation));
    append(encoded, RLP::encode(transaction.stakeMsg.maxTotalDelegation));

    auto slotPubKeysEncoded = Data();
    for (auto pk = transaction.stakeMsg.slotPubKeys.begin();
         pk != transaction.stakeMsg.slotPubKeys.end(); ++pk) {
        append(slotPubKeysEncoded, RLP::encode(*pk));
    }
    append(encoded, RLP::encodeList(slotPubKeysEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.amount));

    return RLP::encodeList(encoded);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<EditValidator> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.bytes));

    auto descriptionEncoded = Data();
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.name));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.identity));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.website));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.securityContact));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.details));
    append(encoded, RLP::encodeList(descriptionEncoded));

    auto decEncoded = Data();
    append(decEncoded, RLP::encode(transaction.stakeMsg.commissionRate.value));
    append(encoded, RLP::encodeList(decEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.minSelfDelegation));
    append(encoded, RLP::encode(transaction.stakeMsg.maxTotalDelegation));

    append(encoded, RLP::encode(transaction.stakeMsg.slotKeyToRemove));
    append(encoded, RLP::encode(transaction.stakeMsg.slotKeyToAdd));

    return RLP::encodeList(encoded);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<Delegate> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.amount));
    return RLP::encodeList(encoded);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<Undelegate> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.amount));
    return RLP::encodeList(encoded);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<CollectRewards> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.bytes));
    return RLP::encodeList(encoded);
}

template <typename Directive>
Data StakingSigner::rlpNoHash(const StakingTransaction<Directive> &transaction,
                              const bool include_vrs) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.directive));
    append(encoded, rlpNoHashDirective(transaction));

    append(encoded, RLP::encode(transaction.nonce));
    append(encoded, RLP::encode(transaction.gasPrice));
    append(encoded, RLP::encode(transaction.gasLimit));
    if (include_vrs) {
        append(encoded, RLP::encode(transaction.v));
        append(encoded, RLP::encode(transaction.r));
        append(encoded, RLP::encode(transaction.s));
    } else {
        append(encoded, RLP::encode(chainID));
        append(encoded, RLP::encode(0));
        append(encoded, RLP::encode(0));
    }
    return RLP::encodeList(encoded);
}

template <typename Directive>
std::string StakingSigner::txnAsRLPHex(StakingTransaction<Directive> &transaction) const noexcept {
    return TW::hex(rlpNoHash<Directive>(transaction, false));
}

template <typename Directive>
Data StakingSigner::hash(const StakingTransaction<Directive> &transaction) const noexcept {
    return Hash::keccak256(rlpNoHash<Directive>(transaction, false));
}

uint256_t StakingSigner::deriveChainID(const uint256_t v) noexcept {
    return (v - 35) / 2;
}