// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include <optional>

namespace TW::WebAuthn {

std::optional<PublicKey> getPublicKey(const Data& attestationObject);
Data reconstructSignedMessage(const Data& authenticatorData, const Data& clientDataJSON);

}
