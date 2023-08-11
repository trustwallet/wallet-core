// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "uint256.h"
#include "PublicKey.h"
#include "../proto/Barz.pb.h"

namespace TW::Barz {

std::string getCounterfactualAddress(const Proto::ContractAddressInput input);
Data getInitCode(const std::string& factoryAddress, const PublicKey& publicKey, const std::string& verificationFacet, const uint32_t salt);
Data getFormattedSignature(const Data& signature, const Data challenge, const Data& authenticatorData, const std::string& clientDataJSON);

}
