// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/Json.pb.h"

enum JsonSignErrorCode {
    JsonSignErrorCodeNotSupported = 1,
    JsonSignErrorCodeJsonParseError = 2,
};

namespace TW::Json {
/// Helper class to perform json signing
class Signer {
public:
    const Proto::SigningInput& input;

    explicit Signer(const Proto::SigningInput& input) : input(input) {}

    Proto::SigningOutput sign() const noexcept;
private:
    void parse(const std::string &transaction, google::protobuf::Message *message, Proto::SigningOutput &output) const noexcept;
};

}

/// Wrapper for C interface.
struct TWJsonSigner {
    TW::Json::Signer impl;
};

