// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "TWTezosForger.h"

using namespace TW;
using namespace TW::Tezos;

Address::Address(const std::string& str) {
  branch = str;
}

void add_operation(Transaction transaction) {
  operation_list.push_back(transaction);
}

std::string OperationList::forge() const {
  // TODO: branch can be refactored to a general datatype with prefix and content
  std::string result = forgeBranch(branch);

  for (int i = 0; i < transaction_list.size(); i++) {
    Transaction t = operation_list[i];
    result += forgeOperation(operation);
  }
  return result;
}
