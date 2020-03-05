// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Staking.h"

#include "Ethereum/ABI/Function.h"
#include "Data.h"
#include "uint256.h"

namespace TW::IoTeX {

using namespace TW::Ethereum::ABI;

void stakingStake(const Data& candidate, uint64_t stakeDuration, bool nonDecay, const Data& dataIn, Data& dataOut) {
    Function func("createPygg");
    func.addInParam(std::make_shared<ParamByteArrayFix>(12, candidate));
    func.addInParam(std::make_shared<ParamUInt256>(uint256_t(stakeDuration)));
    func.addInParam(std::make_shared<ParamBool>(nonDecay));
    func.addInParam(std::make_shared<ParamByteArray>(dataIn));

    func.encode(dataOut);
}

void stakingUnstake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
    Function func("unstake");
    func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
    func.addInParam(std::make_shared<ParamByteArray>(dataIn));

    func.encode(dataOut);
}

void stakingWithdraw(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
    Function func("withdraw");
    func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
    func.addInParam(std::make_shared<ParamByteArray>(dataIn));

    func.encode(dataOut);
}

void stakingAddStake(uint64_t pyggIndex, const Data& dataIn, Data& dataOut) {
    Function func("storeToPygg");
    func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
    func.addInParam(std::make_shared<ParamByteArray>(dataIn));

    func.encode(dataOut);
}

void stakingMoveStake(uint64_t pyggIndex, const Data& candidate, const Data& dataIn, Data& dataOut) {
    Function func("revote");
    func.addInParam(std::make_shared<ParamUInt256>(uint256_t(pyggIndex)));
    func.addInParam(std::make_shared<ParamByteArrayFix>(12, candidate));
    func.addInParam(std::make_shared<ParamByteArray>(dataIn));

    func.encode(dataOut);
}

} // namespace TW::IoTeX
