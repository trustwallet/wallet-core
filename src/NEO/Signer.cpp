// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "InvocationTransaction.h"
#include "Script.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../proto/Common.pb.h"
#include "../proto/NEO.pb.h"

using namespace std;
using namespace TW;

namespace TW::NEO {

Signer::Signer(const PrivateKey& priKey)
    : privateKey(std::move(priKey)) {
    auto pub = privateKey.getPublicKey(TWPublicKeyTypeNIST256p1);
    publicKey = pub.bytes;
    address = Address(pub);
}

PrivateKey Signer::getPrivateKey() const {
    return privateKey;
}

TW::PublicKey Signer::getPublicKey() const {
    return TW::PublicKey(publicKey, TWPublicKeyTypeNIST256p1);
}

Address Signer::getAddress() const {
    return address;
}

void Signer::sign(Transaction& tx) const {
    auto unsignedTx = tx.serialize();
    auto signature = sign(unsignedTx);
    tx.witnesses.clear();
    Witness witness;
    witness.invocationScript = Script::CreateInvocationScript(signature);
    witness.verificationScript = Script::CreateSignatureRedeemScript(publicKey);
    tx.witnesses.push_back(witness);
}

Data Signer::sign(const Data& data) const {
    auto signature = getPrivateKey().sign(TW::Hash::sha256(data), TWCurveNIST256p1);
    signature.pop_back();
    return signature;
}

Proto::TransactionPlan Signer::plan(const Proto::SigningInput& input) {
    Proto::TransactionPlan plan;
    try {
        std::map<std::string, int64_t> available;
        std::map<std::string, int64_t> required;

        for (int i = 0; i < input.outputs_size(); i++) {
            required[input.outputs(i).asset_id()] = input.outputs(i).amount();
            for (int j = 0; j < input.outputs(i).extra_outputs_size(); j++) {
                required[input.outputs(i).asset_id()] += input.outputs(i).extra_outputs(j).amount();
            }
        }

        for (int i = 0; i < input.inputs_size(); i++) {
            if (available.find(input.inputs(i).asset_id()) == available.end()) {
                available[input.inputs(i).asset_id()] = input.inputs(i).value();
                plan.add_inputs()->MergeFrom(input.inputs(i));
                continue;
            }

            // if the required has been enough, not need to add input
            if (input.inputs(i).asset_id() != input.gas_asset_id() &&
                required[input.inputs(i).asset_id()] < available[input.inputs(i).asset_id()]) {
                continue;
            }

            available[input.inputs(i).asset_id()] += input.inputs(i).value();
            plan.add_inputs()->MergeFrom(input.inputs(i));
        }

        int existGASTransfer = -1;
        for (int i = 0; i < input.outputs_size(); i++) {
            auto* outputPlan = plan.add_outputs();

            if (available.find(input.outputs(i).asset_id()) == available.end() ||
                available[input.outputs(i).asset_id()] < required[input.outputs(i).asset_id()]) {
                throw Common::Proto::SigningError(Common::Proto::Error_low_balance);
            }

            if (input.outputs(i).asset_id() == input.gas_asset_id()) {
                existGASTransfer = i;
            }

            int64_t availableAmount = available[input.outputs(i).asset_id()];
            outputPlan->set_available_amount(availableAmount);
            outputPlan->set_amount(input.outputs(i).amount());

            outputPlan->set_to_address(input.outputs(i).to_address());
            outputPlan->set_asset_id(input.outputs(i).asset_id());
            outputPlan->set_change_address(input.outputs(i).change_address());

            auto changeAmount = availableAmount - input.outputs(i).amount();
            for (int j = 0; j < input.outputs(i).extra_outputs_size(); j++) {
                auto* extra_plan = outputPlan->add_extra_outputs();

                extra_plan->set_to_address(input.outputs(i).extra_outputs(j).to_address());
                extra_plan->set_amount(input.outputs(i).extra_outputs(j).amount());
                changeAmount -= input.outputs(i).extra_outputs(j).amount();
            }
            outputPlan->set_change(changeAmount);
        }

        const int64_t SIGNATURE_SIZE = 103;
        int64_t transactionSize =
            prepareUnsignedTransaction(input, plan, false)->serialize().size() + SIGNATURE_SIZE;

        const int64_t LARGE_TX_SIZE = 1024;
        const int64_t MIN_FEE_FOR_LARGE_TX = 100000;
        const int64_t FEE_FOR_BYTE_OF_TX = 1000;

        bool feeNeed = input.fee() > 0;
        if (transactionSize >= LARGE_TX_SIZE) {
            feeNeed = true;
        }
        if (feeNeed && existGASTransfer < 0) {
            auto* outputPlan = plan.add_outputs();
            existGASTransfer = plan.outputs_size() - 1;

            if (available.find(input.gas_asset_id()) == available.end() ||
                available[input.gas_asset_id()] < 1024) {
                throw Common::Proto::SigningError(Common::Proto::Error_tx_too_big);
            }

            int64_t availableAmount = available[input.gas_asset_id()];
            outputPlan->set_available_amount(availableAmount);
            outputPlan->set_amount(0);
            outputPlan->set_change(availableAmount);

            outputPlan->set_to_address(input.gas_change_address());
            outputPlan->set_change_address(input.gas_change_address());
            outputPlan->set_asset_id(input.gas_asset_id());
        }

        if (feeNeed) {
            transactionSize =
                prepareUnsignedTransaction(input, plan, false)->serialize().size() + SIGNATURE_SIZE;
            int64_t fee = 0;
            if (transactionSize >= LARGE_TX_SIZE) {
                fee = MIN_FEE_FOR_LARGE_TX;
                fee += transactionSize * FEE_FOR_BYTE_OF_TX;
            }
            fee = input.fee() < fee ? fee : input.fee();
            int64_t change = plan.outputs(existGASTransfer).change() - fee;
            plan.mutable_outputs(existGASTransfer)->set_change(change);
            plan.set_fee(fee);
        }
    } catch (const Common::Proto::SigningError& error) {
        plan.set_error(error);
    }

    return plan;
}

std::shared_ptr<Transaction> Signer::prepareUnsignedTransaction(const Proto::SigningInput& input,
                                                                const Proto::TransactionPlan& plan,
                                                                bool validate) {
    std::shared_ptr<Transaction> transaction;
    try {
        switch (input.transaction().transaction_oneof_case()) {
        case Proto::Transaction::kNep5Transfer: {
            auto t = std::make_shared<InvocationTransaction>();
            auto nep5Tx = input.transaction().nep5_transfer();
            t->script = Script::CreateNep5TransferScript(
                parse_hex(nep5Tx.asset_id()), Address(nep5Tx.from()).toScriptHash(),
                Address(nep5Tx.to()).toScriptHash(), load(nep5Tx.amount()), nep5Tx.script_with_ret());

            transaction = t;
            break;
        }
        case Proto::Transaction::kInvocationGeneric: {
            auto t = std::make_shared<InvocationTransaction>();
            auto script = input.transaction().invocation_generic().script();
            t->script = Data(script.begin(), script.end());
            t->gas = input.transaction().invocation_generic().gas();

            transaction = t;
            break;
        }
        default:
            transaction = std::make_shared<Transaction>();
            break;
        }

        for (int i = 0; i < plan.inputs_size(); i++) {
            CoinReference coin;
            Data prevHashReverse(plan.inputs(i).prev_hash().begin(),
                                 plan.inputs(i).prev_hash().end());
            std::reverse(prevHashReverse.begin(), prevHashReverse.end());
            coin.prevHash = load(prevHashReverse);
            coin.prevIndex = (uint16_t)plan.inputs(i).prev_index();
            transaction->inInputs.push_back(coin);
        }

        for (int i = 0; i < plan.outputs_size(); i++) {
            if (plan.outputs(i).asset_id() == input.gas_asset_id()) {
                if (validate) {
                    auto sumAmount = plan.outputs(i).amount() + plan.outputs(i).change() + plan.fee();
                    for (int j = 0; j < plan.outputs(i).extra_outputs_size(); j++) {
                        sumAmount += plan.outputs(i).extra_outputs(j).amount();
                    }
                    if (sumAmount != plan.outputs(i).available_amount()) {
                        throw Common::Proto::SigningError(Common::Proto::Error_wrong_fee);
                    }
                }
            }

            if (plan.outputs(i).amount() > 0) { // to recipient, 0 if gas change only exist
                TransactionOutput out;
                out.assetId = load(parse_hex(plan.outputs(i).asset_id()));
                out.value = (int64_t)plan.outputs(i).amount();
                auto scriptHash = TW::NEO::Address(plan.outputs(i).to_address()).toScriptHash();
                out.scriptHash = load(scriptHash);
                transaction->outputs.push_back(out);

                for (int j = 0; j < plan.outputs(i).extra_outputs_size(); j++) {
                    TransactionOutput extraOut;
                    extraOut.assetId = load(parse_hex(plan.outputs(i).asset_id()));
                    extraOut.value = (int64_t)plan.outputs(i).extra_outputs(j).amount();
                    auto extraScriptHash = TW::NEO::Address(plan.outputs(i).extra_outputs(j).to_address()).toScriptHash();
                    extraOut.scriptHash = load(extraScriptHash);
                    transaction->outputs.push_back(extraOut);
                }
            }

            // change
            if (plan.outputs(i).change() > 0) {
                TransactionOutput out;
                out.assetId = load(parse_hex(plan.outputs(i).asset_id()));
                out.value = plan.outputs(i).change();
                auto scriptHash = TW::NEO::Address(plan.outputs(i).change_address()).toScriptHash();
                out.scriptHash = load(scriptHash);
                transaction->outputs.push_back(out);
            }
        }

        for (int i = 0; i < plan.attributes_size(); i++) {
            TransactionAttribute attr;
            attr.usage = (TransactionAttributeUsage)plan.attributes(i).usage();
            attr._data.assign(plan.attributes(i).data().begin(), plan.attributes(i).data().end());

            transaction->attributes.push_back(attr);
        }
        return transaction;
    } catch (...) {
    }

    return transaction;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    try {
        auto signer =
            Signer(PrivateKey(Data(input.private_key().begin(), input.private_key().end())));
        Proto::TransactionPlan plan;
        if (input.has_plan()) {
            plan = input.plan();
        } else {
            plan = signer.plan(input);
        }
        auto transaction = prepareUnsignedTransaction(input, plan);
        signer.sign(*transaction);
        auto signedTx = transaction->serialize();

        output.set_encoded(signedTx.data(), signedTx.size());
    } catch (const Common::Proto::SigningError& error) {
        output.set_error(error);
    }

    return output;
}

Data Signer::signaturePreimage(const Proto::SigningInput& input) {
    Proto::TransactionPlan p;
    if (input.has_plan()) {
        p = input.plan();
    } else {
        p = plan(input);
    }
    auto transaction = prepareUnsignedTransaction(input, p);
    return transaction->serialize();
}

Data Signer::encodeTransaction(const Proto::SigningInput& input,
                               const std::vector<PublicKey>& publicKeys,
                               const std::vector<Data>& signatures) {
    Proto::TransactionPlan p;
    if (input.has_plan()) {
        p = input.plan();
    } else {
        p = plan(input);
    }
    auto transaction = prepareUnsignedTransaction(input, p);
    transaction->witnesses.clear();

    if (publicKeys.size() != signatures.size()) {
        return {Data()};
    }

    for (size_t i = 0; i < publicKeys.size(); i++) {
        Witness witness;
        witness.invocationScript = Script::CreateInvocationScript(signatures[i]);
        witness.verificationScript = Script::CreateSignatureRedeemScript(publicKeys[i].bytes);
        transaction->witnesses.push_back(witness);
    }

    return transaction->serialize();
}

} // namespace TW::NEO
