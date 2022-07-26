#pragma once

#include <optional>

#include "Address.h"
#include "CellSlice.h"
#include "CellBuilder.h"
#include "Wallet.h"

namespace TW::Everscale {

using uint128_t = CellBuilder::uint128_t;
using MsgAddressInt = Address::MsgAddressInt;

class CommonMsgInfo {
public:
    virtual void writeTo(CellBuilder& builder) const = 0;

    virtual ~CommonMsgInfo() = 0;
};

inline CommonMsgInfo::~CommonMsgInfo() { }

class ExternalInboundMessageHeader: public CommonMsgInfo {
    Address::MsgAddressInt dst_;
    CellBuilder::uint128_t importFee_;

public:
    ExternalInboundMessageHeader(MsgAddressInt dst) : dst_(dst), importFee_(0) {}

    void writeTo(CellBuilder& builder) const override;
};

class InternalMessageHeader: public CommonMsgInfo {
    bool ihrDisabled_;
    bool bounce_;
    bool bounced_;
    MsgAddressInt dst_;
    uint128_t value_;
    uint128_t ihrFee_;
    uint128_t fwdFee_;
    uint64_t createdLt_;
    uint32_t createdAt_;

public:
    InternalMessageHeader(bool ihrDisabled, bool bounce, MsgAddressInt dst, uint64_t value) :
        ihrDisabled_(ihrDisabled),
        bounce_(bounce),
        dst_(dst),
        value_(static_cast<uint128_t>(value)),
        ihrFee_(0),
        fwdFee_(0),
        createdLt_(0),
        createdAt_(0)
    {}

    void writeTo(CellBuilder& builder) const override;
};

class Message {
private:
    std::shared_ptr<CommonMsgInfo> header_;

    std::optional<StateInit> init_;
    std::optional<CellSlice> body_;

public:
    using HeaderRef = std::shared_ptr<CommonMsgInfo>;

    Message(HeaderRef header) : header_(header) {}

    Cell::Ref intoCell() const;
    inline void setBody(CellSlice body) { body_ = body; }
    inline void setStateInit(StateInit stateInit) { init_ = stateInit; }
};

}
