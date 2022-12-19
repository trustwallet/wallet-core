// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PublicKey.h"
#include "PrivateKey.h"

#include "TheOpenNetwork/Address.h"
#include "TheOpenNetwork/Message.h"

#include "Everscale/Cell.h"
#include "Everscale/CellBuilder.h"

using namespace TW::Everscale;

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
        uint32_t seqno,
        uint8_t mode,
        uint64_t expireAt = 0,
        const std::string& comment = ""
    ) const;

protected:
    [[nodiscard]] virtual Cell::Ref createDataCell() const = 0;
    virtual void writeSigningPayload(CellBuilder& builder, uint32_t seqno = 0, uint64_t expireAt = 0) const = 0;

private:
    [[nodiscard]] Cell::Ref createSigningMessage(
        const Address& dest,
        uint64_t amount,
        uint32_t seqno,
        uint8_t mode,
        uint64_t expireAt = 0,
        const std::string& comment = ""
    ) const;
    [[nodiscard]] Everscale::StateInit createStateInit() const;
};

} // namespace TW::TheOpenNetwork
