// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ParamsBuilder.h"
#include "Data.h"
#include "OpCode.h"

#include <TrezorCrypto/bignum.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/nist256p1.h>

#include <algorithm>
#include <list>

namespace TW::Ontology {

void ParamsBuilder::buildNeoVmParam(ParamsBuilder& builder, const NeoVmParamValue& param) {

    if (auto* paramStr = std::get_if<std::string>(&param.params); paramStr) {
        builder.push(*paramStr);
    } else if (auto* paramFixedArray = std::get_if<NeoVmParamValue::ParamFixedArray>(&param.params); paramFixedArray) {
        builder.push(*paramFixedArray);
    } else if (auto* paramData = std::get_if<Data>(&param.params); paramData) {
        builder.push(*paramData);
    } else if (auto* paramInteger = std::get_if<uint64_t>(&param.params); paramInteger) {
        builder.push(*paramInteger);
    } else if (auto* paramArray = std::get_if<NeoVmParamValue::ParamArray>(&param.params); paramArray) {
        for (auto&& item : *paramArray) {
            std::visit([&builder](auto&& arg) {
                NeoVmParamValue::ParamVariant value = arg;
                ParamsBuilder::buildNeoVmParam(builder, {value});
            },
                       item);
        }
        builder.push(static_cast<uint8_t>(paramArray->size()));
        builder.pushBack(PACK);
    } else if (auto* paramList = std::get_if<NeoVmParamValue::ParamList>(&param.params); paramList) {
        builder.pushBack(PUSH0);
        builder.pushBack(NEW_STRUCT);
        builder.pushBack(TO_ALT_STACK);
        for (auto const& p : *paramList) {
            std::visit([&builder](auto&& arg) {
                NeoVmParamValue::ParamVariant value = arg;
                ParamsBuilder::buildNeoVmParam(builder, {value});
            }, p);
            builder.pushBack(DUP_FROM_ALT_STACK);
            builder.pushBack(SWAP);
            builder.pushBack(HAS_KEY);
        }
        builder.pushBack(FROM_ALT_STACK);
    } else {
        throw std::runtime_error("Unsupported param type.");
    }
}

void ParamsBuilder::buildNeoVmParam(ParamsBuilder& builder, const std::string& param) {
    builder.pushBack(param);
}

void ParamsBuilder::buildNeoVmParam(ParamsBuilder& builder, const std::array<uint8_t, 20>& param) {
    builder.pushBack(Data(param.begin(), param.end()));
}

void ParamsBuilder::buildNeoVmParam(ParamsBuilder& builder, const Data& param) {
    builder.push(param);
}

void ParamsBuilder::pushVar(const Data& data) {
    pushVar(data.size());
    bytes.insert(bytes.end(), data.begin(), data.end());
}

void ParamsBuilder::pushVar(std::size_t value) {
    if (value < 0xFD) {
        ParamsBuilder::pushBack(static_cast<uint8_t>(value));
    } else if (value < 0xFFFF) {
        bytes.push_back(0xFD);
        encode16LE(static_cast<uint16_t>(value), bytes);
    } else if (value < 0xFFFFFFFF) {
        bytes.push_back(0xFE);
        encode32LE(static_cast<uint32_t>(value), bytes);
    } else {
        bytes.push_back(0xFF);
        encode64LE(value, bytes);
    }
}

void ParamsBuilder::push(const std::string& data) {
    push(Data(data.begin(), data.end()));
}

void ParamsBuilder::push(const std::array<uint8_t, 20>& data) {
    push(Data(data.begin(), data.end()));
}

void ParamsBuilder::push(const Data& data) {
    auto dataSize = data.size();
    if (dataSize < 75) {
        bytes.push_back(static_cast<uint8_t>(dataSize));
    } else if (dataSize < 256) {
        bytes.push_back(PUSH_DATA1);
        bytes.push_back(static_cast<uint8_t>(dataSize));
    } else if (dataSize < 65536) {
        bytes.push_back(PUSH_DATA2);
        encode16LE(static_cast<uint16_t>(dataSize), bytes);
    } else {
        bytes.push_back(PUSH_DATA4);
        encode32LE(static_cast<uint16_t>(dataSize), bytes);
    }
    bytes.insert(bytes.end(), data.begin(), data.end());
}

void ParamsBuilder::push(uint64_t num, uint8_t len) {
    Data data;
    for (auto i = 0; i < len; i++) {
        data.push_back(static_cast<uint8_t>(num));
        num >>= 8;
    }
    if (data.back() >> 7 == 1) {
        data.push_back(0x00);
    }
    push(data);
}

void ParamsBuilder::push(uint64_t num) {
    if (num == 0) {
        bytes.push_back(PUSH0);
    } else if (num < 16) {
        num += 80;
        bytes.push_back(static_cast<uint8_t>(num));
    } else if (num < 128) {
        push(Data{static_cast<uint8_t>(num)});
    } else if (num <= 0xFFFF) {
        push(num, 2);
    } else if (num <= 0xFFFFFF) {
        push(num, 3);
    } else if (num <= 0xFFFFFFFF) {
        push(num, 4);
    } else if (num <= 0xFFFFFFFFFF) {
        push(num, 5);
    } else if (num <= 0xFFFFFFFFFFFF) {
        push(num, 6);
    } else if (num <= 0xFFFFFFFFFFFFFF) {
        push(num, 7);
    } else {
        push(num, 8);
    }
}

void ParamsBuilder::pushBack(uint8_t data) {
    bytes.push_back(data);
}

void ParamsBuilder::pushBack(uint32_t data) {
    encode32LE(data, bytes);
}

void ParamsBuilder::pushBack(uint64_t data) {
    encode64LE(data, bytes);
}

void ParamsBuilder::pushBack(const std::string& data) {
    bytes.insert(bytes.end(), data.begin(), data.end());
}

void ParamsBuilder::pushBack(const std::array<uint8_t, 20>& data) {
    bytes.insert(bytes.end(), data.begin(), data.end());
}

void ParamsBuilder::push(uint8_t num) {
    if (num == 0) {
        bytes.push_back(PUSH0);
    } else if (num < 16) {
        num += 80;
        bytes.push_back(static_cast<uint8_t>(num));
    } else if (num < 128) {
        push(Data{num});
    } else {
        push(Data{num, PUSH0});
    }
}

Data ParamsBuilder::fromSigs(const std::vector<Data>& sigs) {
    ParamsBuilder builder;
    for (auto const& sig : sigs) {
        builder.push(sig);
    }
    return builder.getBytes();
}

Data ParamsBuilder::fromPubkey(const Data& publicKey) {
    ParamsBuilder builder;
    builder.push(publicKey);
    builder.pushBack(CHECK_SIG);
    return builder.getBytes();
}

Data ParamsBuilder::fromMultiPubkey(uint8_t m, const std::vector<Data>& pubKeys) {
    if (m > pubKeys.size()) {
        throw std::runtime_error("Invalid m in signature data.");
    }
    if (pubKeys.size() > MAX_PK_SIZE) {
        throw std::runtime_error("Too many public key found.");
    }
    ParamsBuilder builder;
    builder.push(m);
    auto sortedPubKeys = pubKeys;
    std::sort(sortedPubKeys.begin(), sortedPubKeys.end(), [](Data& o1, Data& o2) -> int {
        curve_point p1, p2;
        ecdsa_read_pubkey(&nist256p1, o1.data(), &p1);
        ecdsa_read_pubkey(&nist256p1, o2.data(), &p2);
        auto result = bn_is_less(&p1.x, &p2.x);
        if (result != 0) {
            return result;
        }
        return bn_is_less(&p1.y, &p2.y);
    });
    for (auto const& pk : sortedPubKeys) {
        builder.push(pk);
    }
    builder.push((uint8_t)sortedPubKeys.size());
    builder.pushBack(CHECK_MULTI_SIG);
    return builder.getBytes();
}

Data ParamsBuilder::buildNativeInvokeCode(const Data& contractAddress, uint8_t version,
                                          const std::string& method, const NeoVmParamValue& params) {
    ParamsBuilder builder;
    ParamsBuilder::buildNeoVmParam(builder, params);
    builder.push(Data(method.begin(), method.end()));
    builder.push(contractAddress);
    builder.push(version);
    builder.pushBack(SYS_CALL);
    std::string nativeInvoke = "Ontology.Native.Invoke";
    builder.push(Data(nativeInvoke.begin(), nativeInvoke.end()));
    return builder.getBytes();
}

Data ParamsBuilder::buildOep4InvokeCode(const Address& contractAddress, const std::string& method, const NeoVmParamValue& params) {
    ParamsBuilder builder;
    ParamsBuilder::buildNeoVmParam(builder, params);
    builder.push(method);
    builder.pushBack(APP_CALL);
    Address clone = contractAddress;
    std::reverse(std::begin(clone._data), std::end(clone._data));
    builder.pushBack(clone._data);

    return builder.getBytes();
}

} // namespace TW::Ontology
