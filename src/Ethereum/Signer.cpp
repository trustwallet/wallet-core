// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include "Coin.h"
#include <TrustWalletCore/TWCoinType.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Ethereum {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    try {
        uint256_t chainID = load(input.chain_id());
        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto transaction = Signer::build(input);

        auto signature = sign(key, chainID, transaction);

        auto output = Proto::SigningOutput();

        auto pre_hash = transaction->preHash(chainID);
        output.set_pre_hash(pre_hash.data(), pre_hash.size());

        auto encoded = transaction->encoded(signature, chainID);
        output.set_encoded(encoded.data(), encoded.size());

        auto v = store(signature.v, 1);
        output.set_v(v.data(), v.size());
        auto r = store(signature.r, 32);
        output.set_r(r.data(), r.size());
        auto s = store(signature.s, 32);
        output.set_s(s.data(), s.size());

        output.set_data(transaction->payload.data(), transaction->payload.size());

        return output;
    } catch (std::exception&) {
        return Proto::SigningOutput();
    }
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return hex(output.encoded());
}

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input, const Data& signature) noexcept {
    try {
        uint256_t chainID = load(input.chain_id());
        auto transaction = Signer::build(input);

        // prepare Signature
        const Signature sigStruct = signatureDataToStruct(signature, transaction->usesReplayProtection(), chainID);

        auto output = Proto::SigningOutput();

        auto pre_hash = transaction->preHash(chainID);
        output.set_pre_hash(pre_hash.data(), pre_hash.size());

        auto encoded = transaction->encoded(sigStruct, chainID);
        output.set_encoded(encoded.data(), encoded.size());

        auto v = store(sigStruct.v, 1);
        output.set_v(v.data(), v.size());
        auto r = store(sigStruct.r, 32);
        output.set_r(r.data(), r.size());
        auto s = store(sigStruct.s, 32);
        output.set_s(s.data(), s.size());

        output.set_data(transaction->payload.data(), transaction->payload.size());
        return output;
    } catch (std::exception&) {
        return Proto::SigningOutput();
    }
}

Signature Signer::signatureDataToStruct(const Data& signature, bool includeEip155, const uint256_t& chainID) noexcept {
    if (!includeEip155) {
        return signatureDataToStructSimple(signature);
    }
    return signatureDataToStructWithEip155(chainID, signature);
}

Signature Signer::signatureDataToStructSimple(const Data& signature) noexcept {
    boost::multiprecision::uint256_t r, s, v;
    import_bits(r, signature.begin(), signature.begin() + 32);
    import_bits(s, signature.begin() + 32, signature.begin() + 64);
    import_bits(v, signature.begin() + 64, signature.begin() + 65);
    return Signature{r, s, v};
}

Data Signer::simpleStructToSignatureData(const Signature& signature) noexcept {
    Data fullSignature;

    auto r = store(signature.r, 32);
    append(fullSignature, r);
    auto s = store(signature.s, 32);
    append(fullSignature, s);
    auto v = store(signature.v, 1);
    append(fullSignature, v);

    return fullSignature;
}

Signature Signer::signatureDataToStructWithEip155(const uint256_t& chainID, const Data& signature) noexcept {
    Signature rsv = signatureDataToStructSimple(signature);
    // Embed chainID in V param, for replay protection, legacy (EIP155)
    if (chainID != 0) {
        rsv.v += 35 + chainID + chainID;
    } else {
        rsv.v += 27;
    }
    return rsv;
}

Signature Signer::sign(const PrivateKey& privateKey, const Data& hash, bool includeEip155, const uint256_t& chainID) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return signatureDataToStruct(signature, includeEip155, chainID);
}

// May throw
Data addressStringToData(const std::string& asString) {
    if (asString.empty()) {
        return {};
    }
    // only ronin address prefix is not 0x
    if (asString.compare(0, 2, "0x") != 0) {
        return TW::addressToData(TWCoinTypeRonin, asString);
    }
    return TW::addressToData(TWCoinTypeEthereum, asString);
}

