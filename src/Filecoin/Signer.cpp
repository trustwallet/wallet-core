// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <google/protobuf/util/json_util.h>

#include "AddressConverter.h"
#include "Ethereum/Transaction.h"
#include "Signer.h"

namespace TW::Filecoin {

// ChainId defines the chain ID used in the Ethereum JSON-RPC endpoint.
// As per https://github.com/ethereum-lists/chains
static constexpr uint256_t FILECOIN_EIP155_CHAIN_ID = 314;

static Proto::SigningOutput errorOutput(const char* error) {
    Proto::SigningOutput output;
    output.set_error_message(error);
    return output;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    try {
        switch (input.derivation()) {
            case Proto::DerivationType::SECP256K1:
                return signSecp256k1(input);
            case Proto::DerivationType::DELEGATED:
                return signDelegated(input);
            default:
                return errorOutput("Unknown derivation type");
        }
    } catch (const std::exception& exp) {
        return errorOutput(exp.what());
    }
}

Data Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    Data toSign = Hash::blake2b(transaction.cid(), 32);
    auto signature = privateKey.sign(toSign, TWCurveSECP256k1);
    return Data(signature.begin(), signature.end());
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}

TW::Data Signer::signaturePreimage(const Proto::SigningInput& input) noexcept {
    auto pubkey = PublicKey(Data(input.public_key().begin(), input.public_key().end()), TWPublicKeyTypeSECP256k1Extended);
    auto tx = Signer::buildTx(pubkey, input);
    return tx.cid();
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept {
    auto tx = Signer::buildTx(publicKey, input);
    const auto json = tx.serialize(Transaction::SignatureType::SECP256K1, signature);
    
    // Return Protobuf output.
    Proto::SigningOutput output;
    output.set_json(json.data(), json.size());
    return output;
}

Proto::SigningOutput Signer::signSecp256k1(const Proto::SigningInput& input) {
    // Load private key and transaction from Protobuf input.
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    auto transaction = Signer::buildTx(pubkey, input);

    // Sign transaction.
    auto signature = sign(key, transaction);
    const auto json = transaction.serialize(Transaction::SignatureType::SECP256K1, signature);

    // Return Protobuf output.
    Proto::SigningOutput output;
    output.set_json(json.data(), json.size());
    return output;
}

Transaction Signer::buildTx(const PublicKey& publicKey, const Proto::SigningInput& input) noexcept {
    Address from_address = Address::secp256k1Address(publicKey);
    Address to_address(input.to());

    // Load the transaction params.
    Data params(input.params().begin(), input.params().end());

    // Simple transfer by default.
    Transaction::MethodType method = Transaction::MethodType::SEND;
    if (to_address.type == Address::Type::DELEGATED) {
        method = Transaction::MethodType::INVOKE_EVM;
    }

    return {
        Transaction(
            /* to */         to_address,
            /* from */       from_address,
            /* nonce */      input.nonce(),
            /* value */      load(input.value()),
            /* gasLimit */   input.gas_limit(),
            /* gasFeeCap */  load(input.gas_fee_cap()),
            /* gasPremium */ load(input.gas_premium()),
            /* method */     method,
            /* params */     params
        )
    };
}

/// https://github.com/filecoin-project/lotus/blob/ce17546a762eef311069e13410d15465d832a45e/chain/messagesigner/messagesigner.go#L197-L211
Proto::SigningOutput Signer::signDelegated(const Proto::SigningInput& input) {
    // Load private key from Protobuf input.
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    // Load the transaction params.
    Data params(input.params().begin(), input.params().end());

    // Generate a Delegated `f4` address.
    Address fromAddress = Address::delegatedAddress(pubkey);

    // Parse `to` address. Expects either ID `f1` or Delegated `f4` address.
    Address toAddress(input.to());
    auto toEth = AddressConverter::convertToEthereum(toAddress);
    if (!toEth) {
        throw std::invalid_argument("Expected a Delegated recipient");
    }
    Data toBytes(toEth->bytes.begin(), toEth->bytes.end());

    // Sign transaction as an Ethereum EIP1559 native transfer.
    auto ethTransaction = Ethereum::TransactionEip1559::buildNativeTransfer(
        /* nonce */                 input.nonce(),
        /* maxInclusionFeePerGas */ load(input.gas_premium()),
        /* maxFeePerGas */          load(input.gas_fee_cap()),
        /* gasLimit */              input.gas_limit(),
        /* toAddress */             toBytes,
        /* amount */                load(input.value()),
        /* data */                  params
    );
    Data preHash = ethTransaction->preHash(FILECOIN_EIP155_CHAIN_ID);
    Data signature = privateKey.sign(preHash, TWCurveSECP256k1);

    // Generate a Filecoin signed message.
    Transaction filecoinTransaction(
        /* to */         toAddress,
        /* from */       fromAddress,
        /* nonce */      input.nonce(),
        /* value */      load(input.value()),
        /* gasLimit */   input.gas_limit(),
        /* gasFeeCap */  load(input.gas_fee_cap()),
        /* gasPremium */ load(input.gas_premium()),
        /* method */     Transaction::MethodType::INVOKE_EVM,
        /* params */     params
    );
    const auto json = filecoinTransaction.serialize(Transaction::SignatureType::DELEGATED, signature);

    // Return Protobuf output.
    Proto::SigningOutput output;
    output.set_json(json.data(), json.size());
    return output;
}

} // namespace TW::Filecoin
