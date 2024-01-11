// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Transaction.h"
#include "TransactionFactoryConfig.h"
#include "uint256.h"
#include "../proto/MultiversX.pb.h"

namespace TW::MultiversX {

/// Creates specific transaction objects, wrt. the provided "TransactionFactoryConfig".
class TransactionFactory {
private:
    TransactionFactoryConfig config;

public:
    TransactionFactory();
    TransactionFactory(const TransactionFactoryConfig& config);

    /// Creates the appropriate transaction object, with respect to the "oneof" field (substructure) of Proto::SigningInput.
    Transaction create(const Proto::SigningInput& input);

    Transaction fromGenericAction(const Proto::SigningInput& input);

    /// This should be used to transfer EGLD.
    /// For reference, see: https://docs.multiversx.com/developers/signing-transactions/signing-transactions.
    Transaction fromEGLDTransfer(const Proto::SigningInput& input);

    /// This should be used to transfer regular ESDTs (fungible tokens).
    /// For reference, see: https://docs.multiversx.com/developers/esdt-tokens
    ///
    /// The "regular" ESDT tokens held by an account can be fetched from https://api.multiversx.com/accounts/{address}/tokens.
    Transaction fromESDTTransfer(const Proto::SigningInput& input);

    /// This should be used to transfer NFTs, SFTs and Meta ESDTs.
    /// For reference, see: https://docs.multiversx.com/developers/nft-tokens
    ///
    /// The semi-fungible and non-fungible tokens held by an account can be fetched from https://api.multiversx.com/accounts/{address}/nfts?type=SemiFungibleESDT,NonFungibleESDT.
    /// The Meta ESDTs (a special kind of SFTs) held by an account can be fetched from https://api.multiversx.com/accounts/{address}/nfts?type=MetaESDT.
    ///
    /// The fields "token_collection" and "token_nonce" are found as well in the HTTP response of the API call (as "collection" and "nonce", respectively).
    Transaction fromESDTNFTTransfer(const Proto::SigningInput& input);

private:
    uint64_t computeGasLimit(size_t dataLength, uint64_t executionGasLimit, bool hasGuardian);
    uint64_t coalesceGasLimit(uint64_t providedGasLimit, uint64_t estimatedGasLimit);
    uint64_t coalesceGasPrice(uint64_t gasPrice);
    std::string coalesceChainId(std::string chainID);
    TransactionOptions decideOptions(const Transaction& transaction);
    std::string prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments);
};

} // namespace TW::MultiversX
