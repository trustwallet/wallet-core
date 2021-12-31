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
private:
    NetworkConfig networkConfig;
    GasEstimator gasEstimator;
public:
    TransactionFactory();
    TransactionFactory(const NetworkConfig& networkConfig);

    /// Creates the appropriate transaction object, with respect to the "oneof" field (substructure) of Proto::SigningInput.
    Proto::TransactionMessage createTransaction(const Proto::SigningInput &input);

    Proto::TransactionMessage createGenericTransaction(const Proto::TransactionMessage& genericTransaction);

    /// This should be used to transfer EGLD.
    /// For reference, see: https://docs.elrond.com/developers/signing-transactions/signing-transactions/#general-structure.
    Proto::TransactionMessage createEGLDTransfer(const Proto::EGLDTransfer& transfer);

    /// This should be used to transfer regular ESDTs (fungible tokens).
    /// For reference, see: https://docs.elrond.com/developers/esdt-tokens/#transfers
    ///
    /// The "regular" ESDT tokens held by an account can be fetched from https://api.elrond.com/accounts/{address}/tokens.
    Proto::TransactionMessage createESDTTransfer(const Proto::ESDTTransfer& transfer);

    /// This should be used to transfer NFTs, SFTs and Meta ESDTs.
    /// For reference, see: https://docs.elrond.com/developers/nft-tokens/#transfers
    ///
    /// The semi-fungible and non-fungible tokens held by an account can be fetched from https://api.elrond.com/accounts/{address}/nfts?type=SemiFungibleESDT,NonFungibleESDT.
    /// The Meta ESDTs (a special kind of SFTs) held by an account can be fetched from https://api.elrond.com/accounts/{address}/nfts?type=MetaESDT.
    ///
    /// The fields "transfer.token_collection" and "transfer.token_nonce" are found as well in the HTTP response of the API call (as "collection" and "nonce", respectively).
    Proto::TransactionMessage createESDTNFTTransfer(const Proto::ESDTNFTTransfer& transfer);
private:
    uint64_t coalesceGasLimit(uint64_t providedGasLimit, uint64_t estimatedGasLimit);
    uint64_t coalesceGasPrice(uint64_t gasPrice);
    std::string coalesceChainId(std::string chainID);
    std::string prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments);
};

} // namespace
