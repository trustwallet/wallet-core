// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/Common.pb.h"

namespace TW::Common {

struct SigningError {
    Proto::SigningError code;
    std::string text;
    SigningError() : code(Proto::OK), text("") {}
    SigningError(Proto::SigningError code, std::string text) : code(code), text(text) {}
    bool hasError() const { return code != Proto::OK || text.length() > 0; }
    bool operator==(const SigningError& error2) const { return code == error2.code && text == error2.text; }
    bool operator!=(const SigningError& error2) const { return !(*this == error2); }
    std::string toString() const { return std::to_string(code) + ": " + text; }
};

} // namespace TW::Common
