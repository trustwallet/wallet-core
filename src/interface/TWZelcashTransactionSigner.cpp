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
#include "../Zelcash/Transaction.h"

#include <TrustWalletCore/TWZelcashTransactionSigner.h>

using namespace TW;
using namespace TW::Zelcash;

struct TWZelcashTransactionSigner *_Nonnull TWZelcashTransactionSignerCreate(TW_Bitcoin_Proto_SigningInput data) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWZelcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction>(std::move(input)) };
}

struct TWZelcashTransactionSigner *_Nonnull TWZelcashTransactionSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Bitcoin::Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWZelcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction>(std::move(input), std::move(plan)) };
}

void TWZelcashTransactionSignerDelete(struct TWZelcashTransactionSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWZelcashTransactionSignerPlan(struct TWZelcashTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWZelcashTransactionSignerSign(struct TWZelcashTransactionSigner *_Nonnull signer) {
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
