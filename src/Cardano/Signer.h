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
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    static const uint64_t Network_Mainnet_Protocol_Magic = 764824073;

    // Helper methods
    /// Prepare transaction inputs and outputs, computes fee.  Uses input; sets output.transaction, output.fee.
    static Proto::SigningOutput buildTransaction(const Proto::SigningInput& input);
    /// Prepare unsigned Tx data (Cbor).  Uses input and output.transaction; returns unisgnedEncodedCborData.
    static Data prepareUnsignedTx(const Proto::SigningInput& input, const Proto::SigningOutput& output);
    /// Prepare signed Tx data (Cbor).  Uses input, unisgnedEncodedCborData; sets output.encoded and output.transaction_id.
    static void prepareSignedTx(const Proto::SigningInput& input, const Data& unisgnedEncodedCborData, Proto::SigningOutput& output);
};

} // namespace TW::Cardano

/// Wrapper for C interface.
struct TWCardanoSigner {
    TW::Cardano::Signer impl;
};
