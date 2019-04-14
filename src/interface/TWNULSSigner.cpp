// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNULSSigner.h>

#include "../NULS/Signer.h"
#include "../NULS/TransactionBuilder.h"
#include "../PrivateKey.h"
#include "../proto/NULS.pb.h"

using namespace TW;
using namespace TW::NULS;

TW_NULS_Proto_TransactionPlan TWNULSSignerPlan(TW_NULS_Proto_TransactionPurpose data) {
    Proto::TransactionPurpose purpose;
    purpose.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    auto plan = TransactionBuilder::plan(purpose);
    std::vector<uint8_t> serializeData(plan.ByteSize());
    plan.SerializeToArray(serializeData.data(), plan.ByteSize());

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serializeData.data()),
                                 serializeData.size());
}

TW_NULS_Proto_SigningOutput TWNULSSignerSign(TW_NULS_Proto_TransactionPlan data) {
    Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    const auto signer = Signer(plan);
    const auto output = signer.sign();

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(output.data()), output.size());
}
