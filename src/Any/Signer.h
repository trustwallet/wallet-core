// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/Any.pb.h"

enum SignerErrorCode {
    SignerErrorCodeNotSupported = 1,
    SignerErrorCodeInvalidJson = 2,
};

namespace TW::Any {
/// Helper class to perform json signing
class Signer {
public:
    explicit Signer(const Proto::SigningInput& input) : input(input) {}

    Proto::SigningOutput sign() const noexcept;
private:
    const Proto::SigningInput& input;

    void parse(const std::string &transaction, google::protobuf::Message *message, Proto::SigningOutput &output) const noexcept;
    void toJson(const google::protobuf::Message &message, std::string *json_string) const noexcept;
};

}

/// Wrapper for C interface.
struct TWAnySigner {
    TW::Any::Signer impl;
};

