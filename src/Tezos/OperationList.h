#pragma once

#include "../Data.h"
#include "proto/Tezos.pb.h"
#include "../PrivateKey.h"
#include <string>

using namespace TW::Tezos;
using namespace TW::Tezos::Proto;

namespace TW::Tezos {

class OperationList {
  public:
    std::string branch;
    std::vector<Operation> operation_list;
    OperationList(const std::string& string);
    void addOperation(const Operation& transaction);
    /// Returns a data representation of the operations.
    Data forge(const PrivateKey& privateKey) const;
    Data forgeBranch() const;
};

} // namespace TW::Tezos
