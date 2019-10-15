// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Icon address types
TW_EXPORT_ENUM()

enum TWPolkadotAddressType {
	TWPolkaDotAddressTypePolkaDotLiveSS58 = 0x0, // Polkadot Live (SS58 checksum preimage)
    TWPolkaDotAddressTypePolkaDotLiveAccountID = 0x1, // Polkadot Live (AccountId checksum preimage)
    TWPolkaDotAddressTypePolkaDotCanarySS58 = 0x2, // Polkadot Canary (SS58 checksum preimage)
    TWPolkaDotAddressTypePolkaDotLiveCanaryAccountID = 0x3, // Polkadot Canary (AccountId checksum preimage)
    TWPolkaDotAddressTypeKulupuSS58 = 0x10, // Kulupu (SS58 checksum preimage)
    TWPolkaDotAddressTypeKulupu = 0x11, // Kulupu (Reserved)
    TWPolkaDotAddressTypeDothereumSS58 = 0x14, // Dothereum (SS58 checksum preimage)
    TWPolkaDotAddressTypeDothereumAccountID = 0x15, // Dothereum (AccountId checksum preimage)
    TWPolkaDotAddressTypeGenericSS58 = 0x2a, // Generic Substrate wildcard (SS58 checksum preimage)
    TWPolkaDotAddressTypeGenericAccountID = 0x2b // Generic Substrate wildcard (AccountId checksum preimage)
};

TW_EXTERN_C_END
