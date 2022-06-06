// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "AddressV3.h"

#include "PrivateKey.h"
#include "Cbor.h"
#include "HexCoding.h"

#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace TW::Cardano;
using namespace TW;
using namespace std;


static const Data placeholderPrivateKey = parse_hex("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
static const auto PlaceholderFee = 170000;
static const auto ExtraInputAmount = 500000;

Proto::SigningOutput Signer::sign() {
    // plan if needed
    if (input.has_plan()) {
        _plan = TransactionPlan::fromProto(input.plan());
    } else {
        // no plan supplied, plan it
        _plan = doPlan();
    }

    return signWithPlan();
}

Common::Proto::SigningError Signer::buildTransactionAux(Transaction& tx, const Proto::SigningInput& input, const TransactionPlan& plan) {
    tx = Transaction();
    for (const auto& i: plan.utxos) {
        tx.inputs.emplace_back(i.txHash, i.outputIndex);
    }

    // Spending output
    if (!AddressV3::isValidLegacy(input.transfer_message().to_address())) {
        return Common::Proto::Error_invalid_address;
    }
    const auto toAddress = AddressV3(input.transfer_message().to_address());
    tx.outputs.emplace_back(toAddress.data(), plan.amount, plan.outputTokens);
    // Change
    bool hasChangeToken = any_of(plan.changeTokens.bundle.begin(), plan.changeTokens.bundle.end(), [](auto&& t) { return t.second.amount > 0; });
    if (plan.change > 0 || hasChangeToken) {
        if (!AddressV3::isValidLegacy(input.transfer_message().change_address())) {
            return Common::Proto::Error_invalid_address;
        }
        const auto changeAddress = AddressV3(input.transfer_message().change_address());
        tx.outputs.emplace_back(changeAddress.data(), plan.change, plan.changeTokens);
    }
    tx.fee = plan.fee;
    tx.ttl = input.ttl();

    return Common::Proto::OK;
}

Common::Proto::SigningError Signer::assembleSignatures(vector<pair<Data, Data>>& signatures, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& txId, bool sizeEstimationOnly) {
    signatures.clear();
    // Private keys and corresponding addresses
    map<string, Data> privateKeys;
    for (auto i = 0; i < input.private_key_size(); ++i) {
        const auto privateKeyData = data(input.private_key(i));
        if (!PrivateKey::isValid(privateKeyData)) {
            return Common::Proto::Error_invalid_private_key;
        }
        const auto privateKey = PrivateKey(privateKeyData);
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        const auto address = AddressV3(publicKey);
        privateKeys[address.string()] = privateKeyData;
    }

    // collect every unique input UTXO address
    vector<string> addresses;
    for (auto& u: plan.utxos) {
        if (!AddressV3::isValid(u.address)) {
            return Common::Proto::Error_invalid_address;
        }
        if (find(addresses.begin(), addresses.end(), u.address) == addresses.end()) {
            addresses.push_back(u.address);
        }
    }

    // create signature for each address
    for (auto& a: addresses) {
        const auto privKeyFind = privateKeys.find(a);
        Data privateKeyData;
        if (privKeyFind != privateKeys.end()) {
            privateKeyData = privKeyFind->second;
        } else {
            // private key not found
            if (sizeEstimationOnly) {
                privateKeyData = placeholderPrivateKey;
            } else {
                return Common::Proto::Error_missing_private_key;
            }
        }
        const auto privateKey = PrivateKey(privateKeyData);
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        const auto signature = privateKey.sign(txId, TWCurveED25519Extended);
        // public key (first 32 bytes) and signature (64 bytes)
        signatures.emplace_back(subData(publicKey.bytes, 0, 32), signature);
    }

    return Common::Proto::OK;
}

Cbor::Encode cborizeSignatures(const vector<pair<Data, Data>>& signatures) {
    // signatures as Cbor
    vector<Cbor::Encode> sigsCbor;
    for (auto& s: signatures) {
        sigsCbor.emplace_back(Cbor::Encode::array({
            Cbor::Encode::bytes(s.first),
            Cbor::Encode::bytes(s.second)
        }));
    }

    // Cbor-encode txAux & signatures
    return Cbor::Encode::map({
        make_pair(
            Cbor::Encode::uint(0),
            Cbor::Encode::array(sigsCbor)
        )
    });
}

Proto::SigningOutput Signer::signWithPlan() {
    auto ret = Proto::SigningOutput();
    if (_plan.error != Common::Proto::OK) {
        // plan has error
        ret.set_error(_plan.error);
        return ret;
    }

    Data encoded;
    Data txId;
    const auto buildRet = encodeTransaction(encoded, txId, input, _plan);
    if (buildRet != Common::Proto::OK) {
        ret.set_error(buildRet);
        return ret;
    }

    ret.set_encoded(string(encoded.begin(), encoded.end()));
    ret.set_tx_id(string(txId.begin(), txId.end()));
    ret.set_error(Common::Proto::OK);

    return ret;
}

Common::Proto::SigningError Signer::encodeTransaction(Data& encoded, Data& txId, const Proto::SigningInput& input, const TransactionPlan& plan, bool sizeEstimationOnly) {
    if (plan.error != Common::Proto::OK) {
        return plan.error;
    }

    Transaction txAux;
    const auto buildRet = buildTransactionAux(txAux, input, plan);
    if (buildRet != Common::Proto::OK) {
        return buildRet;
    }
    txId = txAux.getId();

    vector<pair<Data, Data>> signatures;
    const auto sigError = assembleSignatures(signatures, input, plan, txId, sizeEstimationOnly);
    if (sigError != Common::Proto::OK) {
        return sigError;
    }
    const auto sigsCbor = cborizeSignatures(signatures);

    // Cbor-encode txAux & signatures
    const auto cbor = Cbor::Encode::array({
        // txaux
        Cbor::Encode::fromRaw(txAux.encode()),
        // signatures
        sigsCbor,
        // aux data
        Cbor::Encode::null(),
    });
    encoded = cbor.encoded();
    return Common::Proto::OK;
}

// Select a subset of inputs, to cover desired coin amount. Simple algorithm: pick largest ones.
vector<TxInput> selectInputsSimpleNative(const vector<TxInput>& inputs, Amount amount) {
    auto ii = vector<TxInput>(inputs);
    sort(ii.begin(), ii.end(), [](TxInput t1, TxInput t2) {
        return t1.amount > t2.amount;
    });
    auto selected = vector<TxInput>();
    Amount selectedAmount = 0;

    for (const auto& i: ii) {
        selected.push_back(i);
        selectedAmount += i.amount;
        if (selectedAmount >= amount) {
            break;
        }
    }
    return selected;
}

// Select a subset of inputs, to cover desired token amount. Simple algorithm: pick largest ones.
void selectInputsSimpleToken(const vector<TxInput>& inputs, string key, uint256_t amount, vector<TxInput>& selectedInputs) {
    uint256_t selectedAmount = std::accumulate(selectedInputs.begin(), selectedInputs.end(), uint256_t(0), [key](uint256_t sum, const TxInput& si){ return si.tokenBundle.getAmount(key); });
    if (selectedAmount >= amount) {
        return; // already covered
    }
    // sort inputs descending
    auto ii = vector<TxInput>(inputs);
    std::sort(ii.begin(), ii.end(), [key](TxInput t1, TxInput t2) { return t1.tokenBundle.getAmount(key) > t2.tokenBundle.getAmount(key); });
    for (const auto& i: ii) {
        if (distance(selectedInputs.begin(), find(selectedInputs.begin(), selectedInputs.end(), i)) < selectedInputs.size()) {
            // already selected
            continue;
        }
        selectedInputs.push_back(i);
        selectedAmount += i.amount;
        if (selectedAmount >= amount) {
            return;
        }
    }
    // not enough
}

// Select a subset of inputs, to cover desired amount. Simple algorithm: pick largest ones
vector<TxInput> Signer::selectInputsWithTokens(const vector<TxInput>& inputs, Amount amount, const TokenBundle& requestedTokens) {
    auto selected = selectInputsSimpleNative(inputs, amount);
    for (auto iter = requestedTokens.bundle.begin(); iter != requestedTokens.bundle.end(); ++iter) {
        const auto& ta = iter->second;
        selectInputsSimpleToken(inputs, ta.key(), ta.amount, selected);
    }
    return selected;
}

// Create a simple plan, used for estimation
TransactionPlan simplePlan(Amount amount, const TokenBundle& requestedTokens, const vector<TxInput>& selectedInputs, bool maxAmount) {
    TransactionPlan plan;
    plan.amount = amount;
    plan.utxos = selectedInputs;
    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u: plan.utxos) {
        plan.availableAmount += u.amount;
        for (auto iter = u.tokenBundle.bundle.begin(); iter != u.tokenBundle.bundle.end(); ++iter) {
            plan.availableTokens.add(iter->second);
        }
    }
    plan.fee = PlaceholderFee; // placeholder value
    // adjust/compute output amount and output tokens
    if (!maxAmount) {
        // reduce amount if needed
        plan.amount = max(Amount(0), min(plan.amount, plan.availableAmount - plan.fee));
        plan.outputTokens = requestedTokens;
    } else {
        // max available amount
        plan.amount = max(Amount(0), plan.availableAmount - plan.fee);
        plan.outputTokens = plan.availableTokens; // use all
    }

    // compute change
    plan.change = plan.availableAmount - (plan.amount + plan.fee);
    for (auto iter = plan.availableTokens.bundle.begin(); iter != plan.availableTokens.bundle.end(); ++iter) {
        const auto key = iter->second.key();
        const auto changeAmount = iter->second.amount - plan.outputTokens.getAmount(key);
        assert(changeAmount >= 0);
        plan.changeTokens.bundle[key] = iter->second;
        plan.changeTokens.bundle[key].amount = changeAmount;
    }
    return plan;
}

