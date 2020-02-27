// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXStaking.h>

#include "IoTeX/Staking.h"
#include "Data.h"

using namespace TW;

TWData* _Nonnull TWIoTeXStakingStake(TWData* _Nonnull candidate, uint64_t stakeDuration, bool nonDecay, TWData* _Nonnull data) {
    Data encoded;
    TW::IoTeX::stakingStake(*reinterpret_cast<const Data*>(candidate), stakeDuration, nonDecay, *reinterpret_cast<const Data*>(data), encoded);
    return TWDataCreateWithData(&encoded);
}

TWData* _Nonnull TWIoTeXStakingUnstake(uint64_t pyggIndex, TWData* _Nonnull data) {
    Data encoded;
    TW::IoTeX::stakingUnstake(pyggIndex, *reinterpret_cast<const Data*>(data), encoded);
    return TWDataCreateWithData(&encoded);
}

TWData* _Nonnull TWIoTeXStakingWithdraw(uint64_t pyggIndex, TWData* _Nonnull data) {
    Data encoded;
    TW::IoTeX::stakingWithdraw(pyggIndex, *reinterpret_cast<const Data*>(data), encoded);
    return TWDataCreateWithData(&encoded);
}

TWData* _Nonnull TWIoTeXStakingAddStake(uint64_t pyggIndex, TWData* _Nonnull data) {
    Data encoded;
    TW::IoTeX::stakingAddStake(pyggIndex, *reinterpret_cast<const Data*>(data), encoded);
    return TWDataCreateWithData(&encoded);
}

TWData* _Nonnull TWIoTeXStakingMoveStake(uint64_t pyggIndex, TWData* _Nonnull candidate, TWData* _Nonnull data) {
    Data encoded;
    TW::IoTeX::stakingMoveStake(pyggIndex, *reinterpret_cast<const Data*>(candidate), *reinterpret_cast<const Data*>(data), encoded);
    return TWDataCreateWithData(&encoded);
}
