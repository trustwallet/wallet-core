// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Cbor.h"
#include "Hash.h"
#include "HexCoding.h"

using namespace TW::Cardano;
using namespace TW;
using namespace std;


TokenAmount TokenAmount::fromProto(const Proto::TokenAmount& proto) {
    auto ret = TokenAmount();
    ret.policyId = proto.policy_id();
    ret.assetName = proto.asset_name();
    ret.amount = load(proto.amount());
    return ret;
}

Proto::TokenAmount TokenAmount::toProto() const {
    Proto::TokenAmount tokenAmount;
    tokenAmount.set_policy_id(policyId.data(), policyId.size());
    tokenAmount.set_asset_name(assetName.data(), assetName.size());
    const auto amountData = store(amount);
    tokenAmount.set_amount(amountData.data(), amountData.size());
    return tokenAmount;
}

TokenBundle TokenBundle::fromProto(const Proto::TokenBundle& proto) {
    auto ret = TokenBundle();
    for (auto i = 0; i < proto.token_size(); ++i) {
        ret.add(TokenAmount::fromProto(proto.token(i)));
    }
    return ret;
}

Proto::TokenBundle TokenBundle::toProto() const {
    Proto::TokenBundle proto;
    for (const auto& t: bundle) {
        *(proto.add_token()) = t.second.toProto();
    }
    return proto;
}

void TokenBundle::add(const TokenAmount& ta) {
    const auto key = ta.key();
    if (bundle.find(key) == bundle.end()) {
        bundle[key] = ta;
    } else {
        auto entry = bundle[key];
        entry.amount += ta.amount;
        bundle[key] = entry;
    }
}

uint256_t TokenBundle::getAmount(const std::string& key) const {
    const auto& findkey = bundle.find(key);
    if (findkey == bundle.end()) {
        return 0;
    }
    return findkey->second.amount;
}

unordered_set<string> TokenBundle::getPolicyIds() const {
    unordered_set<string> policyIds;
    std::transform(bundle.cbegin(), bundle.cend(),
                   std::inserter(policyIds, policyIds.begin()),
                   [](auto&& cur){ return cur.second.policyId; });
    return policyIds;
}

vector<TokenAmount> TokenBundle::getByPolicyId(const string& policyId) const {
    vector<TokenAmount> filtered;
    for (const auto& t: bundle) {
        if (t.second.policyId == policyId) {
            filtered.push_back(t.second);
        }
    }
    return filtered;
}

uint64_t roundupBytesToWords(uint64_t b) { return ((b + 7) / 8); }

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
    return max(MinUtxoValue, (MinUtxoValue / adaOnlyUTxOSize) * (utxoEntrySizeWithoutVal + sizeB));
}

uint64_t TokenBundle::minAdaAmount() const {
    if (size() == 0) {
        // ADA only
        return MinUtxoValue;
    }

    unordered_set<string> policyIdRegistry;
    unordered_set<string> assetNameRegistry;
    uint64_t numPids = 0;
    uint64_t numAssets = 0;
    uint64_t sumAssetNameLengths = 0;
    for (const auto& t: bundle) {
        policyIdRegistry.emplace(t.second.policyId);
        if (t.second.assetName.length() > 0) {
            assetNameRegistry.emplace(t.second.assetName);
        }
    }
    numPids = uint64_t(policyIdRegistry.size());
    numAssets = uint64_t(assetNameRegistry.size());
    for_each(assetNameRegistry.begin(), assetNameRegistry.end(), [&sumAssetNameLengths](string a){ sumAssetNameLengths += a.length(); });
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
    for (const auto& token: tokenBundle.bundle) {
        *txInput.add_token_amount() = token.second.toProto();
    }
    return txInput;
}

bool TW::Cardano::operator==(const TxInput& i1, const TxInput& i2) { return i1.outputIndex == i2.outputIndex && i1.txHash == i2.txHash; }

