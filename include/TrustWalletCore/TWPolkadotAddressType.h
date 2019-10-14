// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Icon address types
TW_EXPORT_ENUM(uint32_t)

enum TWPolkadotAddressType {
	TWPolkadotAddressTypePolkaDotLiveSS58 = 0x0, // Polkadot Live (SS58 checksum preimage)
    TWPolkadotAddressTypePolkaDotLiveAccountID = 0x1, // Polkadot Live (AccountId checksum preimage)
    TWPolkadotAddressTypePolkaDotCanarySS58 = 0x2, // Polkadot Canary (SS58 checksum preimage)
    TWPolkadotAddressTypePolkaDotLiveCanaryAccountID = 0x3, // Polkadot Canary (AccountId checksum preimage)
    TWPolkadotAddressTypeKulupuSS58 = 0x10, // Kulupu (SS58 checksum preimage)
    TWPolkadotAddressTypeKulupu = 0x11, // Kulupu (Reserved)
    TWPolkadotAddressTypeDothereumSS58 = 0x14, // Dothereum (SS58 checksum preimage)
    TWPolkadotAddressTypeDothereumAccountID = 0x15, // Dothereum (AccountId checksum preimage)
    TWPolkadotAddressTypeGenericSS58 = 0x2a, // Generic Substrate wildcard (SS58 checksum preimage)
    TWPolkadotAddressTypeGenericAccountID = 0x2b // Generic Substrate wildcard (AccountId checksum preimage)
};

TW_EXTERN_C_END
