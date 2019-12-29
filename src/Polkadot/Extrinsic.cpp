// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Extrinsic.h"

using namespace TW;
using namespace TW::Polkadot;

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;

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
    // call index from MetadataV10
    Data data;
    if (input.has_balance_call()) {
        auto transfer = input.balance_call().transfer();
        auto address = SS58Address(transfer.to_address(), byte(input.network()));
        auto value = load(transfer.value());
        // call index
        append(data, {0x04, 0x00});
        // destination
        append(data, encodeAddress(address));
        // value
        append(data, encodeCompact(value));
    } else if (input.has_staking_call()) {
        auto staking = input.staking_call();
        if (staking.has_bond()) {
            auto address = SS58Address(staking.bond().validator(), byte(input.network()));
            auto value = load(staking.bond().value());
            auto reward = byte(staking.bond().reward_destination());
            // call index
            append(data, {0x06, 0x00});
            // validator
            append(data, encodeAddress(address));
            // value
            append(data, encodeCompact(value));
            // reward destination
            append(data, reward);
        } else if (staking.has_bond_extra()) {
            auto value = load(staking.unbond().value());
            // call index
            append(data, {0x06, 0x01});
            // value
            append(data, encodeCompact(value));
        } else if (staking.has_unbond()) {
            auto value = load(staking.unbond().value());
            // call index
            append(data, {0x06, 0x02});
            // value
            append(data, encodeCompact(value));
        } else if (staking.has_withdraw_unbonded()) {
            // call index
            append(data, {0x06, 0x03});
        } else if (staking.has_nominate()) {
            std::vector<SS58Address> addresses;
            for (auto& n : staking.nominate().nominators()) {
                addresses.push_back(SS58Address(n, byte(input.network())));
            }
            // call index
            append(data, {0x06, 0x05});
            // nominators
            append(data, encodeAddresses(addresses));
        } else if (staking.has_chill()) {
            // call index
            append(data, {0x06, 0x06});
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
    // genesis hash
    append(data, genesisHash);
    // block hash
    append(data, blockHash);
    return data;
}

Data Extrinsic::encodeSignature(const PublicKey& signer, const Data& signature) const {
    Data data;
    // version header
    append(data, Data{ static_cast<uint8_t>(version | signedBit)});
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
