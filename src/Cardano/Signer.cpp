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

using namespace TW::Cardano;
using namespace TW;
using namespace std;


static const Data placeholderPrivateKey = parse_hex("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");

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
        tx.inputs.push_back(OutPoint{i.txHash, i.outputIndex});
    }

    // Spending output
    if (!AddressV3::isValid(input.transfer_message().to_address())) {
        return Common::Proto::Error_invalid_address;
    }
    const auto toAddress = AddressV3(input.transfer_message().to_address());
    tx.outputs.push_back(TxOutput{
        toAddress.data(),
        plan.amount
    });
    // Change
    if (plan.change > 0) {
        if (!AddressV3::isValid(input.transfer_message().change_address())) {
            return Common::Proto::Error_invalid_address;
        }
        const auto changeAddress = AddressV3(input.transfer_message().change_address());
        tx.outputs.push_back(TxOutput{
            changeAddress.data(),
            plan.change
        });
    }
    tx.fee = plan.fee;
    tx.ttl = input.ttl();

    return Common::Proto::OK;
}

Common::Proto::SigningError Signer::assembleSignatures(std::vector<std::pair<Data, Data>>& signatures, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& txId, bool sizeEstimationOnly) {
    signatures.clear();
    // Private keys and corresponding addresses
    std::map<std::string, Data> privateKeys;
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
    std::vector<std::string> addresses;
    for (auto& u: plan.utxos) {
        if (!AddressV3::isValid(u.address)) {
            return Common::Proto::Error_invalid_address;
        }
        if (std::find(addresses.begin(), addresses.end(), u.address) == addresses.end()) {
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
        signatures.push_back(std::make_pair(subData(publicKey.bytes, 0, 32), signature));
    }

    return Common::Proto::OK;
}

Cbor::Encode cborizeSignatures(const std::vector<std::pair<Data, Data>>& signatures) {
    // signatures as Cbor
    std::vector<Cbor::Encode> sigsCbor;
    for (auto& s: signatures) {
        sigsCbor.push_back(Cbor::Encode::array({
            Cbor::Encode::bytes(s.first),
            Cbor::Encode::bytes(s.second)
        }));
    }

    // Cbor-encode txAux & signatures
    return Cbor::Encode::map({
        std::make_pair(
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

    std::vector<std::pair<Data, Data>> signatures;
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

// Select a subset of inputs, to cover desired amount. Simple algorithm: pick largest ones
vector<TxInput> Signer::selectInputsSimple(const vector<TxInput>& inputs, Amount amount) {
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

// Create a simple plan, used for estimation
TransactionPlan simplePlan(const vector<TxInput>& inputs, Amount amount) {
    TransactionPlan plan;
    plan.amount = amount;
    plan.utxos = Signer::selectInputsSimple(inputs, amount);
    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u: plan.utxos) {
        plan.availableAmount += u.amount;
    }
    plan.fee = 100000; // placeholder value
    plan.change = plan.availableAmount - plan.amount;
    return plan;
}

// Estimates size of transaction in bytes.
uint64_t estimateTxSize(const Proto::SigningInput& input, Amount amount) {
    auto inputs = vector<TxInput>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        inputs.push_back(TxInput::fromProto(input.utxos(i)));
    }
    const auto _simplePlan = simplePlan(inputs, amount);

    Data encoded;
    Data txId;
    const auto encodeError = Signer::encodeTransaction(encoded, txId, input, _simplePlan, true);
    if (encodeError != Common::Proto::OK) {
        return 0;
    }

    auto encodedSize = encoded.size();
    const auto extra = 11;
    return encodedSize + extra;
}

Amount txFeeFunction(uint64_t txSizeInBytes) {
    const double fixedTerm = 155381;
    const double linearTerm = 43.946;

    const Amount fee = (Amount)(ceil(fixedTerm + (double)txSizeInBytes * linearTerm));
    return fee;
}

Amount Signer::estimateFee(const Proto::SigningInput& input, Amount amount) {
    return txFeeFunction(estimateTxSize(input, amount));
}

TransactionPlan Signer::doPlan() const {
    auto plan = TransactionPlan();

    bool maxAmount = input.transfer_message().use_max_amount();
    if (input.transfer_message().amount() == 0 && !maxAmount) {
        plan.error = Common::Proto::Error_zero_amount_requested;
        return plan;
    }
    // Check input UTXOs, process, sum ADA amounts
    auto utxos = vector<TxInput>();
    uint64_t inputSum = 0;
    for (auto i = 0; i < input.utxos_size(); ++i) {
        if (input.utxos(i).token_amount_size() > 0) {
            // input UTXO with token amount
            plan.error = Common::Proto::Error_invalid_utxo_amount;
            return plan;
        }
        utxos.push_back(TxInput::fromProto(input.utxos(i)));
        inputSum += input.utxos(i).amount();
    }
    if (inputSum == 0 || input.utxos_size() == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(inputSum > 0);

    // select UTXOs
    plan.amount = input.transfer_message().amount();
    assert(plan.amount > 0 || maxAmount);

    // if amount requested is the same or more than available amount, it cannot be satisifed, but
    // treat this case as MaxAmount, and send maximum available (which will be less)
    if (!maxAmount && input.transfer_message().amount() >= inputSum) {
        maxAmount = true;
    }

    // select UTXOs
    if (!maxAmount) {
        // aim for 1.5x of target
        plan.utxos = selectInputsSimple(utxos, plan.amount * 3 / 2 + 1);
    } else {
        // maxAmount, select all
        plan.utxos = utxos;
    }
    assert(plan.utxos.size() > 0);

    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u: plan.utxos) {
        plan.availableAmount += u.amount;
    }
    if (plan.availableAmount == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(plan.availableAmount > 0);

    if (plan.amount > plan.availableAmount) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount <= plan.availableAmount);

    plan.fee = estimateFee(input, plan.amount);

    // adjust/compute amount
    if (!maxAmount) {
        // reduce amount if needed
        plan.amount = std::max(Amount(0), std::min(plan.amount, plan.availableAmount - plan.fee));
    } else {
        // max available amount
        plan.amount = std::max(Amount(0), plan.availableAmount - plan.fee);
    }
    assert(plan.amount >= 0 && plan.amount <= plan.availableAmount);

    if (plan.amount + plan.fee > plan.availableAmount) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount + plan.fee <= plan.availableAmount);

    // compute change
    plan.change = plan.availableAmount - (plan.amount + plan.fee);

    assert(plan.change >= 0 && plan.change <= plan.availableAmount);
    assert(!maxAmount || plan.change == 0); // change is 0 in max amount case
    assert(plan.amount + plan.change + plan.fee == plan.availableAmount);

    return plan;
}
