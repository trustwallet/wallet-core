// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionFactory.h"

#include "Codec.h"

namespace TW::MultiversX {

const int TX_VERSION = 2;

TransactionFactory::TransactionFactory()
    : TransactionFactory(TransactionFactoryConfig::GetDefault()) {
}

TransactionFactory::TransactionFactory(const TransactionFactoryConfig& config)
    : config(config) {
}

Transaction TransactionFactory::create(const Proto::SigningInput& input) {
    if (input.has_egld_transfer()) {
        return fromEGLDTransfer(input);
    } else if (input.has_esdt_transfer()) {
        return fromESDTTransfer(input);
    } else if (input.has_esdtnft_transfer()) {
        return fromESDTNFTTransfer(input);
    } else {
        return fromGenericAction(input);
    }
}

/// Copies the input fields into a transaction object, without any other logic.
Transaction TransactionFactory::fromGenericAction(const Proto::SigningInput& input) {
    auto action = input.generic_action();

    Transaction transaction;
    transaction.nonce = action.accounts().sender_nonce();
    transaction.sender = action.accounts().sender();
    transaction.senderUsername = action.accounts().sender_username();
    transaction.receiver = action.accounts().receiver();
    transaction.receiverUsername = action.accounts().receiver_username();
    transaction.guardian = action.accounts().guardian();
    transaction.value = action.value();
    transaction.data = action.data();
    transaction.gasLimit = input.gas_limit();
    transaction.gasPrice = input.gas_price();
    transaction.chainID = input.chain_id();
    transaction.version = action.version();
    transaction.options = static_cast<TransactionOptions>(action.options());

    return transaction;
}

Transaction TransactionFactory::fromEGLDTransfer(const Proto::SigningInput& input) {
    auto transfer = input.egld_transfer();

    Transaction transaction;
    transaction.nonce = transfer.accounts().sender_nonce();
    transaction.sender = transfer.accounts().sender();
    transaction.senderUsername = transfer.accounts().sender_username();
    transaction.receiver = transfer.accounts().receiver();
    transaction.receiverUsername = transfer.accounts().receiver_username();
    transaction.guardian = transfer.accounts().guardian();
    transaction.value = transfer.amount();
    transaction.data = transfer.data();
    transaction.gasPrice = coalesceGasPrice(input.gas_price());
    transaction.chainID = coalesceChainId(input.chain_id());
    transaction.version = transfer.version() ? transfer.version() : TX_VERSION;
    transaction.options = decideOptions(transaction);

    // Estimate & set gasLimit:
    uint64_t estimatedGasLimit = computeGasLimit(0, 0, transaction.hasGuardian());
    transaction.gasLimit = coalesceGasLimit(input.gas_limit(), estimatedGasLimit);

    return transaction;
}

Transaction TransactionFactory::fromESDTTransfer(const Proto::SigningInput& input) {
    auto transfer = input.esdt_transfer();

    std::string encodedTokenIdentifier = Codec::encodeString(transfer.token_identifier());
    std::string encodedAmount = Codec::encodeBigInt(transfer.amount());
    std::string data = prepareFunctionCall("ESDTTransfer", {encodedTokenIdentifier, encodedAmount});

    Transaction transaction;
    transaction.nonce = transfer.accounts().sender_nonce();
    transaction.sender = transfer.accounts().sender();
    transaction.senderUsername = transfer.accounts().sender_username();
    transaction.receiver = transfer.accounts().receiver();
    transaction.receiverUsername = transfer.accounts().receiver_username();
    transaction.guardian = transfer.accounts().guardian();
    transaction.value = "0";
    transaction.data = data;
    transaction.gasPrice = coalesceGasPrice(input.gas_price());
    transaction.chainID = coalesceChainId(input.chain_id());
    transaction.version = transfer.version() ? transfer.version() : TX_VERSION;
    transaction.options = decideOptions(transaction);

    // Estimate & set gasLimit:
    uint64_t executionGasLimit = this->config.getGasCostESDTTransfer() + this->config.getAdditionalGasForESDTTransfer();
    uint64_t estimatedGasLimit = computeGasLimit(data.size(), executionGasLimit, transaction.hasGuardian());
    transaction.gasLimit = coalesceGasLimit(input.gas_limit(), estimatedGasLimit);

    return transaction;
}

Transaction TransactionFactory::fromESDTNFTTransfer(const Proto::SigningInput& input) {
    auto transfer = input.esdtnft_transfer();

    std::string encodedCollection = Codec::encodeString(transfer.token_collection());
    std::string encodedNonce = Codec::encodeUint64(transfer.token_nonce());
    std::string encodedQuantity = Codec::encodeBigInt(transfer.amount());
    std::string encodedReceiver = Codec::encodeAddress(transfer.accounts().receiver());
    std::string data = prepareFunctionCall("ESDTNFTTransfer", {encodedCollection, encodedNonce, encodedQuantity, encodedReceiver});

    Transaction transaction;
    transaction.nonce = transfer.accounts().sender_nonce();
    // For NFT, SFT and MetaESDT, transaction.sender == transaction.receiver.
    transaction.sender = transfer.accounts().sender();
    transaction.receiver = transfer.accounts().sender();
    transaction.guardian = transfer.accounts().guardian();
    transaction.value = "0";
    transaction.data = data;
    transaction.gasPrice = coalesceGasPrice(input.gas_price());
    transaction.chainID = coalesceChainId(input.chain_id());
    transaction.version = transfer.version() ? transfer.version() : TX_VERSION;
    transaction.options = decideOptions(transaction);

    // Estimate & set gasLimit:
    uint64_t executionGasLimit = this->config.getGasCostESDTNFTTransfer() + this->config.getAdditionalGasForESDTNFTTransfer();
    uint64_t estimatedGasLimit = computeGasLimit(data.size(), executionGasLimit, transaction.hasGuardian());
    transaction.gasLimit = coalesceGasLimit(input.gas_limit(), estimatedGasLimit);

    return transaction;
}

uint64_t TransactionFactory::computeGasLimit(size_t dataLength, uint64_t executionGasLimit, bool hasGuardian) {
    uint64_t dataMovementGasLimit = this->config.getMinGasLimit() + this->config.getGasPerDataByte() * dataLength;
    uint64_t gasLimit = dataMovementGasLimit + executionGasLimit;

    if (hasGuardian) {
        gasLimit += this->config.getExtraGasLimitForGuardedTransaction();
    }

    return gasLimit;
}

uint64_t TransactionFactory::coalesceGasLimit(uint64_t providedGasLimit, uint64_t estimatedGasLimit) {
    return providedGasLimit > 0 ? providedGasLimit : estimatedGasLimit;
}

uint64_t TransactionFactory::coalesceGasPrice(uint64_t gasPrice) {
    return gasPrice > 0 ? gasPrice : this->config.getMinGasPrice();
}

std::string TransactionFactory::coalesceChainId(std::string chainID) {
    return chainID.empty() ? this->config.getChainId() : chainID;
}

TransactionOptions TransactionFactory::decideOptions(const Transaction& transaction) {
    TransactionOptions options = TransactionOptions::Default;

    if (transaction.hasGuardian()) {
        options = static_cast<TransactionOptions>(options | TransactionOptions::Guarded);
    }

    return options;
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

} // namespace TW::MultiversX
