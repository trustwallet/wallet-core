#pragma once

#include <nlohmann/json.hpp>

#include "../Bravo/Serialization.h"
#include "Action.h"
#include "../Data.h"
#include "../PrivateKey.h"

#include <set>
#include <array>

namespace TW {
namespace EOS {

    class Signature: Bravo::Serializable
    {
    private:
        static const unsigned long DataSize = 65;

    public:
        Signature(Data sig);
        const Data data;
        void serialize(Data& os) const noexcept;
        std::string string() const noexcept;
    };

    class Extensions: Bravo::Serializable {
    public:
        uint16_t type;
        Data buffer;
        void serialize(Data& os) const noexcept;
        nlohmann::json serialize() const noexcept;
    };

    class Transaction: Bravo::Serializable
    {
    public:
        Transaction(const std::string& referenceBlockId, uint32_t referenceBlockTime);

        void serialize(Data& os) const noexcept;
        nlohmann::json serialize() const noexcept;

		inline bool isValid() { return maxNetUsageWords < UINT32_MAX / 8UL; }

        uint16_t refBlockNumber = 0;
        uint32_t refBlockPrefix = 0;
        uint32_t expiration = 0;
        uint32_t maxNetUsageWords = 0;
        uint8_t maxCPUUsageInMS = 0;
        uint32_t delaySeconds = 0;

        std::vector<Action> actions;
        std::vector<Action> contextFreeActions;
        std::vector<Extensions> transactionExtensions;
        std::vector<Signature> signatures;

        std::vector<Data> contextFreeData;

        void setReferenceBlock(const Data& referenceBlockId);

        static const uint32_t ExpirySeconds = 60 * 10;
    };
}} // namespace

// C interface Wrapper
struct TWEOSTransaction {
    TW::EOS::Transaction impl;
};