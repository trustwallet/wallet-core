// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Staking.h"
#include "Data.h"
#include "HexCoding.h"
using namespace TW;

namespace TW::IoTeX {

const char* FromData(const Data& data) {
    auto s = new std::string(data.begin(), data.end());
    s->append(data.size(), '\0');
    auto ss = reinterpret_cast<const std::string*>(s);
    return ss->data();
}

Data dataFromString(const std::string& d) {
    Data data;
    std::copy(d.c_str(), d.c_str() + d.length(), back_inserter(data));
    return data;
}

Data stakingCreate(const Data& candidate, const Data& amount, uint32_t duration, bool autoStake,
                   const Data& payload) {
    auto action = IoTeX::Proto::Staking_Create();
    action.set_candidatename(FromData(candidate));
    action.set_stakedamount(FromData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingAddDeposit(uint64_t index, const Data& amount, const Data& payload) {
    auto action = IoTeX::Proto::Staking_AddDeposit();
    action.set_bucketindex(index);
    action.set_amount(FromData(amount));
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingUnstake(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_Reclaim();
    action.set_bucketindex(index);
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingWithdraw(uint64_t index, const Data& payload) {
    auto action = IoTeX::Proto::Staking_Reclaim();
    action.set_bucketindex(index);
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingRestake(uint64_t index, uint32_t duration, bool autoStake, const Data& payload) {
    auto action = IoTeX::Proto::Staking_Restake();
    action.set_bucketindex(index);
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingChangeCandidate(uint64_t index, const Data& candidate, const Data& payload) {
    auto action = IoTeX::Proto::Staking_ChangeCandidate();
    action.set_bucketindex(index);
    action.set_candidatename(FromData(candidate));
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data stakingTransfer(uint64_t index, const Data& voterAddress, const Data& payload) {
    auto action = IoTeX::Proto::Staking_TransferOwnership();
    action.set_bucketindex(index);
    action.set_voteraddress(FromData(voterAddress));
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data candidateRegister(const Data& name, const Data& operatorAddress, const Data& rewardAddress,
                       const Data& amount, uint32_t duration, bool autoStake,
                       const Data& ownerAddress, const Data& payload) {
    auto cbi = new IoTeX::Proto::Staking_CandidateBasicInfo();
    cbi->set_name(FromData(name));
    cbi->set_operatoraddress(FromData(operatorAddress));
    cbi->set_rewardaddress(FromData(rewardAddress));

    auto action = IoTeX::Proto::Staking_CandidateRegister();
    action.set_allocated_candidate(cbi);
    action.set_stakedamount(FromData(amount));
    action.set_stakedduration(duration);
    action.set_autostake(autoStake);
    action.set_owneraddress(FromData(ownerAddress));
    action.set_payload(FromData(payload));
    return dataFromString(action.SerializeAsString());
}

Data candidateUpdate(const Data& name, const Data& operatorAddress, const Data& rewardAddress) {
    auto action = IoTeX::Proto::Staking_CandidateBasicInfo();
    action.set_name(FromData(name));
    action.set_operatoraddress(FromData(operatorAddress));
    action.set_rewardaddress(FromData(rewardAddress));
    return dataFromString(action.SerializeAsString());
}
} // namespace TW::IoTeX