// Estimates size of transaction in bytes.
uint64_t estimateTxSize(const Proto::SigningInput& input, Amount amount, const TokenBundle& requestedTokens, const vector<TxInput>& selectedInputs) {
    auto inputs = vector<TxInput>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        inputs.emplace_back(TxInput::fromProto(input.utxos(i)));
    }
    const auto _simplePlan = simplePlan(amount, requestedTokens, selectedInputs, input.transfer_message().use_max_amount());

    Data encoded;
    Data txId;
    const auto encodeError = Signer::encodeTransaction(encoded, txId, input, _simplePlan, true);
    if (encodeError != Common::Proto::OK) {
        return 0;
    }

    return encoded.size();
}

// Compute fee from tx size, with some over-estimation
Amount txFeeFunction(uint64_t txSizeInBytes) {
    const double fixedTerm = 155381 + 500;
    const double linearTerm = 43.946 + 0.1;

    const Amount fee = (Amount)(ceil(fixedTerm + (double)txSizeInBytes * linearTerm));
    return fee;
}

Amount Signer::estimateFee(const Proto::SigningInput& input, Amount amount, const TokenBundle& requestedTokens, const vector<TxInput> selectedInputs) {
    return txFeeFunction(estimateTxSize(input, amount, requestedTokens, selectedInputs));
}

