// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Extrinsic.h"
#include <TrustWalletCore/TWSS58AddressType.h>
#include <map>

using namespace TW;
using namespace TW::Polkadot;

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint8_t extrinsicFormat = 4;

static const std::string balanceTransfer = "Balances.transfer";
static const std::string stakingBond = "Staking.bond";
static const std::string stakingBondExtra = "Staking.bond_extra";
static const std::string stakingUnbond = "Staking.unbond";
static const std::string stakingWithdrawUnbond = "Staking.withdraw_unbonded";
static const std::string stakingNominate = "Staking.nominate";
static const std::string stakingChill = "Staking.chill";

static std::map<const std::string, Data> polkadotCallIndices = {
    {balanceTransfer,       Data{0x05, 0x00}},
    {stakingBond,           Data{0x07, 0x00}},
    {stakingBondExtra,      Data{0x07, 0x01}},
    {stakingUnbond,         Data{0x07, 0x02}},
    {stakingWithdrawUnbond, Data{0x07, 0x03}},
    {stakingNominate,       Data{0x07, 0x05}},
    {stakingChill,          Data{0x07, 0x06}},
};

static std::map<const std::string, Data> kusamaCallIndices = {
    {balanceTransfer,       Data{0x04, 0x00}},
    {stakingBond,           Data{0x06, 0x00}},
    {stakingBondExtra,      Data{0x06, 0x01}},
    {stakingUnbond,         Data{0x06, 0x02}},
    {stakingWithdrawUnbond, Data{0x06, 0x03}},
    {stakingNominate,       Data{0x06, 0x05}},
    {stakingChill,          Data{0x06, 0x06}},
};

static Data getCallIndex(TWSS58AddressType network, const std::string& key) {
    switch (network) {
    case TWSS58AddressTypePolkadot:
        return polkadotCallIndices[key];
    case TWSS58AddressTypeKusama:
        return kusamaCallIndices[key];
    }
}

Data Extrinsic::encodeEraNonceTip() const {
    Data data;
    // immortal era
    append(data, era);
    // nonce
    append(data, encodeCompact(nonce));
    // tip
    append(data, encodeCompact(tip));
    return data;
}

Data Extrinsic::encodeCall(const Proto::SigningInput& input) {
    // call index from MetadataV11
    Data data;
    auto network = TWSS58AddressType(input.network());
    if (input.has_balance_call()) {
        auto transfer = input.balance_call().transfer();
        auto address = SS58Address(transfer.to_address(), network);
        auto value = load(transfer.value());
        // call index
        append(data, getCallIndex(network, balanceTransfer));
        // destination
        append(data, encodeAddress(address));
        // value
        append(data, encodeCompact(value));
    } else if (input.has_staking_call()) {
        auto staking = input.staking_call();
        if (staking.has_bond()) {
            auto address = SS58Address(staking.bond().controller(), byte(input.network()));
            auto value = load(staking.bond().value());
            auto reward = byte(staking.bond().reward_destination());
            // call index
            append(data, getCallIndex(network, stakingBond));
            // controller
            append(data, encodeAddress(address));
            // value
            append(data, encodeCompact(value));
            // reward destination
            append(data, reward);
        } else if (staking.has_bond_extra()) {
            auto value = load(staking.unbond().value());
            // call index
            append(data, getCallIndex(network, stakingBondExtra));
            // value
            append(data, encodeCompact(value));
        } else if (staking.has_unbond()) {
            auto value = load(staking.unbond().value());
            // call index
            append(data, getCallIndex(network, stakingUnbond));
            // value
            append(data, encodeCompact(value));
        } else if (staking.has_withdraw_unbonded()) {
            auto spans = staking.withdraw_unbonded().slashing_spans();
            // call index
            append(data, getCallIndex(network, stakingWithdrawUnbond));
            // num_slashing_spans
            encode32LE(spans, data);
        } else if (staking.has_nominate()) {
            std::vector<SS58Address> addresses;
            for (auto& n : staking.nominate().nominators()) {
                addresses.push_back(SS58Address(n, network));
            }
            // call index
            append(data, getCallIndex(network, stakingNominate));
            // nominators
            append(data, encodeAddresses(addresses));
        } else if (staking.has_chill()) {
            // call index
            append(data, getCallIndex(network, stakingChill));
        }
    }
    return data;
}

Data Extrinsic::encodePayload() const {
    Data data;
    // call
    append(data, call);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // specVersion
    encode32LE(specVersion, data);
    // transactionVersion
    encode32LE(version, data);
    // genesis hash
    append(data, genesisHash);
    // block hash
    append(data, blockHash);
    return data;
}

Data Extrinsic::encodeSignature(const PublicKey& signer, const Data& signature) const {
    Data data;
    // version header
    append(data, Data{extrinsicFormat | signedBit});
    // signer public key
    append(data, encodeAddress(signer));
    // signature type
    append(data, sigTypeEd25519);
    // signature
    append(data, signature);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // call
    append(data, call);
    // append length
    encodeLengthPrefix(data);
    return data;
}
