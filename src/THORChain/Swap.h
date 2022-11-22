// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/THORChainSwap.pb.h"

#include <optional>
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

using SwapErrorCode = int;

struct SwapBundled {
    Data out{};
    SwapErrorCode status_code{0};
    std::string error{""};
};

class SwapBuilder {
    Proto::Asset mFromAsset;
    Proto::Asset mToAsset;
    std::string mFromAddress;
    std::string mToAddress;
    std::string mVaultAddress;
    std::optional<std::string> mRouterAddress{std::nullopt};
    std::string mFromAmount;
    std::string mToAmountLimit;
    std::optional<std::string> mAffFeeAddress{std::nullopt};
    std::optional<std::string> mAffFeeRate{std::nullopt};
    std::optional<std::string> mExtraMemo{std::nullopt};

    std::string buildMemo() noexcept;
    SwapBundled buildBitcoin(uint64_t amount, const std::string& memo);
    SwapBundled buildBinance(Proto::Asset fromAsset, uint64_t amount, const std::string& memo);
    SwapBundled buildEth(uint64_t amount, const std::string& memo);

public:
    SwapBuilder() noexcept = default;

    static SwapBuilder builder() noexcept { return {}; }

    SwapBuilder& from(Proto::Asset fromAsset) noexcept {
        mFromAsset = std::move(fromAsset);
        return *this;
    }

    SwapBuilder& fromAddress(std::string fromAddress) noexcept {
        mFromAddress = std::move(fromAddress);
        return *this;
    }

    SwapBuilder& to(Proto::Asset toAsset) noexcept {
        mToAsset = std::move(toAsset);
        return *this;
    }

    SwapBuilder& toAddress(std::string toAddress) noexcept {
        mToAddress = std::move(toAddress);
        return *this;
    }

    SwapBuilder& vault(std::string vaultAddress) noexcept {
        mVaultAddress = std::move(vaultAddress);
        return *this;
    }

    SwapBuilder& router(std::string router) noexcept {
        if (!router.empty()) {
            mRouterAddress = std::move(router);
        }
        return *this;
    }

    SwapBuilder& affFeeAddress(std::string affFeeAddress) noexcept {
        if (!affFeeAddress.empty()) {
            mAffFeeAddress = std::move(affFeeAddress);
        }
        return *this;
    }

    SwapBuilder& affFeeRate(std::string affFeeRate) noexcept {
        if (!affFeeRate.empty()) {
            mAffFeeRate = std::move(affFeeRate);
        }
        return *this;
    }

    SwapBuilder& extraMemo(std::string extraMemo) noexcept {
        if (!extraMemo.empty()) {
            mExtraMemo = std::move(extraMemo);
        }
        return *this;
    }

    SwapBuilder& fromAmount(std::string fromAmount) noexcept {
        mFromAmount = std::move(fromAmount);
        return *this;
    }

    SwapBuilder& toAmountLimit(std::string toAmountLimit) noexcept {
        mToAmountLimit = std::move(toAmountLimit);
        return *this;
    }

    SwapBundled build();
};

/// Building THORChain cross-chain transactions
struct Swap {
    static std::string buildMemo(Proto::Asset toAsset, const std::string& toAddress, uint64_t limit, const std::string& feeAddress, std::optional<uint16_t> feeRate, const std::string& extra);
};

} // namespace TW::THORChainSwap
