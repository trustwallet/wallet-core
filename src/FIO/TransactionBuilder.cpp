// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionBuilder.h"
#include "Signer.h"
#include "../HexCoding.h"

#include <sstream>
#include <ctime>

namespace TW::FIO {

using namespace TW;
using namespace std;

string TransactionBuilder::createTx(const Address& address, const PrivateKey& privateKey, const string& fioName,
    const vector<pair<string, string>>& pubAddresses,
    const ChainParams& chainParams, const string& walletFioName, uint32_t expiryTime) {

    uint64_t maxFee = 0; 
    const auto apiName = "addaddress";

    string actor = Actor::actor(address);
    AddAddressData aaData(fioName, pubAddresses, maxFee, walletFioName, actor);
    Data serData;
    aaData.serialize(serData);
    
    Action action;
    action.account = ApiAccountAddress;
    action.name = apiName;
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

    // create signature
    Data sigBuf(chainParams.chainId);
    append(sigBuf, serTx);
    append(sigBuf, TW::Data(32)); // context_free
    string signature = Signer::signatureToBsase58(Signer::sign(privateKey, sigBuf));

    // Build json
    stringstream ss;
    ss << "{" << endl;
    ss << "\"signatures\": [\"" << signature << "\"]," << endl; 
    ss << "\"compression\": \"none\"," << endl << "\"packed_context_free_data\": \"\"," << endl;
    ss << "\"packed_trx\": \"" << hex(serTx) << "\"" << endl;
    ss << "}";
    return ss.str();
}

} // namespace TW::FIO
