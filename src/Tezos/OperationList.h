#pragma once

#include "Transaction"
#include "proto/Tezos.pb.h"
#include <string>

namespace TW {
namespace Tezos {

class OperationList {
public:
    string branch;
    std::vector< Transaction > operation_list;
    OperationList(const std::string& string);
    void add_operation(Transaction transaction);
    /// Returns a string representation of the operations.
    std::string forge() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWTezosOperationList {
    TW::Tezos::OperationList impl;
};
