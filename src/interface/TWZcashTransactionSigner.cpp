// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionSigner.h"
#include "../Data.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Common.pb.h"
#include "../Zcash/Transaction.h"

#include <TrustWalletCore/TWZcashTransactionSigner.h>

using namespace TW;
using namespace TW::Zcash;

struct TWZcashTransactionSigner *_Nonnull TWZcashTransactionSignerCreate(TW_Bitcoin_Proto_SigningInput data) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWZcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction>(std::move(input)) };
}

struct TWZcashTransactionSigner *_Nonnull TWZcashTransactionSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Bitcoin::Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWZcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction>(std::move(input), std::move(plan)) };
}

void TWZcashTransactionSignerDelete(struct TWZcashTransactionSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWZcashTransactionSignerPlan(struct TWZcashTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWZcashTransactionSignerSign(struct TWZcashTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.sign();
    auto protoResult = TW::Proto::Result();
    if (!result) {
        protoResult.set_success(false);
        protoResult.set_error(result.error());
        auto serialized = protoResult.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    }

    const auto& tx = result.payload();
    auto protoOutput = Bitcoin::Proto::SigningOutput();
    *protoOutput.mutable_transaction() = tx.proto();

    TW::Data encoded;
    tx.encode(encoded);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    protoResult.set_success(true);
    protoResult.add_objects()->PackFrom(protoOutput);

    auto serialized = protoResult.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWZcashTransactionSignerMessage(TW_Bitcoin_Proto_SigningInput data) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWZcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction>(std::move(input)) };
    for (auto i = 0; i < signer->impl.plan.utxos.size(); i++) {
        auto& utxo = signer->impl.plan.utxos[i];
        auto script = Bitcoin::Script(utxo.script().begin(), utxo.script().end());

        if (i >= signer->impl.transaction.inputs.size()) {
            // Check if inputs consistent with utxos,
            // if not, we return null in case of app crash.
            return nullptr;
        }

        auto preImage = signer->impl.transaction.getPreImage(script, i,
                                                            static_cast<TWBitcoinSigHashType>(input.hash_type()), utxo.amount());

        signer->impl.plan.utxos[i].set_script(reinterpret_cast<const uint8_t *>(preImage.data()), preImage.size());
    }

    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWZcashTransactionSignerTransaction(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    try {
        Bitcoin::Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
        Bitcoin::Proto::TransactionPlan plan;
        plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));

        auto signer = new TWZcashTransactionSigner{
            Bitcoin::TransactionSigner<Transaction>(std::move(input), plan)};

        for (auto i = 0; i < signer->impl.transaction.inputs.size(); i++) {
            for (auto j = 0; j < plan.utxos().size(); j++) {
                auto planOutput = Bitcoin::OutPoint(plan.utxos()[j].out_point());
                if (signer->impl.transaction.inputs[i].previousOutput == planOutput) {
                    signer->impl.transaction.inputs[i].script = Bitcoin::Script(
                        plan.utxos()[j].script().begin(), plan.utxos()[j].script().end());
                }
            }
        }

        const auto& tx = signer->impl.transaction;
        auto protoOutput = Bitcoin::Proto::SigningOutput();
        *protoOutput.mutable_transaction() = tx.proto();

        TW::Data encoded;
        tx.encode(encoded);
        protoOutput.set_encoded(encoded.data(), encoded.size());

        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}
