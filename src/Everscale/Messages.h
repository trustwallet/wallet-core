// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <optional>
#include <utility>

#include "Address.h"
#include "CellBuilder.h"
#include "CellSlice.h"
#include "Wallet.h"

#include "../PrivateKey.h"

namespace TW::Everscale {

using uint128_t = CellBuilder::uint128_t;

class CommonMsgInfo {
public:
    virtual void writeTo(CellBuilder& builder) const = 0;

    virtual ~CommonMsgInfo() = default;
};

class ExternalInboundMessageHeader : public CommonMsgInfo {
    Address _dst;
    uint128_t _importFee{};

public:
    explicit ExternalInboundMessageHeader(Address dst)
        : _dst(dst) {}

    void writeTo(CellBuilder& builder) const override;
};

class InternalMessageHeader : public CommonMsgInfo {
    bool _ihrDisabled;
    bool _bounce;
    Address _dst;
    uint128_t _value;

    bool _bounced{};
    uint128_t _ihrFee{};
    uint128_t _fwdFee{};
    uint64_t _createdLt{};
    uint32_t _createdAt{};

public:
    InternalMessageHeader(bool ihrDisabled, bool bounce, Address dst, uint64_t value)
        : _ihrDisabled(ihrDisabled), _bounce(bounce), _dst(dst), _value(static_cast<uint128_t>(value)) {}

    void writeTo(CellBuilder& builder) const override;
};

class Message {
private:
    std::shared_ptr<CommonMsgInfo> _header;

    std::optional<StateInit> _init{};
    std::optional<CellSlice> _body{};

public:
    using HeaderRef = std::shared_ptr<CommonMsgInfo>;

    explicit Message(HeaderRef header)
        : _header(std::move(header)) {}

    [[nodiscard]] Cell::Ref intoCell() const;
    inline void setBody(CellSlice body) { _body = body; }
    inline void setStateInit(const StateInit& stateInit) { _init = stateInit; }
};

Data createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount,
                         uint32_t expiredAt, Address destination, const Cell::Ref& contractData);

} // namespace TW::Everscale
