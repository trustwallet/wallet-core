// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../Polkadot/ScaleCodec.h"
#include "../proto/Substrate.pb.h"
#include "../uint256.h"

namespace TW::Substrate {

// ExtrinsicV4
class Extrinsic {
private:
    Proto::SigningInput _input;

public:
    Data blockHash;
    Data genesisHash;
    uint64_t nonce;
    // Runtime spec version
    uint32_t _specVersion;
    // transaction version
    uint32_t version;
    // balances::TakeFees
    uint256_t tip;
    // encoded Era data
    Data era;
    // encoded Call data
    Data call;
    // network
    int32_t _network;
    // enable multi-address
    bool multiAddress;

    // keep fee asset ids
    Data feeAssetId;

    Extrinsic(const Proto::SigningInput& input)
        : _input(input)
        , blockHash(input.block_hash().begin(), input.block_hash().end())
        , genesisHash(input.genesis_hash().begin(), input.genesis_hash().end())
        , nonce(input.nonce())
        , _specVersion(input.spec_version())
        , version(input.transaction_version())
        , tip(load(input.tip()))
        , _network(input.network())
        , multiAddress(input.multi_address()) {
        if (input.has_era()) {
            era = Polkadot::encodeEra(input.era().block_number(), input.era().period());
        } else {
            // immortal era
            era = Polkadot::encodeCompact(0);
        }
        call = encodeCall();
    }

    Data encodeCall();
    // Payload to sign.
    Data encodePayload() const;
    // Encode final data with signer public key and signature.
    Data encodeSignature(const PublicKey& signer, const Data& signature) const;

protected:
    bool encodeRawAccount(bool enableMultiAddress) const;
    Data encodeTransfer(const Proto::Balance::Transfer& transfer, int32_t network, bool enableMultiAddress) const;
    Data encodeAssetTransfer(const Proto::Balance::AssetTransfer& transfer, int32_t network, bool enableMultiAddress);
    Data encodeBalanceCall(const Proto::Balance& balance);
    Data encodeBatchCall(const std::vector<Data>& calls, int32_t moduleIndex, int32_t methodIndex) const;
    Data encodeEraNonceTip() const;
    Data encodeAuthorizationCall(const Proto::Authorization& authorization) const;
    Data encodeIdentityCall(const Proto::Identity& authorization) const;
};

} // namespace TW::Substrate
