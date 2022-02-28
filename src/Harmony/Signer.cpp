// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/RLP.h"
#include "../HexCoding.h"
#include <google/protobuf/util/json_util.h>


using namespace TW;
using namespace TW::Harmony;

std::tuple<uint256_t, uint256_t, uint256_t> Signer::values(const uint256_t &chainID,
                                                           const Data& signature) noexcept {
    auto r = load(Data(signature.begin(), signature.begin() + 32));
    auto s = load(Data(signature.begin() + 32, signature.begin() + 64));
    auto v = load(Data(signature.begin() + 64, signature.begin() + 65));
    v += 35 + chainID + chainID;
    return std::make_tuple(r, s, v);
}

std::tuple<uint256_t, uint256_t, uint256_t>
Signer::sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data& hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
}

template <typename T>
Proto::SigningOutput Signer::prepareOutput(const Data& encoded, const T &transaction) noexcept {
    auto protoOutput = Proto::SigningOutput();

    auto v = store(transaction.v, 1);
    auto r = store(transaction.r, 32);
    auto s = store(transaction.s, 32);

    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_v(v.data(), v.size());
    protoOutput.set_r(r.data(), r.size());
    protoOutput.set_s(s.data(), s.size());

    return protoOutput;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    if (input.has_transaction_message()) {
        return signTransaction(input);
    }
    if (input.has_staking_message()) {
        Harmony::Proto::StakingMessage stakingMessage = input.staking_message();
        if (stakingMessage.has_create_validator_message()) {
            return signCreateValidator(input);
        }
        if (stakingMessage.has_edit_validator_message()) {
            return signEditValidator(input);
        }
        if (stakingMessage.has_delegate_message()) {
            return signDelegate(input);
        }
        if (stakingMessage.has_undelegate_message()) {
            return signUndelegate(input);
        }
        if (stakingMessage.has_collect_rewards()) {
            return signCollectRewards(input);
        }
    }
    return Proto::SigningOutput();
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    return hex(Signer::sign(input).encoded());
}