std::shared_ptr<TransactionBase> Signer::build(const Proto::SigningInput& input) {
    Data toAddress = addressStringToData(input.to_address());
    uint256_t nonce = load(input.nonce());
    uint256_t gasPrice = load(input.gas_price());
    uint256_t gasLimit = load(input.gas_limit());
    uint256_t maxInclusionFeePerGas = load(input.max_inclusion_fee_per_gas());
    uint256_t maxFeePerGas = load(input.max_fee_per_gas());

    // EIP4337
    Data entryPointAddress = addressStringToData(input.user_operation().entry_point());
    Data senderAddress = addressStringToData(input.user_operation().sender());
    Data initCode = Data(input.user_operation().init_code().begin(), input.user_operation().init_code().end());
    uint256_t preVerificationGas = load(input.user_operation().pre_verification_gas());
    uint256_t verificationGasLimit = load(input.user_operation().verification_gas_limit());
    Data paymasterAndData = Data(input.user_operation().paymaster_and_data().begin(), input.user_operation().paymaster_and_data().end());

    switch (input.transaction().transaction_oneof_case()) {
    case Proto::Transaction::kTransfer: {
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildNativeTransfer(
                nonce, gasPrice, gasLimit,
                /* to: */ toAddress,
                /* amount: */ load(input.transaction().transfer().amount()),
                /* optional data: */ Data(input.transaction().transfer().data().begin(), input.transaction().transfer().data().end()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildNativeTransfer(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* to: */ toAddress,
                /* amount: */ load(input.transaction().transfer().amount()),
                /* optional data: */ Data(input.transaction().transfer().data().begin(), input.transaction().transfer().data().end()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildNativeTransfer(
                entryPointAddress,
                senderAddress,
                toAddress,
                load(input.transaction().transfer().amount()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode,
                Data(input.transaction().transfer().data().begin(), input.transaction().transfer().data().end())
                );
        }
    }

    case Proto::Transaction::kErc20Transfer: {
        Data tokenToAddress = addressStringToData(input.transaction().erc20_transfer().to());
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildERC20Transfer(
                nonce, gasPrice, gasLimit,
                /* tokenContract: */ toAddress,
                /* toAddress */ tokenToAddress,
                /* amount: */ load(input.transaction().erc20_transfer().amount()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildERC20Transfer(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* tokenContract: */ toAddress,
                /* toAddress */ tokenToAddress,
                /* amount: */ load(input.transaction().erc20_transfer().amount()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildERC20Transfer(
                entryPointAddress,
                senderAddress,
                toAddress,
                tokenToAddress,
                load(input.transaction().erc20_transfer().amount()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode);
        }
    }

    case Proto::Transaction::kErc20Approve: {
        Data spenderAddress = addressStringToData(input.transaction().erc20_approve().spender());
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildERC20Approve(
                nonce, gasPrice, gasLimit,
                /* tokenContract: */ toAddress,
                /* toAddress */ spenderAddress,
                /* amount: */ load(input.transaction().erc20_approve().amount()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildERC20Approve(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* tokenContract: */ toAddress,
                /* toAddress */ spenderAddress,
                /* amount: */ load(input.transaction().erc20_approve().amount()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildERC20Approve(
                entryPointAddress,
                senderAddress,
                toAddress,
                spenderAddress,
                load(input.transaction().erc20_approve().amount()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode);
        }
    }

    case Proto::Transaction::kErc721Transfer: {
        Data tokenToAddress = addressStringToData(input.transaction().erc721_transfer().to());
        Data tokenFromAddress = addressStringToData(input.transaction().erc721_transfer().from());
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildERC721Transfer(
                nonce, gasPrice, gasLimit,
                /* tokenContract: */ toAddress,
                /* fromAddress: */ tokenFromAddress,
                /* toAddress */ tokenToAddress,
                /* tokenId: */ load(input.transaction().erc721_transfer().token_id()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildERC721Transfer(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* tokenContract: */ toAddress,
                /* fromAddress: */ tokenFromAddress,
                /* toAddress */ tokenToAddress,
                /* tokenId: */ load(input.transaction().erc721_transfer().token_id()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildERC721Transfer(
                entryPointAddress,
                senderAddress,
                toAddress,
                tokenFromAddress,
                tokenToAddress,
                load(input.transaction().erc721_transfer().token_id()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode);
        }
    }

    case Proto::Transaction::kErc1155Transfer: {
        Data tokenToAddress = addressStringToData(input.transaction().erc1155_transfer().to());
        Data tokenFromAddress = addressStringToData(input.transaction().erc1155_transfer().from());
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildERC1155Transfer(
                nonce, gasPrice, gasLimit,
                /* tokenContract: */ toAddress,
                /* fromAddress: */ tokenFromAddress,
                /* toAddress */ tokenToAddress,
                /* tokenId: */ load(input.transaction().erc1155_transfer().token_id()),
                /* value */ load(input.transaction().erc1155_transfer().value()),
                /* data */ Data(input.transaction().erc1155_transfer().data().begin(), input.transaction().erc1155_transfer().data().end()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildERC1155Transfer(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* tokenContract: */ toAddress,
                /* fromAddress: */ tokenFromAddress,
                /* toAddress */ tokenToAddress,
                /* tokenId: */ load(input.transaction().erc1155_transfer().token_id()),
                /* value */ load(input.transaction().erc1155_transfer().value()),
                /* data */ Data(input.transaction().erc1155_transfer().data().begin(), input.transaction().erc1155_transfer().data().end()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildERC1155Transfer(
                entryPointAddress,
                senderAddress,
                toAddress,
                tokenFromAddress,
                tokenToAddress,
                load(input.transaction().erc1155_transfer().token_id()),
                load(input.transaction().erc1155_transfer().value()),
                Data(input.transaction().erc1155_transfer().data().begin(), input.transaction().erc1155_transfer().data().end()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode);
        }
    }

    case Proto::Transaction::kBatch: {
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return nullptr;
        case Proto::TransactionMode::Enveloped: // Eip1559
            return nullptr;
        case Proto::TransactionMode::UserOp:
            std::vector<Data> addresses;
            std::vector<uint256_t> amounts;
            std::vector<Data> payloads;
            for (int i=0; i < input.transaction().batch().calls().size(); i++) {
                addresses.push_back(addressStringToData(input.transaction().batch().calls()[i].address()));
                amounts.push_back(load(input.transaction().batch().calls()[i].amount()));
                payloads.push_back(Data(input.transaction().batch().calls()[i].payload().begin(), input.transaction().batch().calls()[i].payload().end()));
            }

            return UserOperation::buildBatch(
                entryPointAddress,
                senderAddress,
                addresses,
                amounts,
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode,
                payloads
            );
        }
    }

    case Proto::Transaction::kContractGeneric:
    default: {
        switch (input.tx_mode()) {
        case Proto::TransactionMode::Legacy:
        default:
            return TransactionNonTyped::buildNativeTransfer(
                nonce, gasPrice, gasLimit,
                /* to: */ toAddress,
                /* amount: */ load(input.transaction().contract_generic().amount()),
                /* transaction: */ Data(input.transaction().contract_generic().data().begin(), input.transaction().contract_generic().data().end()));

        case Proto::TransactionMode::Enveloped: // Eip1559
            return TransactionEip1559::buildNativeTransfer(
                nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit,
                /* to: */ toAddress,
                /* amount: */ load(input.transaction().contract_generic().amount()),
                /* transaction: */ Data(input.transaction().contract_generic().data().begin(), input.transaction().contract_generic().data().end()));

        case Proto::TransactionMode::UserOp:
            return UserOperation::buildNativeTransfer(
                entryPointAddress,
                senderAddress,
                toAddress,
                load(input.transaction().contract_generic().amount()),
                nonce,
                gasLimit,
                verificationGasLimit,
                maxFeePerGas,
                maxInclusionFeePerGas,
                preVerificationGas,
                paymasterAndData,
                initCode,
                Data(input.transaction().contract_generic().data().begin(), input.transaction().contract_generic().data().end()));
        }
    }
    }
}

Signature Signer::sign(const PrivateKey& privateKey, const uint256_t& chainID, std::shared_ptr<TransactionBase> transaction) noexcept {
    auto preHash = transaction->preHash(chainID);
    return Signer::sign(privateKey, preHash, transaction->usesReplayProtection(), chainID);
}

} // namespace TW::Ethereum
