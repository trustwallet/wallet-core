// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <optional>
#include <utility>

#include "Cell.h"
#include "CellBuilder.h"
#include "CellSlice.h"
#include "RawAddress.h"

#include "PrivateKey.h"

namespace TW::CommonTON {

class CommonMsgInfo {
public:
    virtual void writeTo(CellBuilder& builder) const = 0;

    virtual ~CommonMsgInfo() = default;
};

class ExternalInboundMessageHeader : public CommonMsgInfo {
    AddressData _dst;
    uint128_t _importFee{};

public:
    explicit ExternalInboundMessageHeader(AddressData dst)
        : _dst(dst) {}

    void writeTo(CellBuilder& builder) const override;
};

class InternalMessageHeader : public CommonMsgInfo {
    bool _ihrDisabled;
    bool _bounce;
    AddressData _dst;
    uint128_t _value;

    bool _bounced{};
    uint128_t _ihrFee{};
    uint128_t _fwdFee{};
    uint64_t _createdLt{};
    uint32_t _createdAt{};

public:
    InternalMessageHeader(bool ihrDisabled, bool bounce, AddressData dst, uint64_t value)
        : _ihrDisabled(ihrDisabled), _bounce(bounce), _dst(dst), _value(static_cast<uint128_t>(value)) {}

    void writeTo(CellBuilder& builder) const override;
};

class StateInit {
public:
    Cell::Ref code;
    Cell::Ref data;

    [[nodiscard]] CellBuilder writeTo() const;
};

struct MessageData {
    std::shared_ptr<CommonMsgInfo> header;
    std::optional<StateInit> init{};
    std::optional<Cell::Ref> body{};

    using HeaderRef = std::shared_ptr<CommonMsgInfo>;

    explicit MessageData(HeaderRef header) : header(std::move(header)) { }
};

} // namespace TW::CommonTON
