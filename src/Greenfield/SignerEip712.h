// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/Greenfield.pb.h"
#include "Result.h"

#include <nlohmann/json.hpp>

namespace TW::Greenfield {

using json = nlohmann::json;
template <typename T>
using SigningResult = Result<T, Common::Proto::SigningError>;

struct Eip712PreImage {
    json typedData;
    Data typedDataHash;
};

struct SignerEip712 {
    ~SignerEip712() = delete;

    /// Signs a Proto::SigningInput transaction as EIP712.
    /// Returns an rsv signature.
    static SigningResult<Data> sign(const Proto::SigningInput& input);

    /// Returns a pre-image hash that needs to be signed.
    static SigningResult<Eip712PreImage> preImageHash(const Proto::SigningInput& input);

    /// Packs the Tx input in a EIP712 object.
    static SigningResult<json> wrapTxToTypedData(const Proto::SigningInput& input);

    /// Packs the `MsgSend` Tx input in a EIP712 object.
    static json wrapMsgSendToTypedData(const Proto::SigningInput& input, const Proto::Message_Send& msgSendProto);

    /// Packs the `MsgTransferOut` Tx input in a EIP712 object.
    static json wrapMsgTransferOutToTypedData(const Proto::SigningInput& input, const Proto::Message_BridgeTransferOut& msgTransferOut);

    /// Prepares the given `signature` to make it Ethereum compatible.
    static void prepareSignature(Data& signature);
};

} // namespace TW::Greenfield