Proto::SigningOutput Signer::signTransaction(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Address toAddr;
    if (!Address::decode(input.transaction_message().to_address(), toAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto transaction = Transaction(
        /* nonce: */ load(input.transaction_message().nonce()),
        /* gasPrice: */ load(input.transaction_message().gas_price()),
        /* gasLimit: */ load(input.transaction_message().gas_limit()),
        /* fromShardID */ load(input.transaction_message().from_shard_id()),
        /* toShardID */ load(input.transaction_message().to_shard_id()),
        /* to: */ toAddr,
        /* amount: */ load(input.transaction_message().amount()),
        /* payload: */
        Data(input.transaction_message().payload().begin(),
             input.transaction_message().payload().end()));

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash(transaction);
    signer.sign(key, hash, transaction);
    auto protoOutput = Proto::SigningOutput();
    auto encoded = signer.rlpNoHash(transaction, true);

    return prepareOutput<Transaction>(encoded, transaction);
}

Proto::SigningOutput Signer::signCreateValidator(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto description = Description(
        /* name */ input.staking_message().create_validator_message().description().name(),
        /* identity */ input.staking_message().create_validator_message().description().identity(),
        /* website */ input.staking_message().create_validator_message().description().website(),
        /* securityContact */
        input.staking_message().create_validator_message().description().security_contact(),
        /* details */ input.staking_message().create_validator_message().description().details());
    auto rate = Decimal(
        /* value */ load(
            input.staking_message().create_validator_message().commission_rates().rate().value()),
        load(input.staking_message()
                 .create_validator_message()
                 .commission_rates()
                 .rate()
                 .precision()));
    auto maxRate = Decimal(
        /* value */
        load(input.staking_message()
                 .create_validator_message()
                 .commission_rates()
                 .max_rate()
                 .value()),
        load(input.staking_message()
                 .create_validator_message()
                 .commission_rates()
                 .max_rate()
                 .precision()));
    auto maxChangeRate = Decimal(
        /* value */
        load(input.staking_message()
                 .create_validator_message()
                 .commission_rates()
                 .max_change_rate()
                 .value()),
        load(input.staking_message()
                 .create_validator_message()
                 .commission_rates()
                 .max_change_rate()
                 .precision()));

    auto commissionRates = CommissionRate(rate, maxRate, maxChangeRate);
    std::vector<Data> slotPubKeys;
    for (auto pk : input.staking_message().create_validator_message().slot_pub_keys()) {
        slotPubKeys.emplace_back(Data(pk.begin(), pk.end()));
    }
    std::vector<Data> slotKeySigs;
    for (auto sig : input.staking_message().create_validator_message().slot_key_sigs()) {
        slotKeySigs.emplace_back(Data(sig.begin(), sig.end()));
    }
    Address validatorAddr;
    if (!Address::decode(input.staking_message().create_validator_message().validator_address(),
                         validatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto createValidator = CreateValidator(
        /* ValidatorAddress */ validatorAddr,
        /* Description */ description,
        /* Commission */ commissionRates,
        /* MinSelfDelegation */
        load(input.staking_message().create_validator_message().min_self_delegation()),
        /* MaxTotalDelegation */
        load(input.staking_message().create_validator_message().max_total_delegation()),
        /* PubKey */ slotPubKeys,
        /* BlsSig */ slotKeySigs,
        /* Amount */ load(input.staking_message().create_validator_message().amount()));

    auto stakingTx = Staking<CreateValidator>(
        DirectiveCreateValidator, createValidator, load(input.staking_message().nonce()),
        load(input.staking_message().gas_price()), load(input.staking_message().gas_limit()),
        load(input.chain_id()), 0, 0);

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash<CreateValidator>(stakingTx);
    signer.sign(key, hash, stakingTx);
    auto encoded = signer.rlpNoHash<CreateValidator>(stakingTx, true);

    return prepareOutput<Staking<CreateValidator>>(encoded, stakingTx);
}

Proto::SigningOutput Signer::signEditValidator(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto description = Description(
        /* name */ input.staking_message().edit_validator_message().description().name(),
        /* identity */ input.staking_message().edit_validator_message().description().identity(),
        /* website */ input.staking_message().edit_validator_message().description().website(),
        /* securityContact */
        input.staking_message().edit_validator_message().description().security_contact(),
        /* details */ input.staking_message().edit_validator_message().description().details());

    std::optional<Decimal> commissionRate;

    if (input.staking_message().edit_validator_message().has_commission_rate()) {
        commissionRate = Decimal(
            /* value */ load(
                input.staking_message().edit_validator_message().commission_rate().value()),
            load(input.staking_message().edit_validator_message().commission_rate().precision()));
    }

    Address validatorAddr;
    if (!Address::decode(input.staking_message().edit_validator_message().validator_address(),
                         validatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto editValidator = EditValidator(
        /* ValidatorAddress */ validatorAddr,
        /* Description */ description,
        /* CommissionRate */ commissionRate,
        /* MinSelfDelegation */
        load(input.staking_message().edit_validator_message().min_self_delegation()),
        /* MaxTotalDelegation */
        load(input.staking_message().edit_validator_message().max_total_delegation()),
        /* SlotKeyToRemove */
        Data(input.staking_message().edit_validator_message().slot_key_to_remove().begin(),
             input.staking_message().edit_validator_message().slot_key_to_remove().end()),
        /* SlotKeyToAdd */
        Data(input.staking_message().edit_validator_message().slot_key_to_add().begin(),
             input.staking_message().edit_validator_message().slot_key_to_add().end()),
        /* SlotKeyToAddSig */
        Data(input.staking_message().edit_validator_message().slot_key_to_add_sig().begin(),
             input.staking_message().edit_validator_message().slot_key_to_add_sig().end()),
        /* Active */
        load(input.staking_message().edit_validator_message().active()));

    auto stakingTx = Staking<EditValidator>(
        DirectiveEditValidator, editValidator, load(input.staking_message().nonce()),
        load(input.staking_message().gas_price()), load(input.staking_message().gas_limit()),
        load(input.chain_id()), 0, 0);

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash<EditValidator>(stakingTx);
    signer.sign(key, hash, stakingTx);
    auto encoded = signer.rlpNoHash<EditValidator>(stakingTx, true);

    return prepareOutput<Staking<EditValidator>>(encoded, stakingTx);
}

Proto::SigningOutput Signer::signDelegate(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    Address delegatorAddr;
    if (!Address::decode(input.staking_message().delegate_message().delegator_address(),
                         delegatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    Address validatorAddr;
    if (!Address::decode(input.staking_message().delegate_message().validator_address(),
                         validatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto delegate = Delegate(delegatorAddr, validatorAddr,
                             load(input.staking_message().delegate_message().amount()));
    auto stakingTx =
        Staking<Delegate>(DirectiveDelegate, delegate, load(input.staking_message().nonce()),
                          load(input.staking_message().gas_price()),
                          load(input.staking_message().gas_limit()), load(input.chain_id()), 0, 0);

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash<Delegate>(stakingTx);
    signer.sign(key, hash, stakingTx);
    auto encoded = signer.rlpNoHash<Delegate>(stakingTx, true);

    return prepareOutput<Staking<Delegate>>(encoded, stakingTx);
}

Proto::SigningOutput Signer::signUndelegate(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    Address delegatorAddr;
    if (!Address::decode(input.staking_message().undelegate_message().delegator_address(),
                         delegatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    Address validatorAddr;
    if (!Address::decode(input.staking_message().undelegate_message().validator_address(),
                         validatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto undelegate = Undelegate(delegatorAddr, validatorAddr,
                                 load(input.staking_message().undelegate_message().amount()));
    auto stakingTx = Staking<Undelegate>(
        DirectiveUndelegate, undelegate, load(input.staking_message().nonce()),
        load(input.staking_message().gas_price()), load(input.staking_message().gas_limit()),
        load(input.chain_id()), 0, 0);

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash<Undelegate>(stakingTx);
    signer.sign(key, hash, stakingTx);
    auto encoded = signer.rlpNoHash<Undelegate>(stakingTx, true);

    return prepareOutput<Staking<Undelegate>>(encoded, stakingTx);
}

Proto::SigningOutput Signer::signCollectRewards(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    Address delegatorAddr;
    if (!Address::decode(input.staking_message().collect_rewards().delegator_address(),
                         delegatorAddr)) {
        // invalid to address
        return Proto::SigningOutput();
    }
    auto collectRewards = CollectRewards(delegatorAddr);
    auto stakingTx = Staking<CollectRewards>(
        DirectiveCollectRewards, collectRewards, load(input.staking_message().nonce()),
        load(input.staking_message().gas_price()), load(input.staking_message().gas_limit()),
        load(input.chain_id()), 0, 0);

    auto signer = Signer(uint256_t(load(input.chain_id())));
    auto hash = signer.hash<CollectRewards>(stakingTx);
    signer.sign(key, hash, stakingTx);
    auto encoded = signer.rlpNoHash<CollectRewards>(stakingTx, true);

    return prepareOutput<Staking<CollectRewards>>(encoded, stakingTx);
}

template <typename T>
void Signer::sign(const PrivateKey &privateKey, const Data& hash, T &transaction) const noexcept {
    auto tuple = sign(chainID, privateKey, hash);
    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::rlpNoHash(const Transaction &transaction, const bool include_vrs) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.nonce));
    append(encoded, RLP::encode(transaction.gasPrice));
    append(encoded, RLP::encode(transaction.gasLimit));
    append(encoded, RLP::encode(transaction.fromShardID));
    append(encoded, RLP::encode(transaction.toShardID));
    append(encoded, RLP::encode(transaction.to.getKeyHash()));
    append(encoded, RLP::encode(transaction.amount));
    append(encoded, RLP::encode(transaction.payload));
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
Data Signer::rlpNoHash(const Staking<Directive> &transaction, const bool include_vrs) const
    noexcept {
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

Data Signer::rlpNoHashDirective(const Staking<CreateValidator> &transaction) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.getKeyHash()));

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
    for (auto pk : transaction.stakeMsg.slotPubKeys) {
        append(slotPubKeysEncoded, RLP::encode(pk));
    }
    append(encoded, RLP::encodeList(slotPubKeysEncoded));

    auto slotBlsSigsEncoded = Data();
    for (auto sig : transaction.stakeMsg.slotKeySigs) {
        append(slotBlsSigsEncoded, RLP::encode(sig));
    }
    append(encoded, RLP::encodeList(slotBlsSigsEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.amount));

    return RLP::encodeList(encoded);
}

Data Signer::rlpNoHashDirective(const Staking<EditValidator> &transaction) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;

    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.getKeyHash()));

    auto descriptionEncoded = Data();
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.name));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.identity));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.website));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.securityContact));
    append(descriptionEncoded, RLP::encode(transaction.stakeMsg.description.details));
    append(encoded, RLP::encodeList(descriptionEncoded));

    auto decEncoded = Data();
    if (transaction.stakeMsg.commissionRate.has_value()) {
        // Note: std::optional.value() is not available in XCode with target < iOS 12; using '*'
        append(decEncoded, RLP::encode((*transaction.stakeMsg.commissionRate).value));
    }
    append(encoded, RLP::encodeList(decEncoded));

    append(encoded, RLP::encode(transaction.stakeMsg.minSelfDelegation));
    append(encoded, RLP::encode(transaction.stakeMsg.maxTotalDelegation));

    append(encoded, RLP::encode(transaction.stakeMsg.slotKeyToRemove));
    append(encoded, RLP::encode(transaction.stakeMsg.slotKeyToAdd));
    append(encoded, RLP::encode(transaction.stakeMsg.slotKeyToAddSig));

    append(encoded, RLP::encode(transaction.stakeMsg.active));

    return RLP::encodeList(encoded);
}

