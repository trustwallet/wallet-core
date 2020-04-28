// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "../Data.h"

namespace TW::NEO {

class Script {
  public:
    static Data CreateSignatureRedeemScript(const Data& publicKey);
    static Data CreateInvocationScript(const Data& signature);
};

} // namespace TW::NEO
