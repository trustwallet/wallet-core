// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "OperationList.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Tezos.pb.h"

#include <string>

namespace TW::Tezos {

/// Helper class that performs Tezos transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

  public:
    /// Signs the given transaction.
    Data signOperationList(const PrivateKey& privateKey, const OperationList& operationList);
    Data buildUnsignedTx(const OperationList& operationList);
    Data buildSignedTx(const OperationList& operationList, Data signature);
    Data signData(const PrivateKey& privateKey, const Data& data);
};

} // namespace TW::Tezos
