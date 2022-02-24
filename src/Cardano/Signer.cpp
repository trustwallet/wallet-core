// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "AddressV3.h"

#include "PrivateKey.h"

#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace TW::Cardano;
using namespace TW;
using namespace std;


Signer::Signer(const Proto::SigningInput& input) : input(input) {
    // derive sender address
    const auto privateKeyData = data(input.private_key());
    if (PrivateKey::isValid(privateKeyData)) {
        const auto privateKey = PrivateKey(privateKeyData);
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        const auto address = AddressV3(publicKey);
        senderAddress = address.data();
    }
}

Proto::SigningOutput Signer::sign() {
    // plan if needed
    if (input.has_plan()) {
        _plan = TransactionPlan::fromProto(input.plan());
    } else {
        // no plan supplied, plan it
        _plan = plan();
    }

    return signWithPlan();
}

Common::Proto::SigningError Signer::buildTransaction(Transaction& tx, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& senderAddress) {
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
        if (senderAddress.size() == 0) {
            if (input.private_key().length() == 0) {
                return Common::Proto::Error_missing_private_key;
            }
            return Common::Proto::Error_invalid_address;
        }
        tx.outputs.push_back(TxOutput{
            senderAddress,
            plan.change
        });
    }
    tx.fee = plan.fee;
    tx.ttl = input.ttl();

    return Common::Proto::OK;
}

Proto::SigningOutput Signer::signWithPlan() {
    auto ret = Proto::SigningOutput();
    if (_plan.error != Common::Proto::OK) {
        // plan has error
        ret.set_error(_plan.error);
        return ret;
    }

    Transaction tx;
    const auto buildRet = buildTransaction(tx, input, _plan, senderAddress);
    if (buildRet != Common::Proto::OK) {
        ret.set_error(buildRet);
        return ret;
    }

    const auto encoded = tx.encode();
    ret.set_encoded(string(encoded.begin(), encoded.end()));
    const auto txid = tx.getId();
    ret.set_tx_id(string(txid.begin(), txid.end()));
    ret.set_error(Common::Proto::OK);

    return ret;
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
uint64_t estimateTxSize(const Proto::SigningInput& input, Amount amount, const Data& senderAddress) {
    auto inputs = vector<TxInput>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        inputs.push_back(TxInput::fromProto(input.utxos(i)));
    }
    const auto _simplePlan = simplePlan(inputs, amount);

    Transaction simpleTx;
    Signer::buildTransaction(simpleTx, input, _simplePlan, senderAddress);

    auto encodedSize = simpleTx.encode().size();
    const auto extra = 11;
    return encodedSize + extra;
}

Amount txFeeFunction(uint64_t txSizeInBytes) {
    const auto a = 155381;
    const double b = 43.946;

    const Amount fee = (Amount)(ceil(a + txSizeInBytes * b));
    return fee;
}

Amount Signer::estimateFee(const Proto::SigningInput& input, Amount amount, const Data& senderAddress) {
    return txFeeFunction(estimateTxSize(input, amount, senderAddress));
}

TransactionPlan Signer::plan() const {
    auto plan = TransactionPlan();

    plan.amount = input.transfer_message().amount();
    if (plan.amount == 0) {
        plan.error = Common::Proto::Error_zero_amount_requested;
        return plan;
    }
    assert(plan.amount > 0);

    if (input.utxos_size() == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    auto utxos = vector<TxInput>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        utxos.push_back(TxInput::fromProto(input.utxos(i)));
    }
    // select UTXOs, aim for 1.5x of target
    plan.utxos = selectInputsSimple(utxos, plan.amount * 3 / 2 + 1);
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

    plan.fee = estimateFee(input, plan.amount, senderAddress);

    if (plan.amount + plan.fee > plan.availableAmount) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount + plan.fee <= plan.availableAmount);

    plan.change = plan.availableAmount - (plan.amount + plan.fee);

    return plan;
}
