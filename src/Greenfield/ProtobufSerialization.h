// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "proto/Greenfield.pb.h"
#include "proto/Common.pb.h"
#include "PublicKey.h"
#include "Result.h"

#include <nlohmann/json.hpp>

namespace TW::Greenfield {

using json = nlohmann::json;
template <typename T>
using SigningResult = Result<T, Common::Proto::SigningError>;

struct ProtobufSerialization {
    // Returns a JSON `{ "mode": "$MODE", "tx_bytes": "$TX_BASE64" }`,
    // where `TX_BASE64` is a serialized Protobuf packaged encoded into base64.
    static SigningResult<json> encodeTxProtobuf(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature);

    // Returns a serialized `cosmos::TxBody` Protobuf package.
    static SigningResult<Data> encodeTxBody(const Proto::SigningInput& input);

    // Returns a serialized `cosmos::AuthInfo` Protobuf package.
    static Data encodeAuthInfo(const Proto::SigningInput& input, const PublicKey& publicKey);
};

} // namespace TW::Greenfield
