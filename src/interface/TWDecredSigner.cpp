// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bitcoin/TransactionBuilder.h"
#include "../Data.h"
#include "../Decred/Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Common.pb.h"
#include "../proto/Decred.pb.h"

#include <TrustWalletCore/TWDecredSigner.h>
#include <algorithm>

using namespace TW;
using namespace TW::Decred;

struct TWDecredSigner *_Nonnull TWDecredSignerCreate(TW_Bitcoin_Proto_SigningInput data) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWDecredSigner{ Signer(std::move(input)) };
}

struct TWDecredSigner *_Nonnull TWDecredSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Bitcoin::Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWDecredSigner{ Signer(std::move(input), std::move(plan)) };
}

void TWDecredSignerDelete(struct TWDecredSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWDecredSignerPlan(struct TWDecredSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Decred_Proto_SigningOutput TWDecredSignerSign(struct TWDecredSigner *_Nonnull signer) {
    auto result = signer->impl.sign();
    auto output = Decred::Proto::SigningOutput();
    if (!result) {
        output.set_error(result.error());
        auto serialized = output.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    }

    const auto& tx = result.payload();

    *output.mutable_transaction() = tx.proto();

    TW::Data encoded;
    tx.encode(encoded);
    output.set_encoded(encoded.data(), encoded.size());

    auto txHash = TW::Hash::blake256(encoded);
    output.set_transaction_id(TW::hex(txHash));

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
