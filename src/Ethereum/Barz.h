// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "uint256.h"
#include "PublicKey.h"
#include "../proto/Barz.pb.h"

namespace TW::Barz {

std::string getCounterfactualAddress(const Proto::ContractAddressInput input);
Data getInitCode(const std::string& factoryAddress, const PublicKey& publicKey, const std::string& verificationFacet, const uint32_t salt);
Data getFormattedSignature(const Data& signature, const Data challenge, const Data& authenticatorData, const std::string& clientDataJSON);
Data getPrefixedMsgHash(const Data msgHash, const std::string& barzAddress, const uint32_t chainId);
Data getDiamondCutCode(const Proto::DiamondCutInput& input); // action should be one of 0, 1, 2. 0 = Add, 1 = Remove, 2 = Replace
Data getAuthorizationHash(const Data& chainId, const std::string& contractAddress, const Data& nonce);
std::string signAuthorization(
    const Data& chainId,
    const std::string& contractAddress,
    const Data& nonce,
    const std::string& privateKey);
Data getEncodedHash(
    const Data& chainId,
    const std::string& codeAddress,
    const std::string& codeName,
    const std::string& codeVersion,
    const std::string& typeHash,
    const std::string& domainSeparatorHash,
    const std::string& sender,
    const std::string& userOpHash);
Data getSignedHash(const std::string& hash, const std::string& privateKey);

}
