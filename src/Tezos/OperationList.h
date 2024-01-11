// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "proto/Tezos.pb.h"
#include "../PrivateKey.h"
#include <string>

namespace TW::Tezos {

using TW::Tezos::Proto::Operation;

class OperationList {
  public:
    std::string branch;
    std::vector<Operation> operation_list;
    OperationList(const std::string& string);
    void addOperation(const Operation& transaction);
    /// Returns a data representation of the operations.
    Data forge(const PrivateKey& privateKey) const;
    Data forge() const;
    Data forgeBranch() const;
};

} // namespace TW::Tezos
