// Copyright © 2017-2023 Trust Wallet.
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

namespace {
template <typename T>
void serializeToArgs(std::vector<TW::Data>& args, T&& toSerialize) {
    TW::BCS::Serializer serializer;
    serializer << std::forward<T>(toSerialize);
    args.emplace_back(serializer.bytes);
}
} // namespace

namespace TW::Aptos {

template <typename TPayload>
std::pair<std::vector<Data>, nlohmann::json> commonTransferPayload(const TPayload& input) {
    std::vector<Data> args;
    serializeToArgs(args, Address(input.to()));
    serializeToArgs(args, input.amount());
    nlohmann::json argsJson = nlohmann::json::array({input.to(), std::to_string(input.amount())});
    return std::make_pair(args, argsJson);
}

TransactionPayload transferPayload(const Proto::SigningInput& input) {
    auto&& [args, argsJson] = commonTransferPayload(input.transfer());
    TransactionPayload payload = EntryFunction(gAptosAccountModule, "transfer", {}, args, argsJson);
    return payload;
}

TransactionPayload createAccountPayload(const Proto::SigningInput& input) {
    std::vector<Data> args;
    serializeToArgs(args, Address(input.create_account().auth_key()));
    nlohmann::json argsJson = nlohmann::json::array({input.create_account().auth_key()});
    TransactionPayload payload = EntryFunction(gAptosAccountModule, "create_account", {}, args, argsJson);
    return payload;
}

TransactionPayload claimNftPayload(const Proto::ClaimNftMessage& msg) {
    std::vector<Data> args;
    serializeToArgs(args, Address(msg.sender()));
    serializeToArgs(args, Address(msg.creator()));
    serializeToArgs(args, msg.collectionname());
    serializeToArgs(args, msg.name());
    serializeToArgs(args, msg.property_version());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            msg.sender(),
                            msg.creator(),
                            msg.collectionname(),
                            msg.name(),
                            std::to_string(msg.property_version()),
                        });
    // clang-format on
    TransactionPayload payload = EntryFunction(gAptosTokenTransfersModule, "claim_script", {}, args, argsJson);
    return payload;
}

TransactionPayload nftOfferPayload(const Proto::OfferNftMessage& msg) {
    std::vector<Data> args;
    serializeToArgs(args, Address(msg.receiver()));
    serializeToArgs(args, Address(msg.creator()));
    serializeToArgs(args, msg.collectionname());
    serializeToArgs(args, msg.name());
    serializeToArgs(args, msg.property_version());
    serializeToArgs(args, msg.amount());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            msg.receiver(),
                            msg.creator(),
                            msg.collectionname(),
                            msg.name(),
                            std::to_string(msg.property_version()),
                            std::to_string(msg.amount())
                        });
    // clang-format on
    TransactionPayload payload = EntryFunction(gAptosTokenTransfersModule, "offer_script", {}, args, argsJson);
    return payload;
}

TransactionPayload tortugaClaimPayload(const std::string& smart_contract_address, const Proto::TortugaClaim& msg) {
    std::vector<Data> args;
    serializeToArgs(args, msg.idx());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            std::to_string(msg.idx())
                        });
    // clang-format on
    ModuleId tortugaStakeModule{Address(smart_contract_address), "stake_router"};
    TransactionPayload payload = EntryFunction(tortugaStakeModule, "claim", {}, args, argsJson);
    return payload;
}

TransactionPayload tortugaStakePayload(const std::string& smart_contract_address, const Proto::TortugaStake& msg) {
    std::vector<Data> args;
    serializeToArgs(args, msg.amount());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            std::to_string(msg.amount())
                        });
    // clang-format on
    ModuleId tortugaStakeModule{Address(smart_contract_address), "stake_router"};
    TransactionPayload payload = EntryFunction(tortugaStakeModule, "stake", {}, args, argsJson);
    return payload;
}

TransactionPayload tortugaUnStakePayload(const std::string& smart_contract_address, const Proto::TortugaUnstake& msg) {
    std::vector<Data> args;
    serializeToArgs(args, msg.amount());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            std::to_string(msg.amount())
                        });
    // clang-format on
    ModuleId tortugaStakeModule{Address(smart_contract_address), "stake_router"};
    TransactionPayload payload = EntryFunction(tortugaStakeModule, "unstake", {}, args, argsJson);
    return payload;
}

TransactionPayload cancelNftOfferPayload(const Proto::CancelOfferNftMessage& msg) {
    std::vector<Data> args;
    serializeToArgs(args, Address(msg.receiver()));
    serializeToArgs(args, Address(msg.creator()));
    serializeToArgs(args, msg.collectionname());
    serializeToArgs(args, msg.name());
    serializeToArgs(args, msg.property_version());
    // clang-format off
    nlohmann::json argsJson = nlohmann::json::array(
                        {
                            msg.receiver(),
                            msg.creator(),
                            msg.collectionname(),
                            msg.name(),
                            std::to_string(msg.property_version()),
                        });
    // clang-format on
    TransactionPayload payload = EntryFunction(gAptosTokenTransfersModule, "cancel_offer_script", {}, args, argsJson);
    return payload;
}

