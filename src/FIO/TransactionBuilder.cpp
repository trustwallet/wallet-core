// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionBuilder.h"
#include "Signer.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>

#include <ctime>

namespace TW::FIO {

using namespace TW;
using namespace std;
using json = nlohmann::json;


string TransactionBuilder::createRegisterFioAddress(const Address& address, const PrivateKey& privateKey, 
    const string& fioName, const std::string& ownerPublicKey,
    const ChainParams& chainParams, uint64_t fee, const string& walletFioName, uint32_t expiryTime) {

    const auto apiName = "regaddress";

    string actor = Actor::actor(address);
    RegisterFioAddressData raData(fioName, ownerPublicKey, fee, walletFioName, actor);
    Data serData;
    raData.serialize(serData);
    
    Action action;
    action.account = ApiAccountAddress;
    action.name = apiName;
    action.includeExtra01BeforeData = false;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});
    Data serAction;
    action.serialize(serAction);

    Transaction tx;
    if (expiryTime == 0) {
        expiryTime = (uint32_t)time(nullptr) + ExpirySeconds;
    }
    tx.expiration = (int32_t)expiryTime;
    tx.refBlockNumber = (uint16_t)(chainParams.headBlockNumber & 0xffff);
    tx.refBlockPrefix = chainParams.refBlockPrefix;
    tx.actions.push_back(action);
    Data serTx;
    tx.serialize(serTx);

    return signAdnBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createAddPubAddress(const Address& address, const PrivateKey& privateKey, const string& fioName,
    const vector<pair<string, string>>& pubAddresses,
    const ChainParams& chainParams, uint64_t fee, const string& walletFioName, uint32_t expiryTime) {

    const auto apiName = "addaddress";

    string actor = Actor::actor(address);
    AddPubAddressData aaData(fioName, pubAddresses, fee, walletFioName, actor);
    Data serData;
    aaData.serialize(serData);
    
    Action action;
    action.account = ApiAccountAddress;
    action.name = apiName;
    action.includeExtra01BeforeData = true;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});
    Data serAction;
    action.serialize(serAction);

    Transaction tx;
    if (expiryTime == 0) {
        expiryTime = (uint32_t)time(nullptr) + ExpirySeconds;
    }
    tx.expiration = (int32_t)expiryTime;
    tx.refBlockNumber = (uint16_t)(chainParams.headBlockNumber & 0xffff);
    tx.refBlockPrefix = chainParams.refBlockPrefix;
    tx.actions.push_back(action);
    Data serTx;
    tx.serialize(serTx);

    return signAdnBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::createTransfer(const Address& address, const PrivateKey& privateKey, 
        const string& payeePublicKey, uint64_t amount,
        const ChainParams& chainParams, uint64_t fee, const string& walletFioName, uint32_t expiryTime) {

    const auto apiName = "trnsfiopubky";

    string actor = Actor::actor(address);
    TransferData ttData(payeePublicKey, amount, fee, walletFioName, actor);
    Data serData;
    ttData.serialize(serData);
    
    Action action;
    action.account = ApiAccountToken;
    action.name = apiName;
    action.includeExtra01BeforeData = false;
    action.actionDataSer = serData;
    action.auth.authArray.push_back(Authorization{actor, AuthrizationActive});
    Data serAction;
    action.serialize(serAction);

    Transaction tx;
    if (expiryTime == 0) {
        expiryTime = (uint32_t)time(nullptr) + ExpirySeconds;
    }
    tx.expiration = (int32_t)expiryTime;
    tx.refBlockNumber = (uint16_t)(chainParams.headBlockNumber & 0xffff);
    tx.refBlockPrefix = chainParams.refBlockPrefix;
    tx.actions.push_back(action);
    Data serTx;
    tx.serialize(serTx);

    return signAdnBuildTx(chainParams.chainId, serTx, privateKey);
}

string TransactionBuilder::signAdnBuildTx(const Data& chainId, const Data& packedTx, const PrivateKey& privateKey) {
    // create signature
    Data sigBuf(chainId);
    append(sigBuf, packedTx);
    append(sigBuf, TW::Data(32)); // context_free
    string signature = Signer::signatureToBsase58(Signer::sign(privateKey, sigBuf));

    // Build json
    json tx = {
        {"signatures", json::array({signature})},
        {"compression", "none"},
        {"packed_context_free_data", ""},
        {"packed_trx", hex(packedTx)}
    };
    return tx.dump();
}

} // namespace TW::FIO
