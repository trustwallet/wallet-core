// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinTransactionSigner.h>

#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionSigner.h"

#include "../Data.h"
#include "../proto/Common.pb.h"
#include "../proto/Bitcoin.pb.h"

using namespace TW::Bitcoin;

struct TWBitcoinTransactionSigner *_Nonnull TWBitcoinTransactionSignerCreate(TW_Bitcoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));
    return new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input)) };
}

void TWBitcoinTransactionSignerDelete(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWBitcoinTransactionSignerPlan(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWBitcoinTransactionSignerSign(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.sign();
    auto protoResult = TW::Proto::Result();
    if (!result) {
        protoResult.set_success(false);
        protoResult.set_error(result.error());
        auto serialized = protoResult.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    }

    const auto& tx = result.payload();
    auto protoOutput = Proto::SigningOutput();
    *protoOutput.mutable_transaction() = tx.proto();

    TW::Data encoded;
    auto hasWitness = std::any_of(tx.inputs.begin(), tx.inputs.end(), [](auto& input) { return !input.scriptWitness.empty(); });
    tx.encode(hasWitness, encoded);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    protoResult.set_success(true);
    protoResult.add_objects()->PackFrom(protoOutput);

    auto serialized = protoResult.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
