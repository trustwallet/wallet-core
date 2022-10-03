// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Hash.h"
#include "MoveTypes.h"
#include "TransactionBuilder.h"
#include "TransactionPayload.h"

namespace TW::Aptos {

template <typename TPayload>
std::pair<std::vector<Data>, nlohmann::json> commonTransferPayload(const TPayload& input) {
    BCS::Serializer aSerializer;
    aSerializer << Address(input.to());
    std::vector<Data> args;
    args.emplace_back(aSerializer.bytes);
    aSerializer.clear();
    aSerializer << input.amount();
    args.emplace_back(aSerializer.bytes);
    nlohmann::json argsJson = nlohmann::json::array({input.to(), std::to_string(input.amount())});
    return std::make_pair(args, argsJson);
}

TransactionPayload transferPayload(const Proto::SigningInput& input) {
    auto&& [args, argsJson] = commonTransferPayload(input.transfer());
    ModuleId module(gAddressOne, "coin");
    TransactionPayload payload = EntryFunction(module, "transfer", {gTransferTag}, args, argsJson);
    return payload;
}

TransactionPayload createAccountPayload(const Proto::SigningInput& input) {
    ModuleId module(gAddressOne, "aptos_account");
    BCS::Serializer aSerializer;
    aSerializer << Address(input.create_account().auth_key());
    std::vector<Data> args;
    args.emplace_back(aSerializer.bytes);
    nlohmann::json argsJson = nlohmann::json::array({input.create_account().auth_key()});
    TransactionPayload payload = EntryFunction(module, "create_account", {}, args, argsJson);
    return payload;
}

TransactionPayload tokenTransferPayload(const Proto::SigningInput& input) {

    auto&& [args, argsJson] = commonTransferPayload(input.token_transfer());
    auto& function = input.token_transfer().function();
    TypeTag tokenTransferTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address(function.account_address()),
                                                                                   function.module(), function.name(), {})})};
    ModuleId module(gAddressOne, "coin");
    TransactionPayload payload = EntryFunction(module, "transfer", {tokenTransferTag}, args, argsJson);
    return payload;
}

Proto::SigningOutput blindSign(const Proto::SigningInput& input) {
    auto output = Proto::SigningOutput();
    BCS::Serializer serializer;
    auto encodedCall = parse_hex(input.any_encoded());
    serializer.add_bytes(begin(encodedCall), end(encodedCall));
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto signature = privateKey.sign(encodedCall, TWCurveED25519);
    auto pubKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    output.set_raw_txn(encodedCall.data(), encodedCall.size());
    output.mutable_authenticator()->set_public_key(pubKeyData.data(), pubKeyData.size());
    output.mutable_authenticator()->set_signature(signature.data(), signature.size());
    serializer << BCS::uleb128{.value = 0} << pubKeyData << signature;
    output.set_encoded(serializer.bytes.data(), serializer.bytes.size());

    // clang-format off
    nlohmann::json json = {
        {"type", "ed25519_signature"},
        {"public_key", hexEncoded(pubKeyData)},
        {"signature", hexEncoded(signature)}
    };
    // clang-format on

    output.set_json(json.dump());
    return output;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto protoOutput = Proto::SigningOutput();
    if (!input.any_encoded().empty()) {
        return blindSign(input);
    }
    auto payloadFunctor = [&input]() {
        switch (input.transaction_payload_case()) {
        case Proto::SigningInput::kTransfer: {
            return transferPayload(input);
        }
        case Proto::SigningInput::kTokenTransfer: {
            return tokenTransferPayload(input);
        }
        case Proto::SigningInput::kCreateAccount:
            return createAccountPayload(input);
        case Proto::SigningInput::TRANSACTION_PAYLOAD_NOT_SET:
            throw std::runtime_error("Transaction payload should be set");
        }
    };
    TransactionBuilder::builder()
        .sender(Address(input.sender()))
        .sequenceNumber(input.sequence_number())
        .payload(payloadFunctor())
        .maxGasAmount(input.max_gas_amount())
        .gasUnitPrice(input.gas_unit_price())
        .expirationTimestampSecs(input.expiration_timestamp_secs())
        .chainId(static_cast<uint8_t>(input.chain_id()))
        .sign(input, protoOutput);
    return protoOutput;
}

} // namespace TW::Aptos
