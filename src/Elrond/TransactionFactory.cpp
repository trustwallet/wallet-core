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

std::string prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments);

const int TX_VERSION = 1;

TransactionFactory::TransactionFactory(const NetworkConfig& networkConfig) : 
    networkConfig(networkConfig),
    gasEstimator(networkConfig) {
}

Proto::TransactionMessage TransactionFactory::createEGLDTransfer(const Address& sender, const Address& receiver, uint256_t amount) {
    Proto::TransactionMessage message;

    long gasLimit = this->gasEstimator.forEGLDTransfer(0);

    message.set_sender(sender.string());
    message.set_receiver(receiver.string());
    message.set_value(toString(amount));
    message.set_gas_limit(gasLimit);
    message.set_gas_price(this->networkConfig.getMinGasPrice());
    message.set_chain_id(this->networkConfig.getChainId());
    message.set_version(TX_VERSION);

    return message;
}

Proto::TransactionMessage TransactionFactory::createESDTTransfer(const Address& sender, const Address& receiver, const std::string& tokenIdentifier, uint256_t amount) {
    Proto::TransactionMessage message;

    std::string encodedTokenIdentifier = Codec::encodeStringTopLevel(tokenIdentifier);
    std::string encodedAmount = Codec::encodeBigIntTopLevel(amount);
    std::string data = prepareFunctionCall("ESDTTransfer", { encodedTokenIdentifier, encodedAmount });
    long gasLimit = this->gasEstimator.forESDTTransfer(data.size());

    message.set_sender(sender.string());
    message.set_receiver(receiver.string());
    message.set_value(toString(0));
    message.set_data(data);
    message.set_gas_limit(gasLimit);
    message.set_gas_price(this->networkConfig.getMinGasPrice());
    message.set_chain_id(this->networkConfig.getChainId());
    message.set_version(TX_VERSION);

    return message;
}

Proto::TransactionMessage TransactionFactory::createESDTNFTTransfer(const Address& sender, const Address& receiver, const std::string& collection, uint64_t nonce, uint256_t quantity) {
    Proto::TransactionMessage message;

    std::string encodedCollection = Codec::encodeStringTopLevel(collection);
    std::string encodedNonce = Codec::encodeUint64TopLevel(nonce);
    std::string encodedQuantity = Codec::encodeBigIntTopLevel(quantity);
    std::string encodedReceiver = Codec::encodeAddressTopLevel(receiver);
    std::string data = prepareFunctionCall("ESDTNFTTransfer", { encodedCollection, encodedNonce, encodedQuantity, encodedReceiver });
    long gasLimit = this->gasEstimator.forESDTNFTTransfer(data.size());
    
    // For NFT, SFT and MetaESDT, transaction.sender == transaction.receiver.
    message.set_sender(sender.string());
    message.set_receiver(sender.string());
    message.set_value(toString(0));

    message.set_data(data);
    message.set_gas_limit(gasLimit);
    message.set_gas_price(this->networkConfig.getMinGasPrice());
    message.set_chain_id(this->networkConfig.getChainId());
    message.set_version(TX_VERSION);

    return message;
}

std::string prepareFunctionCall(const std::string& function, std::initializer_list<const std::string> arguments) {
    const auto ARGUMENTS_SEPARATOR = "@";
    std::string result;

    result.append(function);

    for(auto argument : arguments)
    {
        result.append(ARGUMENTS_SEPARATOR);
        result.append(argument);
    }

    return result;
}
