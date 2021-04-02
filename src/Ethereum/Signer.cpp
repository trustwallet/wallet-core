// Copyright © 2017-2020 Trust Wallet.
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
        auto signer = Signer(load(input.chain_id()));
        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        // TODO legacy is used here, but should be generic
        //const TransactionBase& transaction = Signer::buildLegacy(input);
        const TransactionBase& transaction = Signer::buildAccessList(input);

        auto signature = signer.sign(key, transaction);

        auto output = Proto::SigningOutput();

        //auto encoded = RLP::encodeLegacy(transactionLegacy);
        auto encoded = transaction.encoded(signature, signer.chainID);
        output.set_encoded(encoded.data(), encoded.size());

        auto v = store(signature.v);
        output.set_v(v.data(), v.size());

        auto r = store(signature.r);
        output.set_r(r.data(), r.size());

        auto s = store(signature.s);
        output.set_s(s.data(), s.size());

        //output.set_data(transactionLegacy.payload.data(), transactionLegacy.payload.size());

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

SignatureRSV Signer::values(const uint256_t& chainID, const Data& signature) noexcept {
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
    return SignatureRSV{r, s, newV};
}

SignatureRSV Signer::sign(const uint256_t& chainID, const PrivateKey& privateKey, const Data& hash) noexcept {
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return values(chainID, signature);
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

/*
TransactionXX Signer::build(const Proto::SigningInput& input) {
    uint256_t gasPrice = load(input.gas_price());
    if (gasPrice != 0) {
        // legacy
        return TransactionXX::createLegacy(RLP::encodeLegacy(buildLegacy(input)));
    }
    // EIP1559, enveloped
    // AccessList for now
    uint256_t nonce = load(input.nonce());
    uint256_t gasLimit = load(input.gas_limit());
    Data toAddress = addressStringToData(input.to_address());
    // TODO field
    auto accessList = TransactionEnveloped::buildERC2930AccessListPayload(nonce, gasPrice, gasLimit, toAddress,
        0, Data(), Data(), Data(), Data(), Data());
    return TransactionXX::createEnveloped(TransactionType::OptionalAccessList, accessList);
}
*/

TransactionLegacy Signer::buildLegacy(const Proto::SigningInput& input) {
    Data toAddress = addressStringToData(input.to_address());
    uint256_t nonce = load(input.nonce());
    uint256_t gasPrice = load(input.gas_price());
    uint256_t gasLimit = load(input.gas_limit());
    assert(gasPrice != 0);
    switch (input.transaction().transaction_oneof_case()) {
        case Proto::Transaction::kTransfer:
            {
                auto transaction = TransactionLegacy::buildNativeTransfer(
                    nonce,
                    gasPrice, gasLimit,
                    /* to: */ toAddress,
                    /* amount: */ load(input.transaction().transfer().amount()),
                    /* optional data: */ Data(input.transaction().transfer().data().begin(), input.transaction().transfer().data().end()));
                return transaction;
            }

        case Proto::Transaction::kErc20Transfer:
            {
                Data tokenToAddress = addressStringToData(input.transaction().erc20_transfer().to());
                auto transaction = TransactionLegacy::buildERC20Transfer(
                    nonce,
                    gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* toAddress */ tokenToAddress,
                    /* amount: */ load(input.transaction().erc20_transfer().amount()));
                return transaction;
            }

        case Proto::Transaction::kErc20Approve:
            {
                Data spenderAddress = addressStringToData(input.transaction().erc20_approve().spender());
                auto transaction = TransactionLegacy::buildERC20Approve(
                    nonce,
                    gasPrice, gasLimit,
                    /* tokenContract: */ toAddress,
                    /* toAddress */ spenderAddress,
                    /* amount: */ load(input.transaction().erc20_approve().amount()));
                return transaction;
            }

        case Proto::Transaction::kErc721Transfer:
            {
                Data tokenToAddress = addressStringToData(input.transaction().erc721_transfer().to());
                Data tokenFromAddress = addressStringToData(input.transaction().erc721_transfer().from());
                auto transaction = TransactionLegacy::buildERC721Transfer(
                    nonce,
                    gasPrice, gasLimit,
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
                auto transaction = TransactionLegacy::buildERC1155Transfer(
                    nonce,
                    gasPrice, gasLimit,
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
                auto transaction = TransactionLegacy::buildNativeTransfer(
                    nonce,
                    gasPrice, gasLimit,
                    /* to: */ toAddress,
                    /* amount: */ load(input.transaction().contract_generic().amount()),
                    /* transaction: */ Data(input.transaction().contract_generic().data().begin(), input.transaction().contract_generic().data().end()));
                return transaction;
            }
    }
}

TransactionAccessList Signer::buildAccessList(const Proto::SigningInput& input) {
    Data toAddress = addressStringToData(input.to_address());
    uint256_t nonce = load(input.nonce());
    uint256_t gasPrice = load(input.gas_price());
    uint256_t gasLimit = load(input.gas_limit());
    assert(gasPrice != 0);
    switch (input.transaction().transaction_oneof_case()) {
        case Proto::Transaction::kTransfer:
            {
                auto accessList = std::vector<ALItem>{
                    {
                        parse_hex("0xde0b295669a9fd93d5f28d9ec85e40f4cb697bae"),
                        //parse_hex("0xde0b295669a9fd93d5f28d9ec85e40f4cb697bae""000000000000000000000000"),
                        {
                            parse_hex("0x0000000000000000000000000000000000000000000000000000000000000003"),
                            //parse_hex("0x0000000000000000000000000000000000000000000000000000000000000007"),
                        }
                    },
                    /*
                    {
                        parse_hex("0xbb9bc244d798123fde783fcc1c72d3bb8c189413""000000000000000000000000"),
                        {
                        }
                    },
                    */
                };
                auto transaction = TransactionAccessList(

                    nonce,
                    gasPrice, gasLimit,
                    /* to: */ toAddress,
                    /* amount: */ load(input.transaction().transfer().amount()),
                    /* optional data: */ Data(),
                    /* accessList */ accessList
                );
                return transaction;
            }
    }
    assert(false);
}

SignatureRSV Signer::sign(const PrivateKey& privateKey, const TransactionBase& transaction) const noexcept {
    //auto hash = this->hash(transaction, chainID);
    auto hash = transaction.hash(chainID);
    auto signature = Signer::sign(chainID, privateKey, hash);

    /*
    transaction.r = signature.r;
    transaction.s = signature.s;
    transaction.v = signature.v;
    */

    return signature;
}
