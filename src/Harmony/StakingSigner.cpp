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
    auto delegate = Delegate(Address(input.delegate_message().delegator_address()),
                             Address(input.delegate_message().validator_address()),
                             load(input.delegate_message().amount()));
    auto stakingTx = StakingTransaction<Delegate>(
        /* directive: */ 0x2,
        /* stakeMsg: */ delegate,
        /* nonce: */ load(input.nonce()),
        /* gasPrice: */ load(input.gas_price()),
        /* gasLimit: */ load(input.gas_limit()));
    signer.sign<Delegate>(key, stakingTx);

    auto encoded = signer.rlpNoHash<Delegate>(stakingTx, true);
    auto v = store(stakingTx.v);
    auto r = store(stakingTx.r);
    auto s = store(stakingTx.s);

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

Data StakingSigner::rlpNoHashDirective(const StakingTransaction<Delegate> &transaction) const
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
    if (transaction.directive == DirectiveDelegate) {
        append(encoded, rlpNoHashDirective(transaction));
    }
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