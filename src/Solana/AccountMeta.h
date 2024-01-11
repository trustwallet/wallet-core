// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Solana/Address.h"

namespace TW::Solana {

struct AccountMeta {
    Address account;
    bool isSigner;
    bool isReadOnly;
    AccountMeta(const Address& address, bool isSigner, bool isReadOnly) noexcept : account(address), isSigner(isSigner), isReadOnly(isReadOnly) {}
};

}
