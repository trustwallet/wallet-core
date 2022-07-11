// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include "Coin.h"
#include <TrustWalletCore/TWCoinType.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Ethereum;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    try {
        uint256_t chainID = load(input.chain_id());
        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto transaction = Signer::build(input);

        auto signature = sign(key, chainID, transaction);

        auto output = Proto::SigningOutput();

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
    switch (input.transaction().transaction_oneof_case()) {
        case Proto::Transaction::kTransfer:
            {
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
                }
            }

        case Proto::Transaction::kErc20Transfer:
            {
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
                }
            }

        case Proto::Transaction::kErc20Approve:
            {
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
                }
            }

        case Proto::Transaction::kErc721Transfer:
            {
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
                }
            }

        case Proto::Transaction::kErc1155Transfer:
            {
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
                }
            }

        case Proto::Transaction::kContractGeneric:
        default:
            {
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
                }
            }
    }
}

Signature Signer::sign(const PrivateKey& privateKey, const uint256_t& chainID, std::shared_ptr<TransactionBase> transaction) noexcept {
    auto preHash = transaction->preHash(chainID);
    return Signer::sign(privateKey, preHash, transaction->usesReplayProtection(), chainID);
}
