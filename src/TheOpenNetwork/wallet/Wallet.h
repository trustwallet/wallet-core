// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PublicKey.h"
#include "PrivateKey.h"

#include "TheOpenNetwork/Address.h"
#include "TheOpenNetwork/Message.h"

#include "Everscale/CommonTON/Messages.h"

namespace TW::TheOpenNetwork {

class Wallet {
protected:
    PublicKey publicKey;
    int8_t workchainId;

    // Explore standard codes: https://github.com/toncenter/tonweb/blob/master/src/contract/wallet/WalletSources.md
    const Data walletCode;
    const uint32_t walletId;

public:
    explicit Wallet(PublicKey publicKey, int8_t workchainId, Data walletCode);
    virtual ~Wallet() noexcept = default;

    [[nodiscard]] Address getAddress() const;
    [[nodiscard]] Cell::Ref createTransferMessage(
        const PrivateKey& privateKey,
        const Address& dest,
        uint64_t amount,
        uint32_t sequence_number,
        uint8_t mode,
        uint32_t expireAt = 0,
        const std::string& comment = ""
    ) const;

    [[nodiscard]] Cell::Ref createQueryMessage(
        const PrivateKey& privateKey,
        const Address& dest,
        uint64_t amount,
        uint32_t sequence_number,
        uint8_t mode,
        const Cell::Ref& payload,
        uint32_t expireAt = 0
    ) const;

protected:
    [[nodiscard]] virtual Cell::Ref createDataCell() const = 0;
    virtual void writeSigningPayload(CellBuilder& builder, uint32_t sequence_number = 0, uint32_t expireAt = 0) const = 0;

private:
    [[nodiscard]] Cell::Ref createSigningMessage(
        const Address& dest,
        uint64_t amount,
        uint32_t sequence_number,
        uint8_t mode,
        const Cell::Ref& payload,
        uint32_t expireAt = 0
    ) const;
    [[nodiscard]] CommonTON::StateInit createStateInit() const;
};

} // namespace TW::TheOpenNetwork
