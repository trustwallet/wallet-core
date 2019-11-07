// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonySigner.h>

#include "../Harmony/Signer.h"
#include "../proto/Harmony.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Harmony;

template <typename T>
TW_Harmony_Proto_SigningOutput TWHarmonySignerSign(TW_Harmony_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign<T>(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}

TW_Harmony_Proto_SigningOutput TWHarmonySignerSignTransaction(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Transaction>(input);
}

TW_Harmony_Proto_SigningOutput
TWHarmonySignerSignStakingCreateValidator(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Staking<CreateValidator>>(input);
}

TW_Harmony_Proto_SigningOutput
TWHarmonySignerSignStakingEditValidator(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Staking<EditValidator>>(input);
}

TW_Harmony_Proto_SigningOutput
TWHarmonySignerSignStakingDelegate(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Staking<Delegate>>(input);
}

TW_Harmony_Proto_SigningOutput
TWHarmonySignerSignStakingUndelegate(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Staking<Undelegate>>(input);
}

TW_Harmony_Proto_SigningOutput
TWHarmonySignerSignStakingCollectRewards(TW_Harmony_Proto_SigningInput input) {
    return TWHarmonySignerSign<Staking<CollectRewards>>(input);
}
