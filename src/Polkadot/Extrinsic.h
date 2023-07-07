// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Data.h"
#include "../proto/Polkadot.pb.h"
#include "../uint256.h"
#include  "ScaleCodec.h"

namespace TW::Polkadot {

// ExtrinsicV4
class Extrinsic {
  public:
    Data blockHash;
    Data genesisHash;
    uint64_t nonce;
    // Runtime spec version
    uint32_t specVersion;
    // transaction version
    uint32_t version;
    // balances::TakeFees
    uint256_t tip;
    // encoded Era data
    Data era;
    // encoded Call data
    Data call;
    // network
    uint32_t network;
    // enable multi-address
    bool multiAddress;
    // fee asset id
    Data feeAssetId;

    explicit Extrinsic(const Proto::SigningInput& input)
        : blockHash(input.block_hash().begin(), input.block_hash().end())
        , genesisHash(input.genesis_hash().begin(), input.genesis_hash().end())
        , nonce(input.nonce())
        , specVersion(input.spec_version())
        , version(input.transaction_version())
        , tip(load(input.tip()))
        , network(input.network())
        , multiAddress(input.multi_address()) {
        if (input.has_era()) {
            era = encodeEra(input.era().block_number(), input.era().period());
        } else {
          // immortal era
          era = encodeCompact(0);
        }
        // keep fee asset id encoding which will be used in encodePayload & encodeSignature.
        // see: https://github.com/paritytech/substrate/blob/d1221692968b8bc62d6eab9d10cb6b5bf38c5dc2/frame/transaction-payment/asset-tx-payment/src/lib.rs#L152
        feeAssetId = encodeFeeAssetId(input);
        call = encodeCall(input);
    }

    Data encodeCall(const Proto::SigningInput& input) const;
    // Payload to sign.
    Data encodePayload() const;
    // Encode final data with signer public key and signature.
    Data encodeSignature(const PublicKey& signer, const Data& signature) const;

  protected:
    bool encodeRawAccount() const;
    Data encodeBalanceCall(const Proto::Balance& balance) const;
    Data encodeTransfer(const Proto::Balance::Transfer& transfer) const;
    Data encodeAssetTransfer(const Proto::Balance::AssetTransfer& transfer) const;
    Data encodeStakingCall(const Proto::Staking& staking) const;
    Data encodePolymeshCall(const Proto::PolymeshCall& polymesh) const;
    Data encodeIdentityJoinIdentityAsKey(const Proto::Identity::JoinIdentityAsKey& joinIdentityAsKey) const;
    Data encodeIdentityAddAuthorization(const Proto::Identity::AddAuthorization& addAuthorization) const;
    Data encodeBatchCall(const Proto::CallIndices& batchCallIndices, const std::vector<Data>& calls) const;
    Data encodeEraNonceTip() const;

    static Data encodeFeeAssetId(const Proto::SigningInput& input);
};

} // namespace TW::Polkadot
