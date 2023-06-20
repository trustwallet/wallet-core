// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "uint256.h"
#include "../proto/Barz.pb.h"

namespace TW::Barz {

std::string getCounterfactualAddress(const Proto::ContractAddressInput input);
Data getInitCodeFromPublicKey(const std::string& factoryAddress, const std::string& owner, const std::string& verificationFacet);
Data getInitCodeFromAttestationObject(const std::string& factoryAddress, const std::string& attestationObject, const std::string& verificationFacet);
Data getFormattedSignature(const Data& signature, const Data challenge, const Data& authenticatorData, const std::string& clientDataJSON);

}
