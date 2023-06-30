// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