TransactionPlan Signer::doPlan() const {
    auto plan = TransactionPlan();

    bool maxAmount = input.transfer_message().use_max_amount();
    if (input.transfer_message().amount() == 0 && !maxAmount && input.transfer_message().token_amount().token_size() == 0) {
        plan.error = Common::Proto::Error_zero_amount_requested;
        return plan;
    }
    // Check input UTXOs, process, sum ADA and token amounts
    auto utxos = vector<TxInput>();
    uint64_t inputSum = 0;
    for (auto i = 0; i < input.utxos_size(); ++i) {
        const auto& utxo = input.utxos(i);
        utxos.emplace_back(TxInput::fromProto(utxo));
        inputSum += utxo.amount();
    }
    if (inputSum == 0 || input.utxos_size() == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(inputSum > 0);

    // Amounts requested
    plan.amount = input.transfer_message().amount();
    TokenBundle requestedTokens;
    for (auto i = 0; i < input.transfer_message().token_amount().token_size(); ++i) {
        const auto token = TokenAmount::fromProto(input.transfer_message().token_amount().token(i));
        requestedTokens.add(token);
    }
    assert(plan.amount > 0 || maxAmount);
    if (requestedTokens.size() > 1) {
        // We support transfer of only one coin (for simplicity; inputs may contain more coints which are preserved)
        plan.error = Common::Proto::Error_invalid_requested_token_amount;
        return plan;
    }

    // if amount requested is the same or more than available amount, it cannot be satisifed, but
    // treat this case as MaxAmount, and send maximum available (which will be less)
    if (!maxAmount && input.transfer_message().amount() >= inputSum) {
        maxAmount = true;
    }

    // select UTXOs
    if (!maxAmount) {
        // aim for larger total input, enough for 4/3 of the target amount plus typical fee plus minimal ADA for change plus some extra
        auto targetInputAmount = (plan.amount * 4) / 3 + PlaceholderFee + requestedTokens.minAdaAmount() + ExtraInputAmount;
        plan.utxos = selectInputsWithTokens(utxos, targetInputAmount, requestedTokens);
    } else {
        // maxAmount, select all
        plan.utxos = utxos;
    }
    assert(plan.utxos.size() > 0);

    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u: plan.utxos) {
        plan.availableAmount += u.amount;
        for (auto iter = u.tokenBundle.bundle.begin(); iter != u.tokenBundle.bundle.end(); ++iter) {
            plan.availableTokens.add(iter->second);
        }
    }
    if (plan.availableAmount == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(plan.availableAmount > 0);

    // check that there are enough coins in the inputs
    if (plan.amount > plan.availableAmount) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount <= plan.availableAmount);
    // check that there are enough tokens in the inputs
    for (auto iter = requestedTokens.bundle.begin(); iter != requestedTokens.bundle.end(); ++iter) {
        if (iter->second.amount > plan.availableTokens.getAmount(iter->second.key())) {
            plan.error = Common::Proto::Error_low_balance;
            return plan;
        }
    }

    // compute fee
    if (input.transfer_message().force_fee() == 0) {
        plan.fee = estimateFee(input, plan.amount, requestedTokens, plan.utxos);
    } else {
        // fee provided, use it (capped)
        plan.fee = max(Amount(0), min(plan.availableAmount - plan.amount, input.transfer_message().force_fee()));
    }
    assert(plan.fee >= 0 && plan.fee < plan.availableAmount);

    // adjust/compute output amount
    if (!maxAmount) {
        // reduce amount if needed
        plan.amount = max(Amount(0), min(plan.amount, plan.availableAmount - plan.fee));
    } else {
        // max available amount
        plan.amount = max(Amount(0), plan.availableAmount - plan.fee);
    }
    assert(plan.amount >= 0 && plan.amount <= plan.availableAmount);

    if (plan.amount + plan.fee > plan.availableAmount) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount + plan.fee <= plan.availableAmount);

    // compute output token amounts
    if (!maxAmount) {
        plan.outputTokens = requestedTokens;
    } else {
        plan.outputTokens = plan.availableTokens; // send all
    }

    // compute change
    plan.change = plan.availableAmount - (plan.amount + plan.fee);
    for (auto iter = plan.availableTokens.bundle.begin(); iter != plan.availableTokens.bundle.end(); ++iter) {
        const auto key = iter->second.key();
        const auto changeAmount = iter->second.amount - plan.outputTokens.getAmount(key);
        if (changeAmount > 0) { // omit 0-amount tokens
            plan.changeTokens.bundle[key] = iter->second;
            plan.changeTokens.bundle[key].amount = changeAmount;
        }
    }

    assert(plan.change >= 0 && plan.change <= plan.availableAmount);
    assert(!maxAmount || plan.change == 0); // change is 0 in max amount case
    assert(plan.amount + plan.change + plan.fee == plan.availableAmount);

    return plan;
}
