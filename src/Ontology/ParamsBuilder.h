// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>
#include <string>
#include <vector>
#include <stdexcept>

#include <boost/any.hpp>

#include "BinaryCoding.h"

namespace TW {
namespace Ontology {

class ParamsBuilder {

private:

    std::vector<uint8_t> bytes;

    static const uint8_t PUSH0 = 0x00;
    static const uint8_t PUSH_DATA1 = 0x4C;
    static const uint8_t PUSH_DATA2 = 0x4D;
    static const uint8_t PUSH_DATA4 = 0x4E;
    static const uint8_t PACK = 0xC1;
    static const uint8_t NEW_STRUCT = 0xC6;
    static const uint8_t SYS_CALL = 0x68;
    static const uint8_t DUP_FROM_ALT_STACK = 0x6A;
    static const uint8_t TO_ALT_STACK = 0x6B;
    static const uint8_t FROM_ALT_STACK = 0x6C;
    static const uint8_t SWAP = 0x7C;
    static const uint8_t HAS_KEY = 0xC8;

public:

    std::vector<uint8_t> getBytes() {
        return bytes;
    }

    void cleanUp() {
        bytes.clear();
    }

    static void buildNeoVmParam(ParamsBuilder &builder, const boost::any &param);

    static void buildNeoVmParam(ParamsBuilder &builder, const std::string &param);

    static void buildNeoVmParam(ParamsBuilder &builder, const std::array<uint8_t, 20> &param);

    static void buildNeoVmParam(ParamsBuilder &builder, const std::vector<uint8_t> &param);

    void pushVar(const std::vector<uint8_t> &data);

    template<typename T>
    void pushVar(T data);

    void push(const std::string &data);

    void push(const std::array<uint8_t, 20> &data);

    void push(const std::vector<uint8_t> &data);

    void push(uint64_t num);

    void push(uint8_t num);

    void pushBack(uint8_t data);

    void pushBack(uint32_t data);

    void pushBack(uint64_t data);

    void pushBack(const std::string &data);

    void pushBack(const std::array<uint8_t, 20> &data);

    template<typename T>
    void pushBack(const std::vector<T> &data);

    static std::vector<uint8_t> buildNativeInvokeCode(const std::vector<uint8_t> &contractAddress, uint8_t version, const std::string &method, const boost::any &params);

};

}} // namespace