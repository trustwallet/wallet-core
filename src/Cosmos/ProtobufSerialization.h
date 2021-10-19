// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include "Data.h"

#include <string>

namespace TW::Cosmos {

std::string buildProtoTxBody(const Proto::SigningInput& input) noexcept;

std::string buildAuthInfo(const Proto::SigningInput& input) noexcept;

Data buildSignature(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo) noexcept;

std::string buildProtoTxRaw(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, const Data& signature) noexcept;

} // namespace
