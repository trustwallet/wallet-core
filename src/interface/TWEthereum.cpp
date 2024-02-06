// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "Ethereum/EIP1014.h"
#include "Ethereum/EIP2645.h"
#include <TrustWalletCore/TWEthereum.h>

#include <string>

TWString* TWEthereumEip2645GetPath(TWString* ethAddress, TWString* layer, TWString* application, TWString* index) {
    const auto& ethAddressStr = *reinterpret_cast<const std::string*>(ethAddress);
    const auto& layerStr = *reinterpret_cast<const std::string*>(layer);
    const auto& applicationStr = *reinterpret_cast<const std::string*>(application);
    const auto& indexStr = *reinterpret_cast<const std::string*>(index);
    return new std::string(TW::Ethereum::accountPathFromAddress(ethAddressStr, layerStr, applicationStr, indexStr));
}