// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Wallet.h"

namespace TW::TheOpenNetwork {

class WalletV4R2 : public Wallet {
public:
    explicit WalletV4R2(PublicKey publicKey, int8_t workchainId);

    static const Data code;

private:
    [[nodiscard]] Cell::Ref createDataCell() const override;
    void writeSigningPayload(CellBuilder& builder, uint32_t sequence_number = 0, uint32_t expireAt = 0) const override;
};

} // namespace TW::TheOpenNetwork
