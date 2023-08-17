// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/THORChainSwap.pb.h"
#include "uint256.h"

#include <optional>
#include <string>
#include <utility>
#include <optional>

namespace TW::THORChainSwap {

/// Supported blockchains
enum Chain {
    THOR = 0,
    BTC = 1,
    ETH = 2,
    BNB = 3,
    DOGE = 4,
    BCH = 5,
    LTC = 6,
    ATOM = 7,
    AVAX = 8
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
    std::string mToAmountLimit{"0"};
    std::optional<std::string> mAffFeeAddress{std::nullopt};
    std::optional<std::string> mAffFeeRate{std::nullopt};
    std::optional<std::string> mExtraMemo{std::nullopt};
    std::optional<std::size_t> mExpirationPolicy{std::nullopt};

    SwapBundled buildBitcoin(const uint256_t& amount, const std::string& memo, Chain fromChain);
    SwapBundled buildBinance(Proto::Asset fromAsset, const uint256_t& amount, const std::string& memo);
    SwapBundled buildEth(const uint256_t& amount, const std::string& memo);
    SwapBundled buildAtom(const uint256_t& amount, const std::string& memo);
    SwapBundled buildRune(const uint256_t& amount, const std::string& memo);

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
        } else {
            mAffFeeAddress = std::nullopt;
        }
        return *this;
    }

    SwapBuilder& affFeeRate(std::string affFeeRate) noexcept {
        if (!affFeeRate.empty()) {
            mAffFeeRate = std::move(affFeeRate);
        } else {
            mAffFeeRate = std::nullopt;
        }
        return *this;
    }

    SwapBuilder& extraMemo(std::string extraMemo) noexcept {
        if (!extraMemo.empty()) {
            mExtraMemo = std::move(extraMemo);
        } else {
            mExtraMemo = std::nullopt;
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

    SwapBuilder& expirationPolicy(std::size_t expirationTime)  noexcept {
        if (expirationTime > 0) {
            mExpirationPolicy = expirationTime;
        } else {
            mExpirationPolicy = std::nullopt;
        }
        return *this;
    }

    std::string buildMemo(bool shortened = true) noexcept;

    SwapBundled build(bool shortened = true);
};

} // namespace TW::THORChainSwap
