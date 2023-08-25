// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/Greenfield.pb.h"

#include <nlohmann/json.hpp>

namespace TW::Greenfield {

using json = nlohmann::json;

class SignerEip712 {
public:
    ~SignerEip712() = delete;

    /// Signs a Proto::SigningInput transaction as EIP712.
    /// Returns an rsv signature.
    static Data sign(const Proto::SigningInput& input);

    /// Returns a pre-image hash that needs to be signed.
    static Data preImageHash(const Proto::SigningInput& input);

    /// Packs the Tx input in a EIP712 object.
    static json wrapTxToTypedData(const Proto::SigningInput& input);

    /// Packs the `MsgSend` Tx input in a EIP712 object.
    static json wrapMsgSendToTypedData(const Proto::SigningInput& input);
};

}
