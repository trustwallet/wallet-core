// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Peercoin/TransactionBuilder.h"
#include "../Peercoin/TransactionSigner.h"
#include "../Data.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Peercoin.pb.h"
#include "../proto/Common.pb.h"

#include <TrustWalletCore/TWPeercoinTransactionSigner.h>

#include <algorithm>

using namespace TW::Peercoin;

struct TWPeercoinTransactionSigner *_Nonnull TWPeercoinTransactionSignerCreate(TW_Peercoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWPeercoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input)) };
}

struct TWPeercoinTransactionSigner *_Nonnull TWPeercoinTransactionSignerCreateWithPlan(TW_Peercoin_Proto_SigningInput data, TW_Peercoin_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWPeercoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input), std::move(plan)) };
}

void TWPeercoinTransactionSignerDelete(struct TWPeercoinTransactionSigner *_Nonnull signer) {
    delete signer;
}

TW_Peercoin_Proto_TransactionPlan TWPeercoinTransactionSignerPlan(struct TWPeercoinTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWPeercoinTransactionSignerSign(struct TWPeercoinTransactionSigner *_Nonnull signer) {
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

    TW::Data txHashData = encoded;
    if (hasWitness) {
        txHashData.clear();
        tx.encode(false, txHashData);
    }
    auto txHash = TW::Hash::sha256(TW::Hash::sha256(txHashData));
    std::reverse(txHash.begin(), txHash.end());
    protoOutput.set_transaction_id(TW::hex(txHash));

    protoResult.set_success(true);
    protoResult.add_objects()->PackFrom(protoOutput);

    auto serialized = protoResult.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
