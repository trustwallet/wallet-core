// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TheOpenNetwork/Address.h"

#include "PublicKey.h"

#include "Everscale/Cell.h"
#include "Everscale/CellBuilder.h"

using namespace TW::Everscale;  // TODO(vbushev): move cell, cell builder and the workchain type to the common code?

namespace TW::TheOpenNetwork {

class Wallet {
protected:
    PublicKey publicKey;
    int8_t workchainId;

    // Explore standard codes: https://github.com/toncenter/tonweb/blob/master/src/contract/wallet/WalletSources.md
    const Data walletCode;      // TODO(vbushev): make some fields static?
    const u_int32_t walletId;

public:
    Wallet(PublicKey publicKey, int8_t workchainId, Data walletCode);
    virtual ~Wallet() noexcept = default;

    [[nodiscard]] Address getAddress();

protected:
    [[nodiscard]] virtual Cell::Ref createDataCell() = 0;

private:
    [[nodiscard]] Cell::Ref createStateInit();
};

} // namespace TW::TheOpenNetwork
