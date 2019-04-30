#pragma once

#include <nlohmann/json.hpp>

#include "Asset.h"
#include "Serialization.h"

#include <memory>

namespace TW::Bravo {

const static unsigned int MaxMemoSize = 2048;
const static unsigned int MaxAccountNameSize = 16;

class Operation {
  public:
    virtual void serialize(Data& os) const = 0;
    virtual nlohmann::json serialize() const = 0;
    virtual ~Operation(){};
};

class TransferOperation : public Operation {
  public:
    TransferOperation(const std::string& from, const std::string& to, const Asset& asset,
                      const std::string& memo);
    TransferOperation(const std::string& from, const std::string& to, int64_t amount,
                      bool isTestNet, const std::string& memo);
    TransferOperation(const std::string& from, const std::string& to, const std::string& asset,
                      const std::string& memo);

    void serialize(Data& os) const noexcept override;
    nlohmann::json serialize() const noexcept override;
    void validate();
    static const int OperationId = 2;

  private:
    std::string from, to, memo;
    Asset asset;
};
} // namespace