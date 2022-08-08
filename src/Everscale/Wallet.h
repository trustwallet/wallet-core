// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <boost/integer.hpp>
#include <utility>

#include "../PublicKey.h"

#include "Address.h"
#include "Cell.h"
#include "CellBuilder.h"
#include "CellSlice.h"

const uint32_t WALLET_ID = 0x4BA92D8A;

namespace TW::Everscale {

class Wallet {
public:
    struct Gift {
        bool bounce;
        uint64_t amount;
        Address::MsgAddressInt destination;
        uint8_t flags;
    };

    static const Data code;
};

class StateInit {
    Cell::Ref _code;
    Cell::Ref _data;

public:
    explicit StateInit(Cell::Ref code, Cell::Ref data)
        : _code(std::move(code)), _data(std::move(data)) {}

    [[nodiscard]] CellBuilder writeTo() const;
};

class InitData {
    uint32_t _seqno;
    uint32_t _walletId;
    PublicKey _publicKey;

public:
    explicit InitData(PublicKey publicKey)
        : _seqno(0), _walletId(WALLET_ID), _publicKey(std::move(publicKey)) {}
    explicit InitData(CellSlice cs)
        : _seqno(cs.getNextU32()), _walletId(cs.getNextU32()), _publicKey(PublicKey(cs.getNextBytes(32), TWPublicKeyTypeED25519)) {}

    [[nodiscard]] CellBuilder writeTo() const;
    [[nodiscard]] StateInit makeStateInit() const;
    [[nodiscard]] Address::MsgAddressInt computeAddr(int8_t workchainId) const;
    [[nodiscard]] CellBuilder makeTransferPayload(uint32_t expireAt, const Wallet::Gift& gift) const;
};

} // namespace TW::Everscale
