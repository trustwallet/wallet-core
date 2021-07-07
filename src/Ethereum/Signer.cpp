// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Ethereum;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    try {
        uint256_t chainID = load(input.chain_id());
        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto transaction = Signer::build(input);

        auto preHash = transaction->preHash(chainID);
        auto signature = sign(key, chainID, preHash);

        auto output = Proto::SigningOutput();

        auto encoded = transaction->encoded(signature, chainID);
        output.set_encoded(encoded.data(), encoded.size());

        auto v = store(signature.v);
        output.set_v(v.data(), v.size());

        auto r = store(signature.r);
        output.set_r(r.data(), r.size());

        auto s = store(signature.s);
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

Signature Signer::valuesRSV(const uint256_t& chainID, const Data& signature) noexcept {
    boost::multiprecision::uint256_t r, s, v;
    import_bits(r, signature.begin(), signature.begin() + 32);
    import_bits(s, signature.begin() + 32, signature.begin() + 64);
    import_bits(v, signature.begin() + 64, signature.begin() + 65);
    v += 27;

    boost::multiprecision::uint256_t newV;
    if (chainID != 0) {
        import_bits(newV, signature.begin() + 64, signature.begin() + 65);
        newV += 35 + chainID + chainID;
    } else {
        newV = v;
    }
    return Signature{r, s, newV};
}

Signature Signer::sign(const PrivateKey& privateKey, const uint256_t& chainID, const Data& hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return valuesRSV(chainID, signature);
}

// May throw
Data addressStringToData(const std::string& asString) {
    if (asString.empty()) {
        return {};
    }
    auto address = Address(asString);
    Data asData;
    asData.resize(20);
    std::copy(address.bytes.begin(), address.bytes.end(), asData.data());
    return asData;
}

std::shared_ptr<TransactionNonTyped> Signer::buildNonTyped(const Proto::SigningInput& input) {
    Data toAddress = addressStringToData(input.to_address());
    uint256_t nonce = load(input.nonce());
    uint256_t gasPrice = load(input.gas_price());
    uint256_t gasLimit = load(input.gas_limit());
    assert(gasPrice != 0);
    switch (input.transaction().transaction_oneof_case()) {
        case Proto::Transaction::kTransfer:
            {
                auto transaction = TransactionNonTyped::buildNativeTransfer(
                    nonce, gasPrice, gasLimit,
                    /* to: */ toAddress,
                    /* amount: */ load(input.transaction().transfer().amount()),
                    /* optional data: */ Data(input.transaction().transfer().data().begin(), input.transaction().transfer().data().end()));
                return transaction;
            }

        case Proto::Transaction::kErc20Transfer:
            {
                Data tokenToAddress = addressStringToData(input.transaction().erc20_transfer().to());
                auto transaction = TransactionNonTyped::buildERC20Transfer(
                    nonce, gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* toAddress */ tokenToAddress,
                    /* amount: */ load(input.transaction().erc20_transfer().amount()));
                return transaction;
            }

        case Proto::Transaction::kErc20Approve:
            {
                Data spenderAddress = addressStringToData(input.transaction().erc20_approve().spender());
                auto transaction = TransactionNonTyped::buildERC20Approve(
                    nonce, gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* toAddress */ spenderAddress,
                    /* amount: */ load(input.transaction().erc20_approve().amount()));
                return transaction;
            }

        case Proto::Transaction::kErc721Transfer:
            {
                Data tokenToAddress = addressStringToData(input.transaction().erc721_transfer().to());
                Data tokenFromAddress = addressStringToData(input.transaction().erc721_transfer().from());
                auto transaction = TransactionNonTyped::buildERC721Transfer(
                    nonce, gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* fromAddress: */ tokenFromAddress,
                    /* toAddress */ tokenToAddress,
                    /* tokenId: */ load(input.transaction().erc721_transfer().token_id()));
                return transaction;
            }

        case Proto::Transaction::kErc1155Transfer:
            {
                Data tokenToAddress = addressStringToData(input.transaction().erc1155_transfer().to());
                Data tokenFromAddress = addressStringToData(input.transaction().erc1155_transfer().from());
                auto transaction = TransactionNonTyped::buildERC1155Transfer(
                    nonce, gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* fromAddress: */ tokenFromAddress,
                    /* toAddress */ tokenToAddress,
                    /* tokenId: */ load(input.transaction().erc1155_transfer().token_id()),
                    /* value */ load(input.transaction().erc1155_transfer().value()),
                    /* data */ Data(input.transaction().erc1155_transfer().data().begin(), input.transaction().erc1155_transfer().data().end())
                );
                return transaction;
            }

        case Proto::Transaction::kContractGeneric:
        default:
            {
                auto transaction = TransactionNonTyped::buildNativeTransfer(
                    nonce, gasPrice, gasLimit,
                    /* to: */ toAddress,
                    /* amount: */ load(input.transaction().contract_generic().amount()),
                    /* transaction: */ Data(input.transaction().contract_generic().data().begin(), input.transaction().contract_generic().data().end()));
                return transaction;
            }
    }
}

Signature Signer::sign(const PrivateKey& privateKey, const uint256_t& chainID, std::shared_ptr<TransactionBase> transaction) noexcept {
    auto preHash = transaction->preHash(chainID);
    return Signer::sign(privateKey, chainID, preHash);
}
