// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"
#include "wallet/Wallet.h"

#include "proto/TheOpenNetwork.pb.h"

namespace TW::TheOpenNetwork {

/// Helper class that performs TheOpenNetwork transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Creates a signed transfer message
    static Data createTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::Transfer& transfer);

    /// Creates a signed jetton transfer message
    static Data createJettonTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::JettonTransfer& transfer);

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::TheOpenNetwork
