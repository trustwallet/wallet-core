// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyStakingSigner.h>

#include "../Harmony/StakingSigner.h"
#include "../proto/Harmony.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Harmony;

template <typename Directive>
TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSign(TW_Harmony_Proto_StakingTransactionInput data) {
    Proto::StakingTransactionInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = StakingSigner(load(input.chain_id()));
    auto protoOutput = signer.sign<Directive>(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}

TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSignCreateValidator(TW_Harmony_Proto_StakingTransactionInput input) {
    return TWHarmonyStakingSignerSign<CreateValidator>(input);
}

TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSignEditValidator(TW_Harmony_Proto_StakingTransactionInput input) {
    return TWHarmonyStakingSignerSign<EditValidator>(input);
}

TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSignDelegate(TW_Harmony_Proto_StakingTransactionInput input) {
    return TWHarmonyStakingSignerSign<Delegate>(input);
}

TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSignUndelegate(TW_Harmony_Proto_StakingTransactionInput input) {
    return TWHarmonyStakingSignerSign<Undelegate>(input);
}

TW_Harmony_Proto_StakingTransactionOutput
TWHarmonyStakingSignerSignCollectRewards(TW_Harmony_Proto_StakingTransactionInput input) {
    return TWHarmonyStakingSignerSign<CollectRewards>(input);
}