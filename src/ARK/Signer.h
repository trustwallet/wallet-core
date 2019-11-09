// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../PrivateKey.h"
#include "../proto/ARK.pb.h"

namespace TW::ARK {
class Signer {
  public:
    static Proto::SigningOutput sign(PrivateKey &privateKey, Transaction &tx);
};

}; // namespace TW::ARK

// Wrapper for C interface.
struct TWARKSigner {
    TW::ARK::Signer impl;
};
