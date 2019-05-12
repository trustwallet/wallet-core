// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Chain identifier for Ethereum-based blockchains.
TW_EXPORT_ENUM(uint32_t)
enum TWEthereumChainID {
    TWEthereumChainIDEthereum = 1,
    TWEthereumChainIDGo = 60,
    TWEthereumChainIDPOA = 99,
    TWEthereumChainIDCallisto = 820,
    TWEthereumChainIDEllaism = 64,
    TWEthereumChainIDEthereumClassic = 61,
    TWEthereumChainIDEthersocial = 31102,
    TWEthereumChainIDVeChain = 74,
    TWEthereumChainIDThunderToken = 18,
    TWEthereumChainIDTomoChain = 88,
    TWEthereumChainIDXDai = 100,
    TWEthereumChainIDDEXON = 237,
};

TW_EXTERN_C_END
