// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionFactory.h"

#include "Codec.h"
#include "HexCoding.h"
#include "uint256.h"

using namespace TW;
using namespace TW::Elrond;

const int TX_VERSION = 1;

TransactionFactory::TransactionFactory() : 
    TransactionFactory(NetworkConfig::GetDefault()) {
}

TransactionFactory::TransactionFactory(const NetworkConfig& networkConfig) : 
    networkConfig(networkConfig),
    gasEstimator(networkConfig) {
}

Proto::TransactionMessage TransactionFactory::createTransaction(const Proto::SigningInput &input) {
    if (input.has_egld_transfer()) {
        return createEGLDTransfer(input.egld_transfer());
    } else if (input.has_esdt_transfer()) {
        return createESDTTransfer(input.esdt_transfer());
    } else if (input.has_esdtnft_transfer()) {
        return createESDTNFTTransfer(input.esdtnft_transfer());
    } else {
        return createGenericTransaction(input.transaction());
    }
}

Proto::TransactionMessage TransactionFactory::createGenericTransaction(const Proto::TransactionMessage& genericTransaction) {
    // Creates merely a clone of the input (by invoking the = operator), without any other logic.
    Proto::TransactionMessage transaction = genericTransaction;
    return transaction;
}

Proto::TransactionMessage TransactionFactory::createEGLDTransfer(const Proto::EGLDTransfer& transfer) {
    Proto::TransactionMessage transaction;

    uint64_t estimatedGasLimit = this->gasEstimator.forEGLDTransfer(0);

    transaction.set_nonce(transfer.nonce());
    transaction.set_sender(transfer.sender());
    transaction.set_sender_username(transfer.sender_username());
    transaction.set_receiver(transfer.receiver());
    transaction.set_receiver_username(transfer.receiver_username());
    transaction.set_value(transfer.amount());
    transaction.set_gas_limit(coalesceGasLimit(transfer.gas_limit(), estimatedGasLimit));
    transaction.set_gas_price(coalesceGasPrice(transfer.gas_price()));
    transaction.set_chain_id(coalesceChainId(transfer.chain_id()));
    transaction.set_version(TX_VERSION);

    return transaction;
}

Proto::TransactionMessage TransactionFactory::createESDTTransfer(const Proto::ESDTTransfer& transfer) {
    Proto::TransactionMessage transaction;

    std::string encodedTokenIdentifier = Codec::encodeString(transfer.token_identifier());
    std::string encodedAmount = Codec::encodeBigInt(transfer.amount());
    std::string data = prepareFunctionCall("ESDTTransfer", { encodedTokenIdentifier, encodedAmount });
    uint64_t estimatedGasLimit = this->gasEstimator.forESDTTransfer(data.size());

    transaction.set_nonce(transfer.nonce());
    transaction.set_sender(transfer.sender());
    transaction.set_sender_username(transfer.sender_username());
    transaction.set_receiver(transfer.receiver());
    transaction.set_receiver_username(transfer.receiver_username());
    transaction.set_value("0");
    transaction.set_data(data);
    transaction.set_gas_limit(coalesceGasLimit(transfer.gas_limit(), estimatedGasLimit));
    transaction.set_gas_price(coalesceGasPrice(transfer.gas_price()));
    transaction.set_chain_id(coalesceChainId(transfer.chain_id()));
    transaction.set_version(TX_VERSION);

    return transaction;
}

Proto::TransactionMessage TransactionFactory::createESDTNFTTransfer(const Proto::ESDTNFTTransfer& transfer) {
    Proto::TransactionMessage transaction;

    std::string encodedCollection = Codec::encodeString(transfer.token_collection());
    std::string encodedNonce = Codec::encodeUint64(transfer.token_nonce());
    std::string encodedQuantity = Codec::encodeBigInt(transfer.amount());
    std::string encodedReceiver = Codec::encodeAddress(transfer.receiver());
    std::string data = prepareFunctionCall("ESDTNFTTransfer", { encodedCollection, encodedNonce, encodedQuantity, encodedReceiver });
    uint64_t estimatedGasLimit = this->gasEstimator.forESDTNFTTransfer(data.size());
    
    transaction.set_nonce(transfer.nonce());
    // For NFT, SFT and MetaESDT, transaction.sender == transaction.receiver.
    transaction.set_sender(transfer.sender());
    transaction.set_receiver(transfer.sender());
    transaction.set_value("0");

    transaction.set_data(data);
    transaction.set_gas_limit(coalesceGasLimit(transfer.gas_limit(), estimatedGasLimit));
    transaction.set_gas_price(coalesceGasPrice(transfer.gas_price()));
    transaction.set_chain_id(coalesceChainId(transfer.chain_id()));
    transaction.set_version(TX_VERSION);

    return transaction;
}

uint64_t TransactionFactory::coalesceGasLimit(uint64_t providedGasLimit, uint64_t estimatedGasLimit) {
    return providedGasLimit > 0 ? providedGasLimit : estimatedGasLimit;
}

uint64_t TransactionFactory::coalesceGasPrice(uint64_t gasPrice) {
    return gasPrice > 0 ? gasPrice : this->networkConfig.getMinGasPrice();
}

std::string TransactionFactory::coalesceChainId(std::string chainID) {
    return chainID.empty() ? this->networkConfig.getChainId() : chainID;
}

std::string TransactionFactory::prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments) {
    const auto ARGUMENTS_SEPARATOR = "@";
    std::string result;

    result.append(function);

    for (auto argument : arguments) {
        result.append(ARGUMENTS_SEPARATOR);
        result.append(argument);
    }

    return result;
}
