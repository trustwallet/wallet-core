// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>
#include <nlohmann/json.hpp>

#include <vector>

#define WRAP(type, x) std::shared_ptr<type>(x, type##Delete)
#define WRAPD(x) std::shared_ptr<TWData>(x, TWDataDelete)
#define WRAPS(x) std::shared_ptr<TWString>(x, TWStringDelete)
#define STRING(x) std::shared_ptr<TWString>(TWStringCreateWithUTF8Bytes(x), TWStringDelete)
#define DATA(x) std::shared_ptr<TWData>(TWDataCreateWithHexString(STRING(x).get()), TWDataDelete)

inline void assertStringsEqual(const std::shared_ptr<TWString>& string, const char* expected) {
    ASSERT_STREQ(TWStringUTF8Bytes(string.get()), expected);
}

inline void assertHexEqual(const std::shared_ptr<TWData>& data, const char* expected) {
    auto hex = WRAPS(TWStringCreateWithHexData(data.get()));
    assertStringsEqual(hex, expected);
}


inline void assertJSONEqual(const nlohmann::json& lhs, const nlohmann::json& rhs) {
    ASSERT_EQ(lhs, rhs);
}

inline void assertJSONEqual(const std::string& lhs, const char* expected) {
    auto lhsJson = nlohmann::json::parse(lhs);
    auto rhsJson = nlohmann::json::parse(std::string(expected));
    return assertJSONEqual(lhsJson, rhsJson);
}

inline std::vector<uint8_t>* dataFromTWData(TWData* data) {
    return const_cast<std::vector<uint8_t>*>(reinterpret_cast<const std::vector<uint8_t>*>(data));
}

/// Return a writable temp dir which can be used to create files during testing
std::string getTestTempDir(void);

#define ANY_SIGN(input, coin) \
        {\
            auto inputData = input.SerializeAsString();\
            auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));\
            auto outputTWData = WRAPD(TWAnySignerSign(inputTWData.get(), coin));\
            output.ParseFromArray(TWDataBytes(outputTWData.get()), static_cast<int>(TWDataSize(outputTWData.get())));\
        }
#define ANY_PLAN(input, output, coin) \
        {\
            auto inputData = input.SerializeAsString();\
            auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));\
            auto outputTWData = WRAPD(TWAnySignerPlan(inputTWData.get(), coin));\
            output.ParseFromArray(TWDataBytes(outputTWData.get()), static_cast<int>(TWDataSize(outputTWData.get())));\
        }
#define DUMP_PROTO(input) \
        { \
            std::string json; \
            google::protobuf::util::MessageToJsonString(input, &json); \
            std::cout<<"dump proto: "<<json<<std::endl; \
        }

/// For tests which should throw.  Wrap code under test in this macro.  
/// ExceptionMsg is the expected exception message (startsWith match)
#define EXPECT_EXCEPTION(statement, expExceptionMsg) \
    try { \
        statement; \
        FAIL() << "An exception was expected, but none was thrown"; \
    } catch (const std::exception& ex) { \
        const std::string expEx = expExceptionMsg; \
        const std::string actEx = ex.what(); \
        const auto actExPrefix = actEx.substr(0, expEx.length()); \
        EXPECT_EQ(actExPrefix, expEx); \
    } catch (...) { \
        FAIL() << "Not the expected exception"; \
    }

/// Open a json file
nlohmann::json loadJson(std::string path);
