// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "Protobuf/TronInternal.pb.h"

#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/base58.h>

#include <cassert>

using namespace TW;
using namespace TW::Tron;

size_t base58Capacity = 128;

/// Converts an external TransferContract to an internal one used for signing.
protocol::TransferContract to_internal(const Proto::TransferContract& transfer) {
    auto internal = protocol::TransferContract();

    uint8_t ownerAddressData[base58Capacity];
    int ownerAddressSize = base58_decode_check(transfer.owner_address().c_str(), HASHER_SHA2D, ownerAddressData, (int)base58Capacity);
    internal.set_owner_address(ownerAddressData, ownerAddressSize);

    uint8_t toAddressData[base58Capacity];
    int toAddressSize = base58_decode_check(transfer.to_address().c_str(), HASHER_SHA2D, toAddressData, (int)base58Capacity);
    internal.set_to_address(toAddressData, toAddressSize);

    internal.set_amount(transfer.amount());

    return internal;
}

/// Converts an external TransferAssetContract to an internal one used for signing.
protocol::TransferAssetContract to_internal(const Proto::TransferAssetContract& transfer) {
    auto internal = protocol::TransferAssetContract();

    internal.set_asset_name(transfer.asset_name());

    uint8_t ownerAddressData[base58Capacity];
    int ownerAddressSize = base58_decode_check(transfer.owner_address().c_str(), HASHER_SHA2D, ownerAddressData, (int)base58Capacity);
    internal.set_owner_address(ownerAddressData, ownerAddressSize);

    uint8_t toAddressData[base58Capacity];
    int toAddressSize = base58_decode_check(transfer.to_address().c_str(), HASHER_SHA2D, toAddressData, (int)base58Capacity);
    internal.set_to_address(toAddressData, toAddressSize);

    internal.set_amount(transfer.amount());

    return internal;
}

/// Converts an external BlockHeader to an internal one used for signing.
protocol::BlockHeader to_internal(const Proto::BlockHeader& header) {
    auto internal = protocol::BlockHeader();

    internal.mutable_raw_data()->set_timestamp(header.timestamp());
    internal.mutable_raw_data()->set_tx_trie_root(header.tx_trie_root());
    internal.mutable_raw_data()->set_parent_hash(header.parent_hash());
    internal.mutable_raw_data()->set_number(header.number());
    internal.mutable_raw_data()->set_witness_address(header.witness_address());
    internal.mutable_raw_data()->set_version(header.version());

    return internal;
}

Data getBlockHash(const protocol::BlockHeader& header) {
    const auto data = header.raw_data().SerializeAsString();
    return Hash::sha256(data);
}

void setBlockReference(const Proto::Transaction& transaction, protocol::Transaction& internal) {
    const auto blockHash = getBlockHash(to_internal(transaction.block_header()));
    assert(blockHash.size() > 15);
    internal.mutable_raw_data()->set_ref_block_hash(blockHash.data() + 8, 8);

    const auto blockHeight = transaction.block_header().number();
    auto heightData = Data();
    encode64LE(blockHeight, heightData);
    std::reverse(heightData.begin(), heightData.end());
    internal.mutable_raw_data()->set_ref_block_bytes(heightData.data() + heightData.size() - 2, 2);
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto internal = protocol::Transaction();
    auto output = Proto::SigningOutput();

    if (input.transaction().has_transfer()) {
        auto contract = internal.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TransferContract);

        auto transfer = to_internal(input.transaction().transfer());
        google::protobuf::Any any;
        any.PackFrom(transfer);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_transfer_asset()) {
        auto contract = internal.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TransferAssetContract);

        auto transfer = to_internal(input.transaction().transfer_asset());
        google::protobuf::Any any;
        any.PackFrom(transfer);
        *contract->mutable_parameter() = any;
    }

    internal.mutable_raw_data()->set_timestamp(input.transaction().timestamp());
    internal.mutable_raw_data()->set_expiration(input.transaction().expiration());
    setBlockReference(input.transaction(), internal);

    output.set_ref_block_bytes(internal.raw_data().ref_block_bytes());
    output.set_ref_block_hash(internal.raw_data().ref_block_hash());

    const auto serialized = internal.raw_data().SerializeAsString();
    const auto hash = Hash::sha256(Data(serialized.begin(), serialized.end()));

    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto signature = key.sign(hash, TWCurveSECP256k1);

    output.set_id(hash.data(), hash.size());
    output.set_signature(signature.data(), signature.size());

    return output;
}
