#pragma once

#include <nlohmann/json.hpp>

#include "Serialization.h"
#include "Operation.h"
#include "../Data.h"
#include "../PrivateKey.h"

#include <set>
#include <array>

namespace TW {
namespace Bravo {

    class Signature: Serializable
    {
    private:
        static const unsigned long DataSize = 65;

    public:
        Signature(Data sig);
        virtual ~Signature() { }
        const Data data;
        void serialize(Data& os) const noexcept;
        std::string string() const noexcept;
    };

    /// for the future
    class FutureExtension: Serializable {
    public:
        virtual ~FutureExtension() = 0;
        virtual void serialize(Data& os) const = 0;
        virtual nlohmann::json serialize() const = 0;
    };

    class Transaction: Serializable
    {
    public:
        Transaction(const std::string& referenceBlockId, uint32_t referenceBlockTime);

        void serialize(Data& os) const noexcept;
        nlohmann::json serialize() const noexcept;

		inline bool isValid() { return !operations.empty(); }
		inline void addOperation(Operation *op) { operations.push_back(std::unique_ptr<Operation>(op)); }
        inline void addOperation(std::unique_ptr<Operation>& op) { operations.push_back(std::move(op)); }
        inline std::vector<Signature> getSignatures() const { return signatures; }
        inline void addSignature(const Signature& sig) { signatures.push_back(sig); }

    private:
        uint16_t refBlockNumber = 0;
        uint32_t refBlockPrefix = 0;
        uint32_t expiration = 0;
        std::vector<std::unique_ptr<Operation>> operations;
        std::set<std::unique_ptr<FutureExtension>> extensions;
        std::vector<Signature> signatures;

        void setReferenceBlock(const Data& referenceBlockId);

        static const uint32_t ExpirySeconds = 60 * 10;
    };
}} // namespace

// C interface Wrapper
struct TWBravoTransaction {
    TW::Bravo::Transaction impl;
};