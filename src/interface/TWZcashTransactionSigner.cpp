// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Zcash/TransactionBuilder.h"
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
    return new TWZcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction, TransactionBuilder>(std::move(input)) };
}

struct TWZcashTransactionSigner *_Nonnull TWZcashTransactionSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Bitcoin::Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWZcashTransactionSigner{ Bitcoin::TransactionSigner<Transaction, TransactionBuilder>(std::move(input), std::move(plan)) };
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
