// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Extrinsic.h"
#include <TrustWalletCore/TWSS58AddressType.h>
#include <map>

namespace TW::Polkadot {

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint8_t extrinsicFormat = 4;
static constexpr uint32_t multiAddrSpecVersion = 28;
static constexpr uint32_t multiAddrSpecVersionKsm = 2028;

static const std::string balanceTransfer = "Balances.transfer";
static const std::string utilityBatch = "Utility.batch_all";
static const std::string stakingBond = "Staking.bond";
static const std::string stakingBondExtra = "Staking.bond_extra";
static const std::string stakingUnbond = "Staking.unbond";
static const std::string stakingWithdrawUnbond = "Staking.withdraw_unbonded";
static const std::string stakingNominate = "Staking.nominate";
static const std::string stakingChill = "Staking.chill";
static const std::string stakingReBond = "Staking.rebond";

// Non-existent modules and methods on Polkadot and Kusama chains:
static const std::string assetsTransfer = "Assets.transfer";
static const std::string joinIdentityAsKey = "Identity.join_identity_as_key";
static const std::string identityAddAuthorization = "Identity.add_authorization";

// Readable decoded call index can be found from https://polkascan.io
const static std::map<const std::string, Data> polkadotCallIndices = {
    {balanceTransfer, Data{0x05, 0x00}},
    {stakingBond, Data{0x07, 0x00}},
    {stakingBondExtra, Data{0x07, 0x01}},
    {stakingUnbond, Data{0x07, 0x02}},
    {stakingWithdrawUnbond, Data{0x07, 0x03}},
    {stakingNominate, Data{0x07, 0x05}},
    {stakingChill, Data{0x07, 0x06}},
    {utilityBatch, Data{0x1a, 0x02}},
    {stakingReBond, Data{0x07, 0x13}},
};

// Default Kusama call indices.
const static std::map<const std::string, Data> kusamaCallIndices = {
    {balanceTransfer, Data{0x04, 0x00}},
    {stakingBond, Data{0x06, 0x00}},
    {stakingBondExtra, Data{0x06, 0x01}},
    {stakingUnbond, Data{0x06, 0x02}},
    {stakingWithdrawUnbond, Data{0x06, 0x03}},
    {stakingNominate, Data{0x06, 0x05}},
    {stakingChill, Data{0x06, 0x06}},
    {utilityBatch, Data{0x18, 0x02}},
    {stakingReBond, Data{0x06, 0x13}},
};

static Data getCallIndex(const Proto::CallIndices& callIndices, uint32_t network, const std::string& key) {
    if (callIndices.has_custom()) {
        return encodeCallIndex(callIndices.custom().module_index(), callIndices.custom().method_index());
    }
    if (network == static_cast<uint32_t>(TWSS58AddressTypePolkadot) && polkadotCallIndices.contains(key)) {
        return polkadotCallIndices.at(key);
    }
    if (network == static_cast<uint32_t>(TWSS58AddressTypeKusama) && kusamaCallIndices.contains(key)) {
        return kusamaCallIndices.at(key);
    }
    throw std::invalid_argument("'call_indices' is not set");
}

bool Extrinsic::encodeRawAccount() const {
    if (multiAddress ||
        (network == static_cast<uint32_t>(TWSS58AddressTypePolkadot) && specVersion >= multiAddrSpecVersion) ||
        (network == static_cast<uint32_t>(TWSS58AddressTypeKusama) && specVersion >= multiAddrSpecVersionKsm)) {
        return false;
    }
    return true;
}

Data Extrinsic::encodeEraNonceTip() const {
    Data data;
    // era
    append(data, era);
    // nonce
    append(data, encodeCompact(nonce));
    // tip
    append(data, encodeCompact(tip));
    return data;
}

Data Extrinsic::encodeCall(const Proto::SigningInput& input) const {
    // call index from MetadataV11
    Data data;
    if (input.has_balance_call()) {
        data = encodeBalanceCall(input.balance_call());
    } else if (input.has_staking_call()) {
        data = encodeStakingCall(input.staking_call());
    } else if (input.has_polymesh_call()) {
        data = encodePolymeshCall(input.polymesh_call());
    }
    return data;
}

Data Extrinsic::encodeBalanceCall(const Proto::Balance& balance) const {
    Data data;
    if (balance.has_transfer()) {
        data = encodeTransfer(balance.transfer());
    } else if (balance.has_batchtransfer()) {
        //init call array
        auto calls = std::vector<Data>();
        for (const auto& transfer : balance.batchtransfer().transfers()) {
            Data itemData = encodeTransfer(transfer);
            // put into calls array
            calls.push_back(itemData);
        }
        data = encodeBatchCall(balance.batchtransfer().call_indices(), calls);
    } else if (balance.has_asset_transfer()) {
        data = encodeAssetTransfer(balance.asset_transfer());
    } else if (balance.has_batch_asset_transfer()) {
        // init call array
        auto calls = std::vector<Data>();
        for (const auto& transfer : balance.batch_asset_transfer().transfers()) {
            // put into calls array
            calls.push_back(encodeAssetTransfer(transfer));
        }
        data = encodeBatchCall(balance.batch_asset_transfer().call_indices(), calls);
    }

    return data;
}

Data Extrinsic::encodeTransfer(const Proto::Balance::Transfer& transfer) const {
    Data data;

    auto address = SS58Address(transfer.to_address(), network);
    auto value = load(transfer.value());
    // call index
    append(data, getCallIndex(transfer.call_indices(), network, balanceTransfer));
    // destination
    append(data, encodeAccountId(address.keyBytes(), encodeRawAccount()));
    // value
    append(data, encodeCompact(value));
    // memo
    if (transfer.memo().length() > 0) {
        append(data, 0x01);
        auto memo = transfer.memo();
        if (memo.length() < 32) {
            // padding memo with null
            memo.append(32 - memo.length(), '\0');
        }
        append(data, TW::data(memo));
    }

    return data;
}

Data Extrinsic::encodeAssetTransfer(const Proto::Balance::AssetTransfer& transfer) const {
    Data data;

    auto address = SS58Address(transfer.to_address(), network);
    auto value = load(transfer.value());

    // call index
    append(data, getCallIndex(transfer.call_indices(), network, assetsTransfer));
    // asset id
    if (transfer.asset_id() > 0) {
        // For native token transfer, should ignore asset id
        append(data, Polkadot::encodeCompact(transfer.asset_id()));
    }
    // destination
    append(data, Polkadot::encodeAccountId(address.keyBytes(), encodeRawAccount()));
    // value
    append(data, Polkadot::encodeCompact(value));

    return data;
}

Data Extrinsic::encodeBatchCall(const Proto::CallIndices& batchCallIndices, const std::vector<Data>& calls) const {
    Data data;
    append(data, getCallIndex(batchCallIndices, network, utilityBatch));
    append(data, encodeVector(calls));
    return data;
}

Data Extrinsic::encodeStakingCall(const Proto::Staking& staking) const {
    Data data;
    switch (staking.message_oneof_case()) {
    case Proto::Staking::kBond: {
        auto value = load(staking.bond().value());
        auto reward = byte(staking.bond().reward_destination());
        // call index
        append(data, getCallIndex(staking.bond().call_indices(), network, stakingBond));
        // controller
        if (!staking.bond().controller().empty()) {
            auto controller = SS58Address(staking.bond().controller(), network);
            append(data, encodeAccountId(controller.keyBytes(), encodeRawAccount()));
        }
        // value
        append(data, encodeCompact(value));
        // reward destination
        append(data, reward);
    } break;

    case Proto::Staking::kBondAndNominate: {
        // encode call1
        Data call1;
        {
            auto staking1 = Proto::Staking();
            auto* bond = staking1.mutable_bond();
            bond->set_controller(staking.bond_and_nominate().controller());
            bond->set_value(staking.bond_and_nominate().value());
            bond->set_reward_destination(staking.bond_and_nominate().reward_destination());
            // recursive call
            call1 = encodeStakingCall(staking1);
        }

        // encode call2
        Data call2;
        {
            auto staking2 = Proto::Staking();
            auto* nominate = staking2.mutable_nominate();
            for (auto i = 0; i < staking.bond_and_nominate().nominators_size(); ++i) {
                nominate->add_nominators(staking.bond_and_nominate().nominators(i));
            }
            // recursive call
            call2 = encodeStakingCall(staking2);
        }

        auto calls = std::vector<Data>{call1, call2};
        data = encodeBatchCall(staking.bond_and_nominate().call_indices(), calls);
    } break;

    case Proto::Staking::kBondExtra: {
        auto value = load(staking.bond_extra().value());
        // call index
        append(data, getCallIndex(staking.bond_extra().call_indices(), network, stakingBondExtra));
        // value
        append(data, encodeCompact(value));
    } break;

    case Proto::Staking::kUnbond: {
        auto value = load(staking.unbond().value());
        // call index
        append(data, getCallIndex(staking.unbond().call_indices(), network, stakingUnbond));
        // value
        append(data, encodeCompact(value));
    } break;

    case Proto::Staking::kRebond: {
        auto value = load(staking.rebond().value());
        // call index
        append(data, getCallIndex(staking.rebond().call_indices(), network, stakingReBond));
        // value
        append(data, encodeCompact(value));
    } break;

    case Proto::Staking::kWithdrawUnbonded: {
        auto spans = staking.withdraw_unbonded().slashing_spans();
        // call index
        append(data, getCallIndex(staking.withdraw_unbonded().call_indices(), network, stakingWithdrawUnbond));
        // num_slashing_spans
        encode32LE(spans, data);
    } break;

    case Proto::Staking::kNominate: {
        std::vector<SS58Address> accountIds;
        for (auto& n : staking.nominate().nominators()) {
            accountIds.emplace_back(SS58Address(n, network));
        }
        // call index
        append(data, getCallIndex(staking.nominate().call_indices(), network, stakingNominate));
        // nominators
        append(data, encodeAccountIds(accountIds, encodeRawAccount()));
    } break;

    case Proto::Staking::kChill:
        // call index
        append(data, getCallIndex(staking.chill().call_indices(), network, stakingChill));
        break;

    case Proto::Staking::kChillAndUnbond: {
        // encode call1
        Data call1;
        {
            auto staking1 = Proto::Staking();
            staking1.mutable_chill();
            // recursive call
            call1 = encodeStakingCall(staking1);
        }

        // encode call2
        Data call2;
        {
            auto staking2 = Proto::Staking();
            auto* unbond = staking2.mutable_unbond();
            unbond->set_value(staking.chill_and_unbond().value());
            // recursive call
            call2 = encodeStakingCall(staking2);
        }

        auto calls = std::vector<Data>{call1, call2};
        data = encodeBatchCall(staking.chill_and_unbond().call_indices(), calls);
    } break;

    default:
        break;
    }
    return data;
}

Data Extrinsic::encodePolymeshCall(const Proto::PolymeshCall& polymesh) const {
    Data data;

    if (polymesh.has_identity_call()) {
        const auto& identity = polymesh.identity_call();
        if (identity.has_join_identity_as_key()) {
            data = encodeIdentityJoinIdentityAsKey(identity.join_identity_as_key());
        } else if (identity.has_add_authorization()) {
            data = encodeIdentityAddAuthorization(identity.add_authorization());
        }
    }

    return data;
}

Data Extrinsic::encodeIdentityJoinIdentityAsKey(const Proto::Identity::JoinIdentityAsKey& joinIdentity) const {
    Data data;

    // call index
    append(data, getCallIndex(joinIdentity.call_indices(), network, joinIdentityAsKey));

    // data
    encode64LE(joinIdentity.auth_id(), data);

    return data;
}

Data Extrinsic::encodeIdentityAddAuthorization(const Proto::Identity::AddAuthorization & addAuthorization) const {
    Data data;

    // call index
    append(data, getCallIndex(addAuthorization.call_indices(), network, identityAddAuthorization));

    // target
    append(data, 0x01);
    SS58Address address(addAuthorization.target(), network);
    append(data, Polkadot::encodeAccountId(address.keyBytes(), true));

    // join identity
    append(data, 0x05);

    if (addAuthorization.has_data()) {
        const auto& authData = addAuthorization.data();

        // asset
        if (authData.has_asset()) {
            append(data, 0x01);
            append(data, TW::data(authData.asset().data()));
        } else {
            append(data, 0x00);
        }

        // extrinsic
        if (authData.has_extrinsic()) {
            append(data, 0x01);
            append(data, TW::data(authData.extrinsic().data()));
        } else {
            append(data, 0x00);
        }

        // portfolio
        if (authData.has_portfolio()) {
            append(data, 0x01);
            append(data, TW::data(authData.portfolio().data()));
        } else {
            append(data, 0x00);
        }
    } else {
        // authorize all permissions
        append(data, {0x01, 0x00}); // asset
        append(data, {0x01, 0x00}); // extrinsic
        append(data, {0x01, 0x00}); // portfolio
    }

    // expiry
    append(data, encodeCompact(addAuthorization.expiry()));

    return data;
}

Data Extrinsic::encodePayload() const {
    Data data;
    // call
    append(data, call);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // fee asset id
    if (!feeAssetId.empty()) {
        append(data, feeAssetId);
    }
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
    append(data, encodeAccountId(signer.bytes, encodeRawAccount()));
    // signature type
    append(data, sigTypeEd25519);
    // signature
    append(data, signature);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // fee asset id
    if (!feeAssetId.empty()) {
        append(data, feeAssetId);
    }
    // call
    append(data, call);
    // append length
    encodeLengthPrefix(data);
    return data;
}

Data Extrinsic::encodeFeeAssetId(const Proto::SigningInput& input) {
    if (!input.has_balance_call()) {
        return {};
    }

    uint32_t rawFeeAssetId {0};
    if (input.balance_call().has_asset_transfer()) {
        rawFeeAssetId = input.balance_call().asset_transfer().fee_asset_id();
    } else if (input.balance_call().has_batch_asset_transfer()) {
        rawFeeAssetId = input.balance_call().batch_asset_transfer().fee_asset_id();
    } else {
        return {};
    }

    Data feeAssetId;
    if (rawFeeAssetId > 0) {
        feeAssetId.push_back(0x01);
        Data feeEncoding;
        encode32LE(rawFeeAssetId, feeEncoding);
        append(feeAssetId, feeEncoding);
    } else {
        // use native token
        feeAssetId.push_back(0x00);
    }

    return feeAssetId;
}

} // namespace TW::Polkadot
