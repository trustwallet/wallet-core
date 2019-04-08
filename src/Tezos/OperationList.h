#pragma once

#include "../Data.h"
#include "proto/Tezos.pb.h"
#include <string>

namespace TW::Tezos {

class OperationList {
  public:
    std::string branch;
    std::vector<TW::Tezos::Proto::Operation> operation_list;
    OperationList(const std::string& string);
    void addOperation(const TW::Tezos::Proto::Operation& transaction);
    /// Returns a data representation of the operations.
    Data forge() const;
    Data forgeBranch() const;
};

} // namespace TW::Tezos

/// Wrapper for C interface.
struct TWTezosOperationList {
    TW::Tezos::OperationList impl;
};
