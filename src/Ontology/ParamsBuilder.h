// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../BinaryCoding.h"
#include "../Data.h"

#include <boost/any.hpp>

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

namespace TW::Ontology {

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

    static void buildNeoVmParam(ParamsBuilder& builder, const boost::any& param);

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
                                                      const boost::any& params);
};

} // namespace TW::Ontology