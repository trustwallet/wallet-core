#pragma once

#include <nlohmann/json.hpp>

#include "Serialization.h"
#include "Asset.h"

#include <memory>

namespace TW {
namespace Bravo {

    const static unsigned int MaxMemoSize = 2048;
    const static unsigned int MaxAccountNameSize = 16;

    class Operation: public Serializable
    {
    public:
		virtual void serialize(Data& os) const = 0;
        virtual nlohmann::json serialize() const = 0;
        virtual ~Operation() { };
    };

    class TransferOperation: public Operation {
    public:
        TransferOperation(std::string from, std::string to, Asset asset, std::string memo);
        TransferOperation(std::string from, std::string to, int64_t amount, bool isTestNet, std::string memo);
		TransferOperation(std::string from, std::string to, std::string asset, std::string memo);

        void serialize(Data& os) const noexcept override;
        nlohmann::json serialize() const noexcept override;
		void validate();
        static const int OperationId = 2;

    private:
        std::string from, to, memo;
        Asset asset;

    };
}} // namespace

struct TWBravoOperation {
    std::unique_ptr<TW::Bravo::Operation> implP;
};