// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Action.h"
#include "Prefixes.h"

#include <nlohmann/json.hpp>

#include <set>
#include <array>

namespace TW::EOS {

class Signature {
public:
    Data data;
    Type type;

    static const size_t DataSize = 65;
    static const size_t ChecksumSize = 4;

    Signature(const Data& sig, Type type);
    virtual ~Signature() { }
    void serialize(Data& os) const noexcept;
    std::string string() const noexcept;
};

class Extension {
public:
    uint16_t type;
    Data buffer;

    Extension(uint16_t type, const Data& buffer) : type(type), buffer(buffer) { }
    virtual ~Extension() { }
    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const noexcept;
};

class Transaction {
public:
    Transaction(const Data& referenceBlockId, int32_t referenceBlockTime);

    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const;

    inline bool isValid() { return maxNetUsageWords < UINT32_MAX / 8UL; }

    uint16_t refBlockNumber = 0;
    uint32_t refBlockPrefix = 0;
    int32_t expiration = 0;
    uint32_t maxNetUsageWords = 0;
    uint8_t maxCPUUsageInMS = 0;
    uint32_t delaySeconds = 0;

    std::vector<Action> actions;
    std::vector<Action> contextFreeActions;
    std::vector<Extension> transactionExtensions;
    std::vector<Signature> signatures;

    Data contextFreeData;

    void setReferenceBlock(const Data& referenceBlockId);

    static const int32_t ExpirySeconds = 3600;
    /// Get formatted date
    static std::string formatDate(int32_t date);
};
} // namespace TW::EOS