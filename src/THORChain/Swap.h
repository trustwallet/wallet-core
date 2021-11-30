// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <string>
#include <utility>

namespace TW::THORChainSwap {

/// Supported blockchains
enum Chain {
    THOR = 0,
    BTC = 1,
    ETH = 2,
    BNB = 3,
};

/// Building THORChain cross-chain transactions
class Swap {
public:
    /// Logic to build a native transaction on the source chain for a swap
    /// Returns serialized SigningInput proto message, on the source chain,
    /// and an optional error code + message
    static std::tuple<Data, int, std::string> build(
        Chain fromChain,
        Chain toChain,
        const std::string& fromAddress,     // source address, on source chain, string format
        const std::string& toSymbol,        // destination coin symbol
        const std::string& toTokenId,       // destination token ID, on the destination chain, in case destination is a token, empty otherwise
        const std::string& toAddress,       // destination address, on destination chain, string format
        const std::string& vaultAddress,    // ThorChainSwap vault, on the source chain. Should be queried afresh, as it may change
        const std::string& routerAddress,   // ThorChain router, only in case of Ethereum source network
        const std::string& fromAmount,      // The source amount, as integer in the smallest native unit of the chain
        const std::string& toAmountLimit    // The minimum accepted destination amount.  Actual destination amount will depend on current rates, limit amount can be used to prevent using very unfavorable rates.
    );

protected:
    static std::pair<int, std::string> buildBitcoin(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, uint64_t amount, const std::string& memo, Data& out);
    static std::pair<int, std::string> buildEthereum(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, const std::string& routerAddress, uint64_t amount, const std::string& memo, Data& out);
    static std::pair<int, std::string> buildBinance(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, uint64_t amount, const std::string& memo, Data& out);

public:
    static std::string buildMemo(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& toAddress, uint64_t limit);
};

} // namespace TW