TransactionPayload tokenTransferPayload(const Proto::SigningInput& input) {

    auto&& [args, argsJson] = commonTransferPayload(input.token_transfer());
    auto& function = input.token_transfer().function();
    TypeTag tokenTransferTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address(function.account_address()),
                                                                                   function.module(), function.name(), {})})};
    TransactionPayload payload = EntryFunction(gAptosCoinModule, "transfer", {tokenTransferTag}, args, argsJson);
    return payload;
}

TransactionPayload tokenTransferCoinsPayload(const Proto::SigningInput& input) {
    auto&& [args, argsJson] = commonTransferPayload(input.token_transfer_coins());
    auto& function = input.token_transfer_coins().function();
    TypeTag tokenTransferTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address(function.account_address()),
                                                                                   function.module(), function.name(), {})})};
    TransactionPayload payload = EntryFunction(gAptosAccountModule, "transfer_coins", {tokenTransferTag}, args, argsJson);
    return payload;
}

TransactionPayload registerTokenPayload(const Proto::SigningInput& input) {

    auto& function = input.register_token().function();
    TypeTag tokenRegisterTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address(function.account_address()),
                                                                                   function.module(), function.name(), {})})};
    TransactionPayload payload = EntryFunction(gAptosManagedCoinsModule, "register", {tokenRegisterTag}, {});
    return payload;
}

Proto::SigningOutput blindSign(const Proto::SigningInput& input) {
    auto output = Proto::SigningOutput();
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    if (nlohmann::json j = nlohmann::json::parse(input.any_encoded(), nullptr, false); j.is_discarded()) {
        BCS::Serializer serializer;
        auto encodedCall = parse_hex(input.any_encoded());
        serializer.add_bytes(begin(encodedCall), end(encodedCall));
        auto signature = privateKey.sign(encodedCall, TWCurveED25519);
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
    } else {
        TransactionBuilder::builder()
            .sender(Address(input.sender()))
            .sequenceNumber(input.sequence_number())
            .payload(EntryFunction::from_json(j))
            .maxGasAmount(input.max_gas_amount())
            .gasUnitPrice(input.gas_unit_price())
            .expirationTimestampSecs(input.expiration_timestamp_secs())
            .chainId(static_cast<uint8_t>(input.chain_id()))
            .sign(input, output);
    }
    return output;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto protoOutput = Proto::SigningOutput();
    if (!input.any_encoded().empty()) {
        return blindSign(input);
    }
    auto nftPayloadFunctor = [](const Proto::NftMessage& nftMessage) {
        switch (nftMessage.nft_transaction_payload_case()) {
        case Proto::NftMessage::kOfferNft:
            return nftOfferPayload(nftMessage.offer_nft());
        case Proto::NftMessage::kCancelOfferNft:
            return cancelNftOfferPayload(nftMessage.cancel_offer_nft());
        case Proto::NftMessage::kClaimNft:
            return claimNftPayload(nftMessage.claim_nft());
        case Proto::NftMessage::NFT_TRANSACTION_PAYLOAD_NOT_SET:
            throw std::runtime_error("Nft message payload not set");
        }
    };
    auto liquidStakingFunctor = [](const Proto::LiquidStaking& liquidStakingMessage) {
        switch (liquidStakingMessage.liquid_stake_transaction_payload_case()) {
        case Proto::LiquidStaking::kStake:
            return tortugaStakePayload(liquidStakingMessage.smart_contract_address(), liquidStakingMessage.stake());
        case Proto::LiquidStaking::kUnstake:
            return tortugaUnStakePayload(liquidStakingMessage.smart_contract_address(), liquidStakingMessage.unstake());
        case Proto::LiquidStaking::kClaim:
            return tortugaClaimPayload(liquidStakingMessage.smart_contract_address(), liquidStakingMessage.claim());
        case Proto::LiquidStaking::LIQUID_STAKE_TRANSACTION_PAYLOAD_NOT_SET:
            return TransactionPayload();
        }
    };
    auto payloadFunctor = [&input, &nftPayloadFunctor, &liquidStakingFunctor]() {
        switch (input.transaction_payload_case()) {
        case Proto::SigningInput::kTransfer: {
            return transferPayload(input);
        }
        case Proto::SigningInput::kTokenTransfer: {
            return tokenTransferPayload(input);
        }
        case Proto::SigningInput::kNftMessage: {
            return nftPayloadFunctor(input.nft_message());
        }
        case Proto::SigningInput::kCreateAccount: {
            return createAccountPayload(input);
        }
        case Proto::SigningInput::kRegisterToken: {
            return registerTokenPayload(input);
        }
        case Proto::SigningInput::kLiquidStakingMessage: {
            return liquidStakingFunctor(input.liquid_staking_message());
        }
        case Proto::SigningInput::kTokenTransferCoins: {
            return tokenTransferCoinsPayload(input);
        }
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