Data Signer::rlpNoHashDirective(const Staking<Delegate> &transaction) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.getKeyHash()));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.getKeyHash()));
    append(encoded, RLP::encode(transaction.stakeMsg.amount));
    return RLP::encodeList(encoded);
}

Data Signer::rlpNoHashDirective(const Staking<Undelegate> &transaction) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.getKeyHash()));
    append(encoded, RLP::encode(transaction.stakeMsg.validatorAddress.getKeyHash()));
    append(encoded, RLP::encode(transaction.stakeMsg.amount));
    return RLP::encodeList(encoded);
}

Data Signer::rlpNoHashDirective(const Staking<CollectRewards> &transaction) const noexcept {
    auto encoded = Data();
    using namespace TW::Ethereum;
    append(encoded, RLP::encode(transaction.stakeMsg.delegatorAddress.getKeyHash()));
    return RLP::encodeList(encoded);
}

std::string Signer::txnAsRLPHex(Transaction &transaction) const noexcept {
    return TW::hex(rlpNoHash(transaction, false));
}

template <typename Directive>
std::string Signer::txnAsRLPHex(Staking<Directive> &transaction) const noexcept {
    return TW::hex(rlpNoHash<Directive>(transaction, false));
}

Data Signer::hash(const Transaction &transaction) const noexcept {
    return Hash::keccak256(rlpNoHash(transaction, false));
}

template <typename Directive>
Data Signer::hash(const Staking<Directive> &transaction) const noexcept {
    return Hash::keccak256(rlpNoHash<Directive>(transaction, false));
}
