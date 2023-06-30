// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include "../proto/Cosmos.pb.h"

#include <nlohmann/json.hpp>
#include <string>

#include <TrustWalletCore/TWCoinType.h>

namespace TW::Cosmos::Protobuf {

std::string buildProtoTxBody(const Proto::SigningInput& input);

std::string buildAuthInfo(const Proto::SigningInput& input, TWCoinType coin);
std::string buildAuthInfo(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin);

std::string signaturePreimageProto(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin);

std::string buildProtoTxRaw(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature, TWCoinType coin);
std::string buildProtoTxRaw(const std::string& serializedTxBody, const std::string& serializedAuthInfo, const Data& signature);
Data buildSignature(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, TWCoinType coin);

std::string buildProtoTxJson(const Proto::SigningInput& input, const std::string& serializedTx);

nlohmann::json wasmExecuteSendPayload(const Proto::Message_WasmExecuteContractSend& msg);

nlohmann::json wasmExecuteTransferPayload(const Proto::Message_WasmExecuteContractTransfer& msg);

nlohmann::json wasmExecuteSendPayload(const Proto::Message_WasmExecuteContractSend& msg);

nlohmann::json wasmTerraExecuteTransferPayload(const Proto::Message_WasmTerraExecuteContractTransfer& msg);

nlohmann::json wasmTerraExecuteSendPayload(const Proto::Message_WasmTerraExecuteContractSend& msg);

} // namespace TW::Cosmos::protobuf