TransactionPlan TransactionPlan::fromProto(const Proto::TransactionPlan& proto) {
    auto ret = TransactionPlan();
    ret.availableAmount = proto.available_amount();
    ret.amount = proto.amount();
    ret.fee = proto.fee();
    ret.change = proto.change();
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
        ret.utxos.push_back(TxInput::fromProto(proto.utxos(i)));
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
    for (const auto& token: availableTokens.bundle) {
        *plan.add_available_tokens() = token.second.toProto();
    }
    for (const auto& token: outputTokens.bundle) {
        *plan.add_output_tokens() = token.second.toProto();
    }
    for (const auto& token: changeTokens.bundle) {
        *plan.add_change_tokens() = token.second.toProto();
    }
    for (const auto& u: utxos) {
        *plan.add_utxos() = u.toProto();
    }
    plan.set_error(error);
    return plan;
}

Cbor::Encode cborizeInputs(const std::vector<OutPoint>& inputs) {
    std::vector<Cbor::Encode> ii;
    for (const auto& i: inputs) {
        ii.push_back(Cbor::Encode::array({
            Cbor::Encode::bytes(i.txHash),
            Cbor::Encode::uint(i.outputIndex)
        }));
    }
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
    map<Cbor::Encode, Cbor::Encode> tokensMap;
    for (const auto& policy: policyIds) {
        const auto& subTokens = tokenBundle.getByPolicyId(policy);
        map<Cbor::Encode, Cbor::Encode> subTokensMap;
        for (const auto& token: subTokens) {
            subTokensMap.emplace(
                Cbor::Encode::bytes(data(token.assetName)),
                Cbor::Encode::uint(uint64_t(token.amount)) // 64 bits
            );
        }
        tokensMap.emplace(
            Cbor::Encode::bytes(parse_hex(policy)),
            Cbor::Encode::map(subTokensMap)
        );
    }
    return Cbor::Encode::array({
        Cbor::Encode::uint(amount),
        Cbor::Encode::map(tokensMap)
    });
}

Cbor::Encode cborizeOutput(const TxOutput& output) {
    return Cbor::Encode::array({
        Cbor::Encode::bytes(output.address),
        cborizeOutputAmounts(output.amount, output.tokenBundle)
    });
}

Cbor::Encode cborizeOutputs(const std::vector<TxOutput>& outputs) {
    std::vector<Cbor::Encode> oo;
    for (const auto& o: outputs) {
        oo.push_back(cborizeOutput(o));
    }
    return Cbor::Encode::array(oo);
}

Cbor::Encode cborizeCertKey(const CertKey& certKey) {
    std::vector<Cbor::Encode> c;
    c.push_back(Cbor::Encode::uint(certKey.type));
    c.push_back(Cbor::Encode::bytes(certKey.key));
    return Cbor::Encode::array(c);
}

Cbor::Encode cborizeCert(const Certificate& cert) {    
    std::vector<Cbor::Encode> c;
    c.push_back(Cbor::Encode::uint(cert.type));
    c.push_back(cborizeCertKey(cert.certKey));
    if (cert.poolId.size() > 0) {
        c.push_back(Cbor::Encode::bytes(cert.poolId));
    }
    return Cbor::Encode::array(c);
}

Cbor::Encode cborizeCerts(const std::vector<Certificate>& certs) {    
    std::vector<Cbor::Encode> c;
    for (const auto& i: certs) {
        c.push_back(cborizeCert(i));
    }
    return Cbor::Encode::array(c);
}

Data Transaction::encode() const {
    const auto ii = cborizeInputs(inputs);
    const auto oo = cborizeOutputs(outputs);

    // Encode elements in a map, with fixed numbers as keys
    std::map<Cbor::Encode, Cbor::Encode> mapElems = {
        make_pair(Cbor::Encode::uint(0), ii),
        make_pair(Cbor::Encode::uint(1), oo),
        make_pair(Cbor::Encode::uint(2), Cbor::Encode::uint(fee)),
        make_pair(Cbor::Encode::uint(3), Cbor::Encode::uint(ttl)),
    };

    if (certificates.size() > 0) {
        mapElems.emplace(make_pair(Cbor::Encode::uint(4), cborizeCerts(certificates)));
    }

    Cbor::Encode encode = Cbor::Encode::map(mapElems);
    return encode.encoded();

    // Note: following fields are not included:
    // 5 withdrawals, 7 AUXILIARY_DATA_HASH, 8 VALIDITY_INTERVAL_START
}

Data Transaction::getId() const {
    const auto encoded = encode();
    const auto hash = Hash::blake2b(encoded, 32);
    return hash;
}
