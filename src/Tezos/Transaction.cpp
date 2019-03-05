// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "PublicKey.h"
#include "TWTezosForger.h"
#include "Transaction.h"
#include "../proto/Tezos.pb.h"
#include <variant>

using namespace std;
using namespace TW::Tezos;

string Transaction::serialize() {
  auto forgedSource = source.forge();
  auto forgedFee = forgeZarith(fee);
  auto forgedCounter = forgeZarith(counter);
  auto forgedGasLimit = forgeZarith(gas_limit);
  auto forgedStorageLimit = forgeZarith(storage_limit);

  if (transaction_type == TransactionType::REVEAL) {
    auto forgedPublicKey = std::get<PublicKey>(destination_or_public_key).forge();

    return "07" + forgedSource + forgedFee + forgedCounter + forgedGasLimit
        + forgedStorageLimit + forgedPublicKey;
  }
  auto forgedAmount = forgeZarith(amount);
  auto forgedDestination = std::get<Address>(destination_or_public_key).forge();

  return "08" + forgedSource + forgedFee + forgedCounter + forgedGasLimit
      + forgedStorageLimit + forgedAmount + forgedDestination + forgeBool(false);
}
