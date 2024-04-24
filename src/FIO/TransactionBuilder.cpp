// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionBuilder.h"

#include "Actor.h"
#include "Encryption.h"
#include "NewFundsRequest.h"
#include "Signer.h"

#include "../HexCoding.h"

#include <nlohmann/json.hpp>

#include <ctime>

namespace TW::FIO {

using namespace TW;
using namespace std;
using json = nlohmann::json;

static constexpr auto gRegisterFioAddress = "regaddress";
static constexpr auto gAddPubAddress = "addaddress";
static constexpr auto gRemoveAddress = "remaddress";
static constexpr auto gRemoveAllPubAddresses = "remalladdr";
static constexpr auto gTransferFIOPubkey = "trnsfiopubky";
static constexpr auto gRenewFIOAddress = "renewaddress";
static constexpr auto gNewFundsRequest = "newfundsreq";
static constexpr auto gAddBundledTransactions = "addbundles";

/// Internal helper
ChainParams getChainParams(const Proto::SigningInput& input) {
    return ChainParams{
        TW::data(input.chain_params().chain_id()),
        input.chain_params().head_block_number(),
        input.chain_params().ref_block_prefix()
    };
}

bool TransactionBuilder::expirySetDefaultIfNeeded(uint32_t& expiryTime) {
    if (expiryTime != 0) { return false; } // no change
    // fill based on current time 
    expiryTime = (uint32_t)time(nullptr) + ExpirySeconds;
    return true;
}

string TransactionBuilder::actionName(const Proto::SigningInput& input) {
    switch (input.action().message_oneof_case()) {
        case Proto::Action::MessageOneofCase::kRegisterFioAddressMessage:
            return gRegisterFioAddress;
        case Proto::Action::MessageOneofCase::kAddPubAddressMessage:
            return gAddPubAddress;
        case Proto::Action::MessageOneofCase::kTransferMessage:
            return gTransferFIOPubkey;
        case Proto::Action::MessageOneofCase::kRenewFioAddressMessage:
            return gRenewFIOAddress;
        case Proto::Action::MessageOneofCase::kNewFundsRequestMessage:
            return gNewFundsRequest;
        case Proto::Action::MessageOneofCase::kRemovePubAddressMessage:
            return gRemoveAddress;
        case Proto::Action::MessageOneofCase::kRemoveAllPubAddressesMessage:
            return gRemoveAllPubAddresses;
        case Proto::Action::MessageOneofCase::kAddBundledTransactionsMessage:
            return gAddBundledTransactions;
        default:
            return {};
    }
}

string TransactionBuilder::sign(Proto::SigningInput in) {
    PrivateKey privateKey(in.private_key());
    PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    Address owner(publicKey);
    
    string json;
    if (in.action().has_register_fio_address_message()) {
        const auto action = in.action().register_fio_address_message();
        json = TransactionBuilder::createRegisterFioAddress(owner, privateKey, 
            in.action().register_fio_address_message().fio_address(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_add_pub_address_message()) {
        const auto action = in.action().add_pub_address_message();
        // process addresses
        std::vector<std::pair<std::string, std::string>> addresses;
        for (int i = 0; i < action.public_addresses_size(); ++i) {
            addresses.emplace_back(std::make_pair(action.public_addresses(i).coin_symbol(), action.public_addresses(i).address()));
        }
        json = TransactionBuilder::createAddPubAddress(owner, privateKey,
            action.fio_address(), addresses, 
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_transfer_message()) {
        const auto action = in.action().transfer_message();
        json = TransactionBuilder::createTransfer(owner, privateKey,
            action.payee_public_key(), action.amount(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_renew_fio_address_message()) {
        const auto action = in.action().renew_fio_address_message();
        json = TransactionBuilder::createRenewFioAddress(owner, privateKey,
            action.fio_address(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_new_funds_request_message()) {
        const auto action = in.action().new_funds_request_message();
        const auto content = action.content();
        json = TransactionBuilder::createNewFundsRequest(owner, privateKey,
            action.payer_fio_name(), action.payer_fio_address(), action.payee_fio_name(), content.payee_public_address(),
            content.amount(), content.coin_symbol(), content.memo(), content.hash(), content.offline_url(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry(), Data());
    } else if (in.action().has_remove_pub_address_message()) {
        const auto action = in.action().remove_pub_address_message();
        // process addresses
        std::vector<std::pair<std::string, std::string>> addresses;
        for (int i = 0; i < action.public_addresses_size(); ++i) {
            addresses.emplace_back(std::make_pair(action.public_addresses(i).coin_symbol(), action.public_addresses(i).address()));
        }
        json = TransactionBuilder::createRemovePubAddress(owner, privateKey,
            action.fio_address(), addresses,
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_remove_all_pub_addresses_message()) {
        const auto action = in.action().remove_all_pub_addresses_message();
        json = TransactionBuilder::createRemoveAllPubAddresses(owner, privateKey,
            action.fio_address(), getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_add_bundled_transactions_message()) {
        const auto action = in.action().add_bundled_transactions_message();
        json = TransactionBuilder::createAddBundledTransactions(owner, privateKey, action.fio_address(),
            action.bundle_sets(), getChainParams(in), action.fee(), in.tpid(), in.expiry());
    }
    return json;
}

string TransactionBuilder::createRegisterFioAddress(const Address& address, const PrivateKey& privateKey, 
    const string& fioName,
    const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedRegisterFioAddress(address, fioName, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createAddPubAddress(const Address& address, const PrivateKey& privateKey, const string& fioName,
    const vector<pair<string, string>>& pubAddresses,
    const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedPubAddressAction(gAddPubAddress, address, fioName, pubAddresses, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createRemovePubAddress(const Address& address, const PrivateKey& privateKey, const string& fioName,
    const vector<pair<string, string>>& pubAddresses,
    const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedPubAddressAction(gRemoveAddress, address, fioName, pubAddresses, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

std::string TransactionBuilder::createRemoveAllPubAddresses(const Address& address, const PrivateKey& privateKey, const std::string& fioName,
    const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedRemoveAllAddressesAction(address, fioName, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

std::string TransactionBuilder::createAddBundledTransactions(const Address& address, const PrivateKey& privateKey, const std::string& fioName,
    uint64_t bundleSets, const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedAddBundledTransactions(address, fioName, bundleSets, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createTransfer(const Address& address, const PrivateKey& privateKey, 
        const string& payeePublicKey, uint64_t amount,
        const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedTransfer(address, payeePublicKey, amount, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createRenewFioAddress(const Address& address, const PrivateKey& privateKey, 
    const string& fioName,
    const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime) {

    Transaction transaction = TransactionBuilder::buildUnsignedRenewFioAddress(address, fioName, chainParams, fee, walletTpId, expiryTime);

    Data serTx;
    transaction.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createNewFundsRequest(const Address& address, const PrivateKey& privateKey,
        const string& payerFioName, const string& payerFioAddress, const string& payeeFioName, const string& payeePublicAddress, 
        const string& amount, const string& coinSymbol, const string& memo, const string& hash, const string& offlineUrl,
        const ChainParams& chainParams, uint64_t fee, const string& walletTpId, uint32_t expiryTime,
        const Data& iv) {

    // use coinSymbol for chainCode as well
    NewFundsContent newFundsContent { payeePublicAddress, amount, coinSymbol, coinSymbol, memo, hash, offlineUrl };
    // serialize and encrypt
    Data serContent;
    newFundsContent.serialize(serContent);

    Address payerAddress(payerFioAddress);
    PublicKey payerPublicKey = payerAddress.publicKey();
    // encrypt and encode
    const string encodedEncryptedContent = Encryption::encode(Encryption::encrypt(privateKey, payerPublicKey, serContent, iv));

    string actor = Actor::actor(address);
    NewFundsRequestData nfData(payerFioName, payeeFioName, encodedEncryptedContent, fee, walletTpId, actor);
    Data serData;
    nfData.serialize(serData);
    
    Action action;
    action.account = ContractPayRequest;
    action.name = gNewFundsRequest;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    Data serTx;
    tx.serialize(serTx);

    return signAndBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::signAndBuildTx(const Data& chainId, const Data& packedTx, const PrivateKey& privateKey) {
    // create signature
    Data sigBuf = buildPreSignTxData(chainId, packedTx);
    string signature = Signer::signatureToBase58(Signer::signData(privateKey, sigBuf));

    // Build json
    json tx = {
        {"signatures", json::array({signature})},
        {"compression", "none"},
        {"packed_context_free_data", ""},
        {"packed_trx", hex(packedTx)}
    };
    return tx.dump();
}

Data TransactionBuilder::buildPreSignTxData(const Data& chainId, const Data& packedTx) {
    // create signature
    Data sigBuf(chainId);
    append(sigBuf, packedTx);
    append(sigBuf, TW::Data(32)); // context_free
    return sigBuf;
}

Data TransactionBuilder::buildUnsignedTxBytes(const Proto::SigningInput& in) {
    Address owner(in.owner_public_key());

    Transaction transaction;
    if (in.action().has_register_fio_address_message()) {
        const auto action = in.action().register_fio_address_message();
        transaction = TransactionBuilder::buildUnsignedRegisterFioAddress(owner, 
            in.action().register_fio_address_message().fio_address(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_add_pub_address_message()) {
        const auto action = in.action().add_pub_address_message();
        // process addresses
        std::vector<std::pair<std::string, std::string>> addresses;
        for (int i = 0; i < action.public_addresses_size(); ++i) {
            addresses.emplace_back(std::make_pair(action.public_addresses(i).coin_symbol(), action.public_addresses(i).address()));
        }
        transaction = TransactionBuilder::buildUnsignedPubAddressAction(gAddPubAddress, owner, action.fio_address(), addresses,
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_transfer_message()) {
        const auto action = in.action().transfer_message();
        transaction = TransactionBuilder::buildUnsignedTransfer(owner, action.payee_public_key(), action.amount(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_renew_fio_address_message()) {
        const auto action = in.action().renew_fio_address_message();
        transaction = TransactionBuilder::buildUnsignedRenewFioAddress(owner, action.fio_address(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_remove_pub_address_message()) {
        const auto action = in.action().remove_pub_address_message();
        // process addresses
        std::vector<std::pair<std::string, std::string>> addresses;
        for (int i = 0; i < action.public_addresses_size(); ++i) {
            addresses.emplace_back(std::make_pair(action.public_addresses(i).coin_symbol(), action.public_addresses(i).address()));
        }
        transaction = TransactionBuilder::buildUnsignedPubAddressAction(gRemoveAddress, owner, action.fio_address(), addresses,
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_remove_all_pub_addresses_message()) {
        const auto action = in.action().remove_all_pub_addresses_message();
        transaction = TransactionBuilder::buildUnsignedRemoveAllAddressesAction(owner, action.fio_address(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    } else if (in.action().has_add_bundled_transactions_message()) {
        const auto action = in.action().add_bundled_transactions_message();
        transaction = TransactionBuilder::buildUnsignedAddBundledTransactions(owner, action.fio_address(), action.bundle_sets(),
            getChainParams(in), action.fee(), in.tpid(), in.expiry());
    }

    Data serTx;
    transaction.serialize(serTx);

    return serTx;
}

Proto::SigningOutput TransactionBuilder::buildSigningOutput(const Proto::SigningInput &input, const Data &signature) {
    FIO::Proto::SigningOutput output;
    Data serTx = buildUnsignedTxBytes(input);
    
    string signatureString = Signer::signatureToBase58(signature);
    // Build json
    json tx = {
        {"signatures", json::array({signatureString})},
        {"compression", "none"},
        {"packed_context_free_data", ""},
        {"packed_trx", hex(serTx)}
    };

    output.set_json(tx.dump());
    output.set_action_name(actionName(input));
    return output;
}

Transaction TransactionBuilder::buildUnsignedRegisterFioAddress(const Address& address, const std::string& fioName, const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime){
    string actor = Actor::actor(address);
    RegisterFioAddressData raData(fioName, address.string(), fee, walletTpId, actor);
    Data serData;
    raData.serialize(serData);
    
    Action action;
    action.account = ContractAddress;
    action.name = gRegisterFioAddress;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

Transaction TransactionBuilder::buildUnsignedPubAddressAction(const std::string& apiName, const Address& address,
    const std::string& fioName, const std::vector<std::pair<std::string, std::string>>& pubAddresses,
    const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {

    string actor = Actor::actor(address);
    // convert addresses to add chainCode -- set it equal to coinSymbol
    vector<PublicAddress> pubAddresses2;
    for (const auto& a: pubAddresses) {
        pubAddresses2.push_back(PublicAddress{a.first, a.first, a.second});
    }
    PubAddressActionData actionData(fioName, pubAddresses2, fee, walletTpId, actor);
    Data serData;
    actionData.serialize(serData);

    Action action;
    action.account = ContractAddress;
    action.name = apiName;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

Transaction TransactionBuilder::buildUnsignedTransfer(const Address& address, const std::string& payeePublicKey, uint64_t amount, const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {
    string actor = Actor::actor(address);
    TransferData ttData(payeePublicKey, amount, fee, walletTpId, actor);
    Data serData;
    ttData.serialize(serData);
    
    Action action;
    action.account = ContractToken;
    action.name = gTransferFIOPubkey;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

Transaction TransactionBuilder::buildUnsignedRenewFioAddress(const Address& address, const std::string& fioName, const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {
    string actor = Actor::actor(address);
    RenewFioAddressData raData(fioName, fee, walletTpId, actor);
    Data serData;
    raData.serialize(serData);
    
    Action action;
    action.account = ContractAddress;
    action.name = gRenewFIOAddress;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

Transaction TransactionBuilder::buildUnsignedRemoveAllAddressesAction(const Address& address, const std::string& fioName,
    const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {

    string actor = Actor::actor(address);
    RemoveAllPubAddressActionData actionData(fioName, fee, walletTpId, actor);
    Data serData;
    actionData.serialize(serData);

    Action action;
    action.account = ContractAddress;
    action.name = gRemoveAllPubAddresses;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

Transaction TransactionBuilder::buildUnsignedAddBundledTransactions(const Address& address, const std::string& fioName,
    uint64_t bundleSets, const ChainParams& chainParams, uint64_t fee, const std::string& walletTpId, uint32_t expiryTime) {

    string actor = Actor::actor(address);
    AddBundledTransactionsActionData actionData(fioName, bundleSets, fee, walletTpId, actor);
    Data serData;
    actionData.serialize(serData);

    Action action;
    action.account = ContractAddress;
    action.name = gAddBundledTransactions;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});

    Transaction tx;
    expirySetDefaultIfNeeded(expiryTime);
    tx.set(expiryTime, chainParams);
    tx.actions.push_back(action);
    return tx;
}

} // namespace TW::FIO
