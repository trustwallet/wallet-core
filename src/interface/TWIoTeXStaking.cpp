// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiEncoder.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>
#include <TrustWalletCore/TWIoTeXStaking.h>

#include "Data.h"
#include "uint256.h"

using namespace TW;

TWData* _Nonnull TWIoTeXStakingStake(TWData* _Nonnull candidate, uint64_t stakeDuration, bool nonDecay, TWData* _Nonnull data) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("createPygg"));
    assert(func != nullptr);

    TWEthereumAbiFunctionAddParamBytesFix(func, 12, candidate, false);
    auto v = store(uint256_t(stakeDuration));
    TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    TWEthereumAbiFunctionAddParamBool(func, nonDecay, false);
    TWEthereumAbiFunctionAddParamBytes(func, data, false);

    auto ret = TWEthereumAbiEncoderEncode(func);
    TWEthereumAbiEncoderDeleteFunction(func);
    return ret;
}

/// Function to generate Unstake message
TWData* _Nonnull TWIoTeXStakingUnstake(uint64_t pyggIndex, TWData* _Nonnull data) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("unstake"));
    assert(func != nullptr);

    auto v = store(uint256_t(pyggIndex));
    TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    TWEthereumAbiFunctionAddParamBytes(func, data, false);

    auto ret = TWEthereumAbiEncoderEncode(func);
    TWEthereumAbiEncoderDeleteFunction(func);
    return ret;
}

/// Function to generate Withdraw message
TWData* _Nonnull TWIoTeXStakingWithdraw(uint64_t pyggIndex, TWData* _Nonnull data) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("withdraw"));
    assert(func != nullptr);

    auto v = store(uint256_t(pyggIndex));
    TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    TWEthereumAbiFunctionAddParamBytes(func, data, false);

    auto ret = TWEthereumAbiEncoderEncode(func);
    TWEthereumAbiEncoderDeleteFunction(func);
    return ret;
}

/// Function to generate AddStake message
TWData* _Nonnull TWIoTeXStakingAddStake(uint64_t pyggIndex, TWData* _Nonnull data) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("storeToPygg"));
    assert(func != nullptr);

    auto v = store(uint256_t(pyggIndex));
    TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    TWEthereumAbiFunctionAddParamBytes(func, data, false);

    auto ret = TWEthereumAbiEncoderEncode(func);
    TWEthereumAbiEncoderDeleteFunction(func);
    return ret;
}

/// Function to generate MoveStake message
TWData* _Nonnull TWIoTeXStakingMoveStake(uint64_t pyggIndex, TWData* _Nonnull candidate, TWData* _Nonnull data) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("revote"));
    assert(func != nullptr);

    auto v = store(uint256_t(pyggIndex));
    TWEthereumAbiFunctionAddParamUInt256(func, &v, false);
    TWEthereumAbiFunctionAddParamBytesFix(func, 12, candidate, false);
    TWEthereumAbiFunctionAddParamBytes(func, data, false);

    auto ret = TWEthereumAbiEncoderEncode(func);
    TWEthereumAbiEncoderDeleteFunction(func);
    return ret;
}
