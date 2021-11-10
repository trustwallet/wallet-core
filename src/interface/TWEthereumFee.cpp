// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumFee.h>

#include <nlohmann/json.hpp>

#include "Ethereum/Fee.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <string>
#include <vector>

using namespace TW;

TWString* _Nullable TWEthereumFeeSuggest(TWString* _Nonnull feeHistory) {
    const auto& json = *reinterpret_cast<const std::string*>(feeHistory);
    try {
        const auto parsed = nlohmann::json::parse(json);
        const auto fee = Ethereum::Fee::suggestFee(parsed);
        return TWStringCreateWithUTF8Bytes(fee.dump().c_str());
    }
    catch(...) {
        return nullptr;
    }
}
