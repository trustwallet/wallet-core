// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include <optional>

namespace TW::WebAuthn {

std::optional<PublicKey> getPublicKey(const Data& attestationObject);
Data reconstructSignedMessage(const Data& authenticatorData, const Data& clientDataJSON);

}
