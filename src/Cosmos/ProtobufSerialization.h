// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include "PublicKey.h"
#include "Data.h"

#include <string>
#include <nlohmann/json.hpp>

using string = std::string;
using json = nlohmann::json;

extern const string TYPE_PREFIX_MSG_SEND;
extern const string TYPE_PREFIX_MSG_DELEGATE;
extern const string TYPE_PREFIX_MSG_UNDELEGATE;
extern const string TYPE_PREFIX_MSG_REDELEGATE;
extern const string TYPE_PREFIX_MSG_WITHDRAW_REWARD;
extern const string TYPE_PREFIX_PUBLIC_KEY;

#include <TrustWalletCore/TWCoinType.h>

namespace TW::Cosmos {

std::string buildProtoTxBody(const Proto::SigningInput& input);

std::string buildAuthInfo(const Proto::SigningInput& input, TWCoinType coin);
std::string buildAuthInfo(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin);

std::string signaturePreimageProto(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin);

std::string buildProtoTxRaw(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature, TWCoinType coin);
std::string buildProtoTxRaw(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, const Data& signature);
Data buildSignature(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, TWCoinType coin);

std::string buildProtoTxJson(const Proto::SigningInput& input, const std::string& serializedTx);

nlohmann::json wasmTerraExecuteTransferPayload(const Proto::Message_WasmTerraExecuteContractTransfer& msg);

nlohmann::json wasmTerraExecuteSendPayload(const Proto::Message_WasmTerraExecuteContractSend& msg);

} // namespace
