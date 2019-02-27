// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Icon.pb.h"
#include "../Data.h"

namespace TW {
namespace Icon {

class Signer {
public:
    /// Signs an Icon transaction.
    static Data sign(const Proto::SigningInput& input) noexcept;

    /// Signature pre-image.
    static std::string preImage(const Proto::SigningInput& input) noexcept;
};

}} // namespace
