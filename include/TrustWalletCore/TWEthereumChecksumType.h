// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Checksum types for Ethereum-based blockchains.
TW_EXPORT_ENUM()
enum TWEthereumChecksumType {
    TWEthereumChecksumTypeEIP55 = 0,
    TWEthereumChecksumTypeWanchain = 1,
};

TW_EXTERN_C_END
