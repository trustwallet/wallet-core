// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "Address.h"
#include "../Base64.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::Seele;
using namespace TW::Seele::Proto;

using json = nlohmann::json;

json seeleHigherWrapperJSON(json& jsonObj) {
    json jsonMsgWrapper;

    jsonMsgWrapper["result"] = jsonObj;

    return jsonMsgWrapper;
}

// send coin
json seeleSendCoinsMessageJSON(
        const Seele::Proto::SignTransaction& signTransaction) {
    json jsonMsg;

    jsonMsg["type"] = signTransaction.type();
    jsonMsg["from"] = signTransaction.from();
    jsonMsg["to"] = signTransaction.to();
    jsonMsg["amount"] = signTransaction.amount();
    jsonMsg["accountNonce"] = signTransaction.account_nonce();
    jsonMsg["gasPrice"] = signTransaction.gas_price();
    jsonMsg["gasLimit"] = signTransaction.gas_limit();
    jsonMsg["timestamp"] = signTransaction.timestamp();
    jsonMsg["payload"] = signTransaction.payload();

    return (jsonMsg);
}


json seeleSignatureJSON(const Seele::Proto::Signature& signature) {
    json jsonSignature;

    jsonSignature["sig"] = Base64::encode(Data(signature.sig().begin(), signature.sig().end()));

    return jsonSignature;
}

json TW::Seele::signaturePreimageJSON(const Seele::Proto::SigningInput& signingInput) {
    json jsonForSigning;

    jsonForSigning["data"] = seeleSendCoinsMessageJSON(signingInput.sign_transaction());
    //jsonForSigning["signature"] = new Signature();
    //jsonForSigning["hash"] = "";

    return jsonForSigning;
}

json TW::Seele::transactionJSON(const Seele::Proto::Transaction& transaction) {
    json jsonTx;

    jsonTx["data"] = seeleSendCoinsMessageJSON(transaction.data());
    jsonTx["signature"] = seeleSignatureJSON(transaction.signature());
    jsonTx["hash"] = transaction.hash();


    return seeleHigherWrapperJSON(jsonTx);
}
