// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
