// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Serialization.h"

#include "../BinaryCoding.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <nlohmann/json.hpp>

namespace TW::NEAR {

using json = nlohmann::json;
using SigningResult = Result<void, Common::Proto::SigningError>;

static constexpr auto tokenTransferMethodName = "ft_transfer";

// NEAR `KeyType` values and their fixed Borsh-encoded key sizes.
static constexpr uint32_t keyTypeED25519 = 0;
static constexpr uint32_t keyTypeSECP256K1 = 1;
// SECP256K1 keys are uncompressed, without the 0x04 prefix.
static constexpr std::size_t secp256k1KeySize = 64;

static void writeU8(Data& data, uint8_t number) {
    data.push_back(number);
}

static void writeU32(Data& data, uint32_t number) {
    encode32LE(number, data);
}

static void writeU64(Data& data, uint64_t number) {
    encode64LE(number, data);
}

static SigningResult writeU128(Data& data, const std::string& numberData) {
    if (numberData.size() != 16) {
        return SigningResult::failure(Common::Proto::Error_invalid_params);
    }
    data.insert(std::end(data), std::begin(numberData), std::end(numberData));
    return SigningResult::success();
}

template <class T>
static void writeRawBuffer(Data& data, const T& buf) {
    data.insert(std::end(data), std::begin(buf), std::end(buf));
}

static void writeString(Data& data, const std::string& str) {
    writeU32(data, static_cast<uint32_t>(str.length()));
    writeRawBuffer(data, str);
}

// Public key data is not length-prefixed in Borsh, so its size must match the key type exactly.
static SigningResult writePublicKey(Data& data, const Proto::PublicKey& publicKey) {
    const auto& keyData = publicKey.data();
    switch (publicKey.key_type()) {
    case keyTypeED25519:
        if (keyData.size() != PublicKey::ed25519Size) {
            return SigningResult::failure(Common::Proto::Error_invalid_params);
        }
        break;
    case keyTypeSECP256K1:
        if (keyData.size() != secp256k1KeySize) {
            return SigningResult::failure(Common::Proto::Error_invalid_params);
        }
        break;
    default:
        return SigningResult::failure(Common::Proto::Error_invalid_params);
    }
    writeU8(data, static_cast<uint8_t>(publicKey.key_type()));
    writeRawBuffer(data, keyData);
    return SigningResult::success();
}

static void writeDeployContract(Data& data, const Proto::DeployContract& deployContract) {
    writeU32(data, static_cast<uint32_t>(deployContract.code().size()));
    writeRawBuffer(data, deployContract.code());
}

static SigningResult writeTransfer(Data& data, const Proto::Transfer& transfer) {
    return writeU128(data, transfer.deposit());
}

static SigningResult writeFunctionCall(Data& data, const Proto::FunctionCall& functionCall) {
    writeString(data, functionCall.method_name());

    writeU32(data, static_cast<uint32_t>(functionCall.args().size()));
    writeRawBuffer(data, functionCall.args());

    writeU64(data, functionCall.gas());
    return writeU128(data, functionCall.deposit());
}

static SigningResult writeStake(Data& data, const Proto::Stake& stake) {
    // NEAR only accepts ED25519 validator staking keys.
    if (stake.public_key().key_type() != keyTypeED25519) {
        return SigningResult::failure(Common::Proto::Error_invalid_params);
    }
    auto result = writeU128(data, stake.stake());
    if (!result) {
        return result;
    }
    return writePublicKey(data, stake.public_key());
}

static SigningResult writeFunctionCallPermission(Data& data, const Proto::FunctionCallPermission& functionCallPermission) {
    if (functionCallPermission.allowance().empty()) {
        writeU8(data, 0);
    } else {
        writeU8(data, 1);
        auto result = writeU128(data, functionCallPermission.allowance());
        if (!result) {
            return result;
        }
    }
    writeString(data, functionCallPermission.receiver_id());
    writeU32(data, static_cast<uint32_t>(functionCallPermission.method_names().size()));
    for (auto&& methodName : functionCallPermission.method_names()) {
        writeString(data, methodName);
    }
    return SigningResult::success();
}

static SigningResult writeAccessKey(Data& data, const Proto::AccessKey& accessKey) {
    writeU64(data, accessKey.nonce());
    switch (accessKey.permission_case()) {
    case Proto::AccessKey::kFunctionCall:
        writeU8(data, 0);
        return writeFunctionCallPermission(data, accessKey.function_call());
    case Proto::AccessKey::kFullAccess:
        writeU8(data, 1);
        break;
    case Proto::AccessKey::PERMISSION_NOT_SET:
        break;
    }
    return SigningResult::success();
}

static SigningResult writeAddKey(Data& data, const Proto::AddKey& addKey) {
    auto result = writePublicKey(data, addKey.public_key());
    if (!result) {
        return result;
    }
    return writeAccessKey(data, addKey.access_key());
}

static SigningResult writeDeleteKey(Data& data, const Proto::DeleteKey& deleteKey) {
    return writePublicKey(data, deleteKey.public_key());
}

static void writeDeleteAccount(Data& data, const Proto::DeleteAccount& deleteAccount) {
    writeString(data, deleteAccount.beneficiary_id());
}

static SigningResult writeTokenTransfer(Data& data, const Proto::TokenTransfer& tokenTransfer) {
    writeString(data, tokenTransferMethodName);

    json functionCallArgs = {
        {"amount", tokenTransfer.token_amount()},
        {"receiver_id", tokenTransfer.receiver_id()},
    };
    auto functionCallArgsStr = functionCallArgs.dump();

    writeU32(data, static_cast<uint32_t>(functionCallArgsStr.size()));
    writeRawBuffer(data, functionCallArgsStr);

    writeU64(data, tokenTransfer.gas());
    return writeU128(data, tokenTransfer.deposit());
}

static SigningResult writeAction(Data& data, const Proto::Action& action) {
    uint8_t actionByte = action.payload_case() - Proto::Action::kCreateAccount;
    // `TokenTransfer` action is actually a `FunctionCall`,
    // so we need to set the actionByte to the proper value.
    if (action.payload_case() == Proto::Action::kTokenTransfer) {
        actionByte = Proto::Action::kFunctionCall - Proto::Action::kCreateAccount;
    }

    writeU8(data, actionByte);
    switch (action.payload_case()) {
    case Proto::Action::kCreateAccount:
        return SigningResult::success();
    case Proto::Action::kDeployContract:
        writeDeployContract(data, action.deploy_contract());
        return SigningResult::success();
    case Proto::Action::kFunctionCall:
        return writeFunctionCall(data, action.function_call());
    case Proto::Action::kTransfer:
        return writeTransfer(data, action.transfer());
    case Proto::Action::kStake:
        return writeStake(data, action.stake());
    case Proto::Action::kAddKey:
        return writeAddKey(data, action.add_key());
    case Proto::Action::kDeleteKey:
        return writeDeleteKey(data, action.delete_key());
    case Proto::Action::kDeleteAccount:
        writeDeleteAccount(data, action.delete_account());
        return SigningResult::success();
    case Proto::Action::kTokenTransfer:
        return writeTokenTransfer(data, action.token_transfer());
    case Proto::Action::PAYLOAD_NOT_SET:
        break;
    }
    // No `default` above, so a new action added to the proto triggers -Wswitch until it is encoded here.
    // An action with no payload (or an unknown one) has no valid encoding.
    return SigningResult::failure(Common::Proto::Error_invalid_params);
}

// Serializes the transaction with `signerKey` as the signer's ED25519 public key.
static Result<Data, Common::Proto::SigningError> serializeTransaction(const Proto::SigningInput& input, const Data& signerKey) {
    if (input.block_hash().size() != 32) {
        return Result<Data, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_params);
    }
    Data data;
    writeString(data, input.signer_id());
    auto public_key_proto = Proto::PublicKey();
    public_key_proto.set_data(signerKey.data(), signerKey.size());
    auto keyResult = writePublicKey(data, public_key_proto);
    if (!keyResult) {
        return Result<Data, Common::Proto::SigningError>::failure(keyResult.error());
    }
    writeU64(data, input.nonce());
    writeString(data, input.receiver_id());
    writeRawBuffer(data, input.block_hash());
    writeU32(data, input.actions_size());
    for (const auto& action : input.actions()) {
        auto result = writeAction(data, action);
        if (!result) {
            return Result<Data, Common::Proto::SigningError>::failure(result.error());
        }
    }
    return Result<Data, Common::Proto::SigningError>::success(std::move(data));
}

Result<Data, Common::Proto::SigningError> transactionData(const Proto::SigningInput& input) {
    auto key = PrivateKey(input.private_key(), TWCurveED25519);
    return serializeTransaction(input, key.getPublicKey(TWPublicKeyTypeED25519).bytes);
}

Result<Data, Common::Proto::SigningError> transactionDataWithPublicKey(const Proto::SigningInput& input) {
    return serializeTransaction(input, Data(input.public_key().begin(), input.public_key().end()));
}

Data signedTransactionData(const Data& transactionData, const Data& signatureData) {
    Data data;
    writeRawBuffer(data, transactionData);
    writeU8(data, 0);
    writeRawBuffer(data, signatureData);
    return data;
}

} // namespace TW::NEAR
