// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SignerEip712.h"

#include "Constants.h"
#include "Ethereum/MessageSigner.h"
#include "Ethereum/ABI/ParamStruct.h"
#include "HexCoding.h"

#include <map>

namespace TW::Greenfield {

namespace internal::types {

using TypesMap = std::map<std::string, json, std::less<>>;

json namedParam(const char *name, const char *type) {
    return json {
        {"name", name},
        {"type", type}
    };
}

// https://github.com/bnb-chain/greenfield-cosmos-sdk/blob/b48770f5e210b28536f92734b6228913666d4da1/x/auth/tx/eip712.go#L119-L160
json makeEip712Types(const TypesMap& msgTypes) {
    auto types = json {
        {"EIP712Domain", json::array({
            namedParam("chainId", "uint256"),
            namedParam("name", "string"),
            namedParam("salt", "string"),
            namedParam("verifyingContract", "string"),
            namedParam("version", "string")
        })},
        {"Coin", json::array({
            namedParam("amount", "uint256"),
            namedParam("denom", "string")
        })},
        {"Fee", json::array({
            namedParam("amount", "Coin[]"),
            namedParam("gas_limit", "uint256"),
            namedParam("granter", "string"),
            namedParam("payer", "string")
        })},
    };

    for (const auto& [msgTypeName, msgType] : msgTypes) {
        types[msgTypeName] = msgType;
    }
    return types;
}

// `TypeMsg1Amount` and `Msg1` type names are chosen automatically at the function:
// https://github.com/bnb-chain/greenfield-cosmos-sdk/blob/master/x/auth/tx/eip712.go#L90
// Please note that all parameters repeat the same scheme as `cosmos.bank.v1beta1.MsgSend`.
//
// Use `https://dcellar.io/` with MetaMask to get proper names of types and
json msgSendTypes() {
    // `MsgSend` specific types.
    TypesMap msgTypes = {
        // `TypeMsg1Amount` type represents  `cosmos.bank.v1beta1.MsgSend.amount`.
        {"TypeMsg1Amount", json::array({
            namedParam("amount", "string"),
            namedParam("denom", "string"),
        })},
        {"Msg1", json::array({
            namedParam("amount", "TypeMsg1Amount[]"),
            namedParam("from_address", "string"),
            namedParam("to_address", "string"),
            namedParam("type", "string")
        })},
        {"Tx", json::array({
            namedParam("account_number", "uint256"),
            namedParam("chain_id", "uint256"),
            namedParam("fee", "Fee"),
            namedParam("memo", "string"),
            namedParam("msg1", "Msg1"),
            namedParam("sequence", "uint256"),
            namedParam("timeout_height", "uint256")
        })}
    };

    return makeEip712Types(msgTypes);
}

// `TypeMsg1Amount` and `Msg1` type names are chosen automatically at the function:
// https://github.com/bnb-chain/greenfield-cosmos-sdk/blob/master/x/auth/tx/eip712.go#L90
// Please note that all parameters repeat the same scheme as `greenfield.bridge.MsgTransferOut`.
//
// Use `https://dcellar.io/` with MetaMask to get proper names of types and
json msgTransferOutTypes() {
    // `MsgSend` specific types.
    TypesMap msgTypes = {
        // `TypeMsg1Amount` type represents  `cosmos.bank.v1beta1.MsgSend.amount`.
        {"TypeMsg1Amount", json::array({
            namedParam("amount", "string"),
            namedParam("denom", "string"),
        })},
        {"Msg1", json::array({
            namedParam("amount", "TypeMsg1Amount"),
            namedParam("from", "string"),
            namedParam("to", "string"),
            namedParam("type", "string")
        })},
        {"Tx", json::array({
            namedParam("account_number", "uint256"),
            namedParam("chain_id", "uint256"),
            namedParam("fee", "Fee"),
            namedParam("memo", "string"),
            namedParam("msg1", "Msg1"),
            namedParam("sequence", "uint256"),
            namedParam("timeout_height", "uint256")
        })}
    };

    return makeEip712Types(msgTypes);
}

} // namespace internal::types

json feeToJsonData(const Proto::SigningInput& input, const std::string& feePayer) {
    auto feeAmounts = json::array();
    for (const auto& fAmount : input.fee().amounts()) {
        feeAmounts.push_back(json{
            {"amount", fAmount.amount()},
            {"denom", fAmount.denom()}
        });
    }

    return json{
        {"amount", feeAmounts},
        {"gas_limit", std::to_string(input.fee().gas())},
        {"granter", FEE_GRANTER},
        {"payer", feePayer},
    };
}

// Returns a JSON data of the `EIP712Domain` type.
// https://github.com/bnb-chain/greenfield-cosmos-sdk/blob/b48770f5e210b28536f92734b6228913666d4da1/x/auth/tx/eip712.go#L35-L40
json domainDataJson(const std::string& chainId) {
    return json{
        {"name", "Greenfield Tx"},
        {"version", "1.0.0"},
        {"chainId", chainId},
        {"verifyingContract", "greenfield"},
        {"salt", "0"}
    };
}

// Returns a JSON data of the `EIP712Domain` type using `MsgSend` transaction.
json SignerEip712::wrapMsgSendToTypedData(const Proto::SigningInput& input, const Proto::Message_Send& msgSendProto) {
    auto sendAmounts = json::array();
    for (const auto& sAmount : msgSendProto.amounts()) {
        sendAmounts.push_back(json{
            {"amount", sAmount.amount()},
            {"denom", sAmount.denom()},
        });
    }

    std::string typePrefix = MSG_SEND_TYPE;
    if (!msgSendProto.type_prefix().empty()) {
        typePrefix = msgSendProto.type_prefix();
    }

    return json{
        {"types", internal::types::msgSendTypes()},
        {"primaryType", "Tx"},
        {"domain", domainDataJson(input.eth_chain_id())},
        {"message", json{
            {"account_number", std::to_string(input.account_number())},
            {"chain_id", input.eth_chain_id()},
            {"fee", feeToJsonData(input, msgSendProto.from_address())},
            {"memo", input.memo()},
            {"msg1", json{
                {"amount", sendAmounts},
                {"from_address", msgSendProto.from_address()},
                {"to_address", msgSendProto.to_address()},
                {"type", MSG_SEND_TYPE}
            }},
            {"sequence", std::to_string(input.sequence())},
            {"timeout_height", TIMEOUT_HEIGHT_STR}
        }}
    };
}

// Returns a JSON data of the `EIP712Domain` type using `MsgSend` transaction.
json SignerEip712::wrapMsgTransferOutToTypedData(const Proto::SigningInput& input, const Proto::Message_BridgeTransferOut& msgTransferOut) {
    std::string typePrefix = MSG_TRANSFER_OUT_TYPE;
    if (!msgTransferOut.type_prefix().empty()) {
        typePrefix = msgTransferOut.type_prefix();
    }

    return json{
        {"types", internal::types::msgTransferOutTypes()},
        {"primaryType", "Tx"},
        {"domain", domainDataJson(input.eth_chain_id())},
        {"message", json{
            {"account_number", std::to_string(input.account_number())},
            {"chain_id", input.eth_chain_id()},
            {"fee", feeToJsonData(input, msgTransferOut.from_address())},
            {"memo", input.memo()},
            {"msg1", json{
                {"amount", json{
                    {"amount", msgTransferOut.amount().amount()},
                    {"denom", msgTransferOut.amount().denom()}
                }},
                {"from", msgTransferOut.from_address()},
                {"to", msgTransferOut.to_address()},
                {"type", typePrefix}
            }},
            {"sequence", std::to_string(input.sequence())},
            {"timeout_height", TIMEOUT_HEIGHT_STR}
        }}
    };
}

SigningResult<json> SignerEip712::wrapTxToTypedData(const Proto::SigningInput& input) {
    if (input.messages_size() != 1) {
        return SigningResult<json>::failure(Common::Proto::SigningError::Error_invalid_params);
    }

    switch(input.messages(0).message_oneof_case()) {
        case Proto::Message::kBridgeTransferOut: {
            const auto &msgTransferOut = input.messages(0).bridge_transfer_out();
            return SigningResult<json>::success(wrapMsgTransferOutToTypedData(input, msgTransferOut));
        }
        case Proto::Message::kSendCoinsMessage:
        default: {
            const auto& msgSendProto = input.messages(0).send_coins_message();
            return SigningResult<json>::success(wrapMsgSendToTypedData(input, msgSendProto));
        }
    }
}

SigningResult<Eip712PreImage> SignerEip712::preImageHash(const Proto::SigningInput& input) {
    const auto txTypedDataResult = wrapTxToTypedData(input);
    if (txTypedDataResult.isFailure()) {
        return SigningResult<Eip712PreImage>::failure(txTypedDataResult.error());
    }

    const auto txTypedData = txTypedDataResult.payload();
    const auto txTypedDataHash = Ethereum::ABI::ParamStruct::hashStructJson(txTypedData.dump());
    return SigningResult<Eip712PreImage>::success({.typedData = txTypedData, .typedDataHash = txTypedDataHash});
}

SigningResult<Data> SignerEip712::sign(const Proto::SigningInput& input) {
    const PrivateKey privateKey(data(input.private_key()));
    const auto txTypedDataResult = wrapTxToTypedData(input);
    if (txTypedDataResult.isFailure()) {
        return SigningResult<Data>::failure(txTypedDataResult.error());
    }
    const auto txTypedData = txTypedDataResult.payload().dump();
    const auto chainId = std::stoull(input.eth_chain_id());

    const auto signatureStr = Ethereum::MessageSigner::signTypedData(privateKey, txTypedData, Ethereum::MessageType::Legacy, chainId);
    return SigningResult<Data>::success(parse_hex(signatureStr));
}

void SignerEip712::prepareSignature(Data& signature) {
    Ethereum::MessageSigner::prepareSignature(signature, Ethereum::MessageType::Legacy);
}

} // namespace TW::Greenfield
