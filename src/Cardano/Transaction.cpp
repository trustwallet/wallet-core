// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "AddressV3.h"

#include "Cbor.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Numeric.h"

namespace TW::Cardano {

TokenAmount TokenAmount::fromProto(const Proto::TokenAmount& proto) {
    std::string assetName;
    if (!proto.asset_name().empty()) {
        assetName = proto.asset_name();
    } else if (!proto.asset_name_hex().empty()) {
        auto assetNameData = parse_hex(proto.asset_name_hex());
        assetName.assign(assetNameData.data(), assetNameData.data() + assetNameData.size());
    }

    return {proto.policy_id(), std::move(assetName), load(proto.amount())};
}

Proto::TokenAmount TokenAmount::toProto() const {
    auto assetNameHex = hex(assetName);

    Proto::TokenAmount tokenAmount;
    tokenAmount.set_policy_id(policyId.data(), policyId.size());
    tokenAmount.set_asset_name(assetName.data(), assetName.size());
    tokenAmount.set_asset_name_hex(assetNameHex.data(), assetNameHex.size());
    const auto amountData = store(amount);
    tokenAmount.set_amount(amountData.data(), amountData.size());
    return tokenAmount;
}

TokenBundle TokenBundle::fromProto(const Proto::TokenBundle& proto) {
    TokenBundle ret;
    const auto addFunctor =  [&ret](auto&& cur) { ret.add(TokenAmount::fromProto(cur)); };
    std::for_each(std::cbegin(proto.token()), std::cend(proto.token()), addFunctor);
    return ret;
}

Proto::TokenBundle TokenBundle::toProto() const {
    Proto::TokenBundle proto;
    for (const auto& t : bundle) {
        *(proto.add_token()) = t.second.toProto();
    }
    return proto;
}

void TokenBundle::add(const TokenAmount& ta) {
    const auto key = ta.key();
    if (auto&& [it, inserted] = bundle.try_emplace(key, ta); !inserted) {
        it->second.amount += ta.amount;
    }
}

uint256_t TokenBundle::getAmount(const std::string& key) const {
    const auto& findkey = bundle.find(key);
    return findkey == bundle.end() ? 0 : findkey->second.amount;
}

std::unordered_set<std::string> TokenBundle::getPolicyIds() const {
    std::unordered_set<std::string> policyIds;
    std::transform(bundle.cbegin(), bundle.cend(),
                   std::inserter(policyIds, policyIds.begin()),
                   [](auto&& cur) { return cur.second.policyId; });
    return policyIds;
}

std::vector<TokenAmount> TokenBundle::getByPolicyId(const std::string& policyId) const {
    std::vector<TokenAmount> filtered;
    for (auto&& t : bundle) {
        if (t.second.policyId == policyId) {
            filtered.emplace_back(t.second);
        }
    }
    return filtered;
}

uint64_t roundupBytesToWords(uint64_t b) {
    return ((b + 7) / 8);
}

const uint64_t TokenBundle::MinUtxoValue = 1000000;

uint64_t TokenBundle::minAdaAmountHelper(uint64_t numPids, uint64_t numAssets, uint64_t sumAssetNameLengths) {
    if (numPids == 0) {
        return MinUtxoValue;
    }

    static const uint64_t coinSize = 0;
    static const uint64_t utxoEntrySizeWithoutVal = 27;
    static const uint64_t adaOnlyUTxOSize = utxoEntrySizeWithoutVal + coinSize; // 27
    static const uint64_t pidSize = 28;

    uint64_t sizeB = 6 + roundupBytesToWords((numAssets * 12) + sumAssetNameLengths + (numPids * pidSize));
    return std::max(MinUtxoValue, (MinUtxoValue / adaOnlyUTxOSize) * (utxoEntrySizeWithoutVal + sizeB));
}

uint64_t TokenBundle::minAdaAmount() const {
    if (size() == 0) {
        // ADA only
        return MinUtxoValue;
    }

    std::unordered_set<std::string> policyIdRegistry;
    std::unordered_set<std::string> assetNameRegistry;
    uint64_t sumAssetNameLengths = 0;
    for (const auto& t : bundle) {
        policyIdRegistry.emplace(t.second.policyId);
        if (t.second.assetName.length() > 0) {
            assetNameRegistry.emplace(t.second.assetName);
        }
    }

    auto numPids = uint64_t(policyIdRegistry.size());
    auto numAssets = uint64_t(assetNameRegistry.size());
    for_each(assetNameRegistry.begin(), assetNameRegistry.end(), [&sumAssetNameLengths](auto&& a){ sumAssetNameLengths += a.length(); });
    
    return minAdaAmountHelper(numPids, numAssets, sumAssetNameLengths);
}

TxInput TxInput::fromProto(const Cardano::Proto::TxInput& proto) {
    auto ret = TxInput();
    ret.txHash = data(proto.out_point().tx_hash());
    ret.outputIndex = proto.out_point().output_index();
    ret.address = proto.address();
    ret.amount = proto.amount();
    for (auto i = 0; i < proto.token_amount_size(); ++i) {
        auto ta = TokenAmount::fromProto(proto.token_amount(i));
        ret.tokenBundle.add(ta);
    }
    return ret;
}

Proto::TxInput TxInput::toProto() const {
    Proto::TxInput txInput;
    txInput.mutable_out_point()->set_tx_hash(txHash.data(), txHash.size());
    txInput.mutable_out_point()->set_output_index(outputIndex);
    txInput.set_address(address.data(), address.size());
    txInput.set_amount(amount);
    for (const auto& token : tokenBundle.bundle) {
        *txInput.add_token_amount() = token.second.toProto();
    }
    return txInput;
}

TxOutput TxOutput::fromProto(const Cardano::Proto::TxOutput& proto) {
    auto ret = TxOutput();
    ret.address = data(proto.address());
    ret.amount = proto.amount();
    for (auto i = 0; i < proto.token_amount_size(); ++i) {
        auto ta = TokenAmount::fromProto(proto.token_amount(i));
        ret.tokenBundle.add(ta);
    }
    return ret;
}

Proto::TxOutput TxOutput::toProto() const {
    Proto::TxOutput txOutput;
    const auto toAddress = AddressV3(address);
    txOutput.set_address(toAddress.string());
    txOutput.set_amount(amount);
    for (const auto& token : tokenBundle.bundle) {
        *txOutput.add_token_amount() = token.second.toProto();
    }
    return txOutput;
}

bool operator==(const TxInput& i1, const TxInput& i2) {
    return i1.outputIndex == i2.outputIndex && i1.txHash == i2.txHash;
}

TransactionPlan TransactionPlan::fromProto(const Proto::TransactionPlan& proto) {
    auto ret = TransactionPlan();
    ret.availableAmount = proto.available_amount();
    ret.amount = proto.amount();
    ret.fee = proto.fee();
    ret.change = proto.change();
    ret.deposit = proto.deposit();
    ret.undeposit = proto.undeposit();
    for (auto i = 0; i < proto.available_tokens_size(); ++i) {
        ret.availableTokens.add(TokenAmount::fromProto(proto.available_tokens(i)));
    }
    for (auto i = 0; i < proto.output_tokens_size(); ++i) {
        ret.outputTokens.add(TokenAmount::fromProto(proto.output_tokens(i)));
    }
    for (auto i = 0; i < proto.change_tokens_size(); ++i) {
        ret.changeTokens.add(TokenAmount::fromProto(proto.change_tokens(i)));
    }
    for (auto i = 0; i < proto.utxos_size(); ++i) {
        ret.utxos.emplace_back(TxInput::fromProto(proto.utxos(i)));
    }
    for (auto i = 0; i < proto.extra_outputs_size(); ++i) {
        ret.extraOutputs.emplace_back(TxOutput::fromProto(proto.extra_outputs(i)));
    }
    ret.error = proto.error();
    return ret;
}

Proto::TransactionPlan TransactionPlan::toProto() const {
    Proto::TransactionPlan plan;
    plan.set_available_amount(availableAmount);
    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(change);
    plan.set_deposit(deposit);
    plan.set_undeposit(undeposit);
    for (const auto& token : availableTokens.bundle) {
        *plan.add_available_tokens() = token.second.toProto();
    }
    for (const auto& token : outputTokens.bundle) {
        *plan.add_output_tokens() = token.second.toProto();
    }
    for (const auto& token : changeTokens.bundle) {
        *plan.add_change_tokens() = token.second.toProto();
    }
    for (const auto& u : utxos) {
        *plan.add_utxos() = u.toProto();
    }
    for (const auto& u : extraOutputs) {
        *plan.add_extra_outputs() = u.toProto();
    }
    plan.set_error(error);
    return plan;
}

Cbor::Encode cborizeInputs(const std::vector<OutPoint>& inputs) {
    // clang-format off
    std::vector<Cbor::Encode> ii;
    for (const auto& i : inputs) {
        ii.emplace_back(Cbor::Encode::array({
            Cbor::Encode::bytes(i.txHash),
            Cbor::Encode::uint(i.outputIndex)
        }));
    }
    // clang-format on
    return Cbor::Encode::array(ii);
}

Cbor::Encode cborizeOutputAmounts(const Amount& amount, const TokenBundle& tokenBundle) {
    if (tokenBundle.size() == 0) {
        // native amount only
        return Cbor::Encode::uint(amount);
    }
    // native and token amounts
    // tokens: organized in two levels: by policyId and by assetName
    const auto policyIds = tokenBundle.getPolicyIds();
    std::map<Cbor::Encode, Cbor::Encode> tokensMap;
    for (const auto& policy : policyIds) {
        const auto& subTokens = tokenBundle.getByPolicyId(policy);
        std::map<Cbor::Encode, Cbor::Encode> subTokensMap;
        for (const auto& token : subTokens) {
            subTokensMap.emplace(
                Cbor::Encode::bytes(data(token.assetName)),
                Cbor::Encode::uint(uint64_t(token.amount)) // 64 bits
            );
        }
        tokensMap.emplace(
            Cbor::Encode::bytes(parse_hex(policy)),
            Cbor::Encode::map(subTokensMap));
    }
    // clang-format off
    return Cbor::Encode::array({
        Cbor::Encode::uint(amount),
        Cbor::Encode::map(tokensMap)
    });
    // clang-format on
}

Cbor::Encode cborizeOutput(const TxOutput& output) {
    // clang-format off
    return Cbor::Encode::array({
        Cbor::Encode::bytes(output.address),
        cborizeOutputAmounts(output.amount, output.tokenBundle)
    });
    // clang-format on
}

Cbor::Encode cborizeOutputs(const std::vector<TxOutput>& outputs) {
    std::vector<Cbor::Encode> oo;
    for (const auto& o : outputs) {
        oo.emplace_back(cborizeOutput(o));
    }
    return Cbor::Encode::array(oo);
}

Cbor::Encode cborizeCertificateKey(const CertificateKey& certKey) {
    std::vector<Cbor::Encode> c;
    c.emplace_back(Cbor::Encode::uint(static_cast<uint8_t>(certKey.type)));
    c.emplace_back(Cbor::Encode::bytes(certKey.key));
    return Cbor::Encode::array(c);
}

Cbor::Encode cborizeCert(const Certificate& cert) {
    std::vector<Cbor::Encode> c;
    c.emplace_back(Cbor::Encode::uint(static_cast<uint8_t>(cert.type)));
    c.emplace_back(cborizeCertificateKey(cert.certKey));
    if (!cert.poolId.empty()) {
        c.emplace_back(Cbor::Encode::bytes(cert.poolId));
    }
    return Cbor::Encode::array(c);
}

Cbor::Encode cborizeCerts(const std::vector<Certificate>& certs) {
    std::vector<Cbor::Encode> c;
    for (const auto& i : certs) {
        c.emplace_back(cborizeCert(i));
    }
    return Cbor::Encode::array(c);
}

Cbor::Encode cborizeWithdrawals(const std::vector<Withdrawal>& withdrawals) {
    std::map<Cbor::Encode, Cbor::Encode> mapElems;
    for (const auto& w : withdrawals) {
        mapElems.emplace(Cbor::Encode::bytes(w.stakingKey), Cbor::Encode::uint(w.amount));
    }
    return Cbor::Encode::map(mapElems);
}

Data Transaction::encode() const {
    const auto ii = cborizeInputs(inputs);
    const auto oo = cborizeOutputs(outputs);

    // Encode elements in a map, with fixed numbers as keys
    std::map<Cbor::Encode, Cbor::Encode> mapElems = {
        std::make_pair(Cbor::Encode::uint(0), ii),
        std::make_pair(Cbor::Encode::uint(1), oo),
        std::make_pair(Cbor::Encode::uint(2), Cbor::Encode::uint(fee)),
        std::make_pair(Cbor::Encode::uint(3), Cbor::Encode::uint(ttl)),
    };

    if (!certificates.empty()) {
        mapElems.emplace(Cbor::Encode::uint(4), cborizeCerts(certificates));
    }
    if (!withdrawals.empty()) {
        mapElems.emplace(Cbor::Encode::uint(5), cborizeWithdrawals(withdrawals));
    }

    Cbor::Encode encode = Cbor::Encode::map(mapElems);
    return encode.encoded();

    // Note: following fields are not included:
    // 7 AUXILIARY_DATA_HASH, 8 VALIDITY_INTERVAL_START
}

Data Transaction::getId() const {
    const auto encoded = encode();
    auto hash = Hash::blake2b(encoded, 32);
    return hash;
}

/// https://github.com/Emurgo/cardano-serialization-lib/blob/78184e0a2c207c2f8bba57b0d3c437f4c808c125/rust/src/utils.rs#L1415
std::optional<uint64_t> minAdaAmountHelper(const TxOutput& output, uint64_t coinsPerUtxoByte) noexcept {
    const size_t outputSize = cborizeOutput(output).encoded().size();
    const auto outputSizeExtended = static_cast<uint64_t>(outputSize + 160);
    if (checkMulUnsignedOverflow(outputSizeExtended, coinsPerUtxoByte)) {
        return std::nullopt;
    }
    return outputSizeExtended * coinsPerUtxoByte;
}

/// https://github.com/Emurgo/cardano-serialization-lib/blob/78184e0a2c207c2f8bba57b0d3c437f4c808c125/rust/src/utils.rs#L1388
std::optional<uint64_t> TxOutput::minAdaAmount(uint64_t coinsPerUtxoByte) const noexcept {
    // A copy of `this`.
    TxOutput output(address, amount, tokenBundle);

    while (true) {
        const auto minAmount = minAdaAmountHelper(output, coinsPerUtxoByte);
        if (!minAmount) {
            return std::nullopt;
        }
        if (output.amount >= *minAmount) {
            return minAmount;
        }
        // Set the amount to `minAmount` and re-try again.
        output.amount = *minAmount;
    }
}

} // namespace TW::Cardano
