// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Elrond.pb.h"
#include "Address.h"
#include "NetworkConfig.h"
#include "GasEstimator.h"
#include "uint256.h"

namespace TW::Elrond {

/// Creates specific transaction objects, wrt. the provided "NetworkConfig".
class TransactionFactory {
    NetworkConfig networkConfig;
    GasEstimator gasEstimator;
public:
    TransactionFactory(const NetworkConfig& networkConfig);

    /// This should be used to transfer EGLD.
    /// For reference, see: https://docs.elrond.com/developers/signing-transactions/signing-transactions/#general-structure.
    Proto::TransactionMessage createEGLDTransfer(const Address& sender, const Address& receiver, uint256_t amount);

    /// This should be used to transfer regular ESDTs (fungible tokens).
    /// For reference, see: https://docs.elrond.com/developers/esdt-tokens/#transfers
    ///
    /// The "regular" ESDT tokens held by an account can be fetched from https://api.elrond.com/accounts/{address}/tokens.
    Proto::TransactionMessage createESDTTransfer(const Address& sender, const Address& receiver, const std::string& tokenIdentifier, uint256_t amount);

    /// This should be used to transfer NFTs, SFTs and Meta ESDTs.
    /// For reference, see: https://docs.elrond.com/developers/nft-tokens/#transfers
    ///
    /// The semi-fungible and non-fungible tokens held by an account can be fetched from https://api.elrond.com/accounts/{address}/nfts?type=SemiFungibleESDT,NonFungibleESDT.
    /// The Meta ESDTs (a special kind of SFTs) held by an account can be fetched from https://api.elrond.com/accounts/{address}/nfts?type=MetaESDT.
    ///
    /// The "collection" and "nonce" parameters are found as well in the HTTP response of the API call.
    Proto::TransactionMessage createESDTNFTTransfer(const Address& sender, const Address& receiver, const std::string& collection, uint64_t nonce, uint256_t quantity);
};

} // namespace

/// Wrapper for C interface.
struct TWElrondTransactionFactory {
    TW::Elrond::TransactionFactory impl;
};
