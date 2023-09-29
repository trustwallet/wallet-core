// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/EthereumAbi.pb.h"
#include "uint256.h"

namespace TW::Ethereum::ABI {

namespace AbiProto = EthereumAbi::Proto;

struct BaseProtoParam {
    virtual ~BaseProtoParam() noexcept = default;

    virtual AbiProto::Token toToken() const = 0;
};

using BaseParams = std::vector<std::shared_ptr<BaseProtoParam>>;

class ProtoBool final: public BaseProtoParam {
public:
    explicit ProtoBool(bool val): m_value(val) {
    }

    ~ProtoBool() override = default;

    AbiProto::Token toToken() const override {
        AbiProto::Token proto;
        proto.set_boolean(m_value);
        return proto;
    }

private:
    bool m_value = false;
};

class ProtoUInt256 final: public BaseProtoParam {
public:
    explicit ProtoUInt256(const uint256_t &num): m_number(store(num)) {
    }

    explicit ProtoUInt256(Data numData): m_number(std::move(numData)) {
    }

    ~ProtoUInt256() override = default;

    AbiProto::Token toToken() const override {
        AbiProto::Token proto;
        proto.mutable_number_uint()->set_bits(256);
        proto.mutable_number_uint()->set_value(m_number.data(), m_number.size());
        return proto;
    }

private:
    Data m_number;
};

class ProtoByteArray final: public BaseProtoParam {
public:
    explicit ProtoByteArray(Data data): m_data(std::move(data)) {
    }

    ~ProtoByteArray() override = default;

    AbiProto::Token toToken() const override {
        AbiProto::Token proto;
        proto.set_byte_array(m_data.data(), m_data.size());
        return proto;
    }

private:
    Data m_data;
};

class ProtoString final: public BaseProtoParam {
public:
    explicit ProtoString(std::string str): m_string(std::move(str)) {
    }

    ~ProtoString() override = default;

    AbiProto::Token toToken() const override {
        AbiProto::Token proto;
        proto.set_string_value(m_string.data(), m_string.size());
        return proto;
    }

private:
    std::string m_string;
};

class ProtoAddress final: public BaseProtoParam {
public:
    ProtoAddress() = default;

    explicit ProtoAddress(std::string addr): m_address(std::move(addr)) {
    }

    ~ProtoAddress() override = default;

    AbiProto::Token toToken() const override {
        AbiProto::Token proto;
        proto.set_address(m_address);
        return proto;
    }

private:
    std::string m_address {"0x0000000000000000000000000000000000000000"};
};

} // namespace TW::Ethereum::ABI
