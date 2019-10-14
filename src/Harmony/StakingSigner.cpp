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
    if (input.has_new_validator_message()) {
        auto description = Description(
            /* name */ input.new_validator_message().description().name(),
            /* identity */ input.new_validator_message().description().identity(),
            /* website */ input.new_validator_message().description().website(),
            /* securityContact */ input.new_validator_message().description().security_contact(),
            /* details */ input.new_validator_message().description().details());
        auto rate = Decimal(
            /* value */ load(input.new_validator_message().commission().rate().value()),
            /* precision */ load(input.new_validator_message().commission().rate().precision()));
        auto maxRate = Decimal(
            /* value */ load(input.new_validator_message().commission().max_rate().value()),
            /* precision */ load(
                input.new_validator_message().commission().max_rate().precision()));
        auto maxChangeRate = Decimal(
            /* value */ load(input.new_validator_message().commission().max_change_rate().value()),
            /* precision */ load(
                input.new_validator_message().commission().max_change_rate().precision()));

        auto commission = CommissionRate(rate, maxRate, maxChangeRate);
        auto newValidator = NewValidator(
            /* Description */ description,
            /* Commission */ commission,
            /* MinSelfDelegation */ load(input.new_validator_message().min_self_delegation()),
            /* StakingAddress */ Address(input.new_validator_message().staking_address()),
            /* PubKey */
            Data(input.new_validator_message().pub_key().begin(),
                 input.new_validator_message().pub_key().end()),
            /* Amount */ load(input.new_validator_message().amount()));

        auto stakingTx = StakingTransaction<NewValidator>(
            DirectiveNewValidator, newValidator, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<NewValidator>(key, stakingTx);

        encoded = signer.rlpNoHash<NewValidator>(stakingTx, true);
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
            /* value */ load(input.edit_validator_message().commission_rate().value()),
            /* precision */ load(input.edit_validator_message().commission_rate().precision()));

        auto editValidator = EditValidator(
            /* Description */ description,
            /* StakingAddress */ Address(input.edit_validator_message().staking_address()),
            /* CommissionRate */ commissionRate,
            /* MinSelfDelegation */ load(input.edit_validator_message().min_self_delegation()));

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
    } else if (input.has_redelegate_message()) {
        auto redelegate = Redelegate(Address(input.redelegate_message().delegator_address()),
                                     Address(input.redelegate_message().validator_src_address()),
                                     Address(input.redelegate_message().validator_dst_address()),
                                     load(input.redelegate_message().amount()));
        auto stakingTx = StakingTransaction<Redelegate>(
            DirectiveRedelegate, redelegate, load(input.nonce()), load(input.gas_price()),
            load(input.gas_limit()), load(input.chain_id()), 0, 0);

        signer.sign<Redelegate>(key, stakingTx);

        encoded = signer.rlpNoHash<Redelegate>(stakingTx, true);
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

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<NewValidator> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    auto descriptionEncoded = Data();
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.name));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.identity));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.website));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.securityContact));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.details));
    append(encoded, RLP::encodeList(descriptionEncoded));

    auto commissionEncoded = Data();

    auto rateEncoded = Data();
    append(rateEncoded, RLP::encode(transaction.stakeMsg.commission.rate.value));
    append(commissionEncoded, RLP::encodeList(rateEncoded));

    auto maxRateEncoded = Data();
    append(maxRateEncoded, RLP::encode(transaction.stakeMsg.commission.maxRate.value));
    append(commissionEncoded, RLP::encodeList(maxRateEncoded));

    auto maxChangeRateEncoded = Data();
    append(maxChangeRateEncoded, RLP::encode(transaction.stakeMsg.commission.maxChangeRate.value));
    append(commissionEncoded, RLP::encodeList(maxChangeRateEncoded));

    append(encoded, RLP::encodeList(commissionEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.minSelfDelegation));
    append(encoded, RLP::encode(transaction.stakeMsg.stakingAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.pubKey));
    append(encoded, RLP::encode(transaction.stakeMsg.amount));

    return RLP::encodeList(encoded);
}

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<EditValidator> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    auto descriptionEncoded = Data();
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.name));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.identity));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.website));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.securityContact));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.details));
    append(encoded, RLP::encodeList(descriptionEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.stakingAddress.bytes));

    auto decEncoded = Data();
    append(decEncoded, RLP::encode(transaction.stakeMsg.commissionRate.value));
    append(encoded, RLP::encodeList(decEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.minSelfDelegation));

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

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<Redelegate> &transaction) const
    noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorSrcAddress.bytes));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorDstAddress.bytes));
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