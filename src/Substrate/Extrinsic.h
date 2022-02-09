//
// Created by Fitz on 2022/1/26.
//

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../proto/Substrate.pb.h"
#include "../uint256.h"
#include "ScaleCodec.h"

namespace TW::Substrate {

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
    byte network;

    Extrinsic(const Proto::SigningInput& input)
    : blockHash(input.block_hash().begin(), input.block_hash().end())
    , genesisHash(input.genesis_hash().begin(), input.genesis_hash().end())
    , nonce(input.nonce())
    , specVersion(input.spec_version())
    , version(input.transaction_version())
    , tip(load(input.tip())) {
        if (input.has_era()) {
            era = encodeEra(input.era().block_number(), input.era().period());
        } else {
            // immortal era
            era = encodeCompact(0);
        }
        network = byte(input.network());
        call = encodeCall(input);
    }

    static Data encodeCall(const Proto::SigningInput& input);
    // Payload to sign.
    Data encodePayload() const;
    // Encode final data with signer public key and signature.
    Data encodeSignature(const PublicKey& signer, const Data& signature) const;

  protected:
    static bool encodeRawAccount(byte network, uint32_t specVersion);
    static Data encodeBalanceCall(const Proto::Balance& balance, byte network, uint32_t specVersion);
    static Data encodeBatchCall(const std::vector<Data>& calls, int32_t moduleIndex, int32_t methodIndex);
    Data encodeEraNonceTip() const;
};

} // namespace TW::Substrate