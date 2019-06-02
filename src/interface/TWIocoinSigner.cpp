// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Iocoin/TransactionBuilder.h"
#include "../Iocoin/TransactionSigner.h"
#include "../Iocoin/Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Iocoin.pb.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Common.pb.h"

#include <TrustWalletCore/TWIocoinSigner.h>

#include <algorithm>

using namespace TW::Bitcoin;

struct TWIocoinSigner *_Nonnull TWIocoinSignerCreate(TW_Iocoin_Proto_SigningInput data) {
	Bitcoin::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWIocoinSigner{ TransactionSigner<TW::Bitcoin::Transaction>(std::move(input)) };
}

void TWIocoinSignerDelete(struct TWIocoinSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWIocoinSignerPlan(struct TWIocoinSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWIocoinSignerSign(struct TWIocoinSigner *_Nonnull signer) {
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

    TW::Data txHashData = encoded;
    auto txHash = TW::Hash::sha256(txHashData);
    std::reverse(txHash.begin(), txHash.end());
    protoOutput.set_transaction_id(TW::hex(txHash));

    protoResult.set_success(true);
    protoResult.add_objects()->PackFrom(protoOutput);

    auto serialized = protoResult.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
