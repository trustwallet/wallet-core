// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../BinaryCoding.h"
#include "Data.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>
#include <list>

#include "Address.h"

namespace TW::Ontology {

struct NeoVmParamValue;

struct NeoVmParamValue {
    using ParamFixedArray = std::array<uint8_t, 20>;
    using ParamList = std::list<std::variant<std::string, ParamFixedArray, Data, uint64_t>>;
    using ParamArray = std::vector<std::variant<std::string, ParamFixedArray, Data, uint64_t, ParamList>>;
    using ParamVariant = std::variant<std::string, ParamFixedArray, Data, uint64_t, ParamArray, ParamList>;
    ParamVariant params;
};

class ParamsBuilder {

private:
    std::vector<uint8_t> bytes;

public:
    static const size_t MAX_PK_SIZE = 16;

    std::vector<uint8_t> getBytes() { return bytes; }

    void cleanUp() { bytes.clear(); }

    static Data fromSigs(const std::vector<Data>& sigs);

    static Data fromPubkey(const Data& publicKey);

    static Data fromMultiPubkey(uint8_t m, const std::vector<Data>& pubKeys);

    static void buildNeoVmParam(ParamsBuilder& builder, const NeoVmParamValue& param);

    static void buildNeoVmParam(ParamsBuilder& builder, const std::string& param);

    static void buildNeoVmParam(ParamsBuilder& builder, const std::array<uint8_t, 20>& param);

    static void buildNeoVmParam(ParamsBuilder& builder, const std::vector<uint8_t>& param);

    void pushVar(const std::vector<uint8_t>& data);

    void pushVar(std::size_t value);

    void push(const std::string& data);

    void push(const std::array<uint8_t, 20>& data);

    void push(const std::vector<uint8_t>& data);

    void push(uint64_t num, uint8_t len);

    void push(uint64_t num);

    void push(uint8_t num);

    void pushBack(uint8_t data);

    void pushBack(uint32_t data);

    void pushBack(uint64_t data);

    void pushBack(const std::string& data);

    void pushBack(const std::array<uint8_t, 20>& data);

    template <typename T>
    void pushBack(const std::vector<T>& data) {
        bytes.insert(bytes.end(), data.begin(), data.end());
    }

    static std::vector<uint8_t> buildNativeInvokeCode(const std::vector<uint8_t>& contractAddress,
                                                      uint8_t version, const std::string& method,
                                                      const NeoVmParamValue& params);

    static std::vector<uint8_t> buildOep4InvokeCode(const Address& contractAddress, const std::string& method, const NeoVmParamValue& params);
};

} // namespace TW::Ontology
