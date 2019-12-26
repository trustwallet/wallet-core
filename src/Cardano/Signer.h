// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../proto/Cardano.pb.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Cardano {

/// Cardano transaction signing.
class Signer {
public:
    static Proto::SigningOutput sign(const Proto::SigningInput& input);

    static const uint64_t Network_Mainnet_Protocol_Magic = 764824073;

    /// Prepare unsigned data for a transaction
    static Data prepareUnsignedTx(const Proto::SigningInput& input);
    static Data prepareSignedTx( const Proto::SigningInput& input, Data& txId_out);
};

} // namespace TW::Cardano

/// Wrapper for C interface.
struct TWCardanoSigner {
    TW::Cardano::Signer impl;
};
