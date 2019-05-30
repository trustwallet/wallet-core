// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/Signer.pb.h"

enum SignerErrorCode {
    SignerErrorCodeNotSupported = 1,
    SignerErrorCodeInvalidJson = 2,
};

namespace TW {
/// Helper class to perform json signing
class AnySigner {
public:
    explicit AnySigner(const Signer::Proto::SigningInput& input) : input(input) {}

    Signer::Proto::SigningOutput sign() const noexcept;
private:
    const Signer::Proto::SigningInput& input;

    void parse(const std::string &transaction, google::protobuf::Message *message, Signer::Proto::SigningOutput &output) const noexcept;
};

}

/// Wrapper for C interface.
struct TWAnySigner {
    TW::AnySigner impl;
};

