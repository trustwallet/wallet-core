// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include <TrezorCrypto/ed25519.h>

#include <algorithm>

using namespace TW;
using namespace TW::Solana;

void Signer::sign(const std::vector<PrivateKey> &privateKeys, Transaction &transaction) {
    for (auto privateKey : privateKeys) {
        auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
        auto index = transaction.getAccountIndex(address);
        auto message = transaction.messageData();
        auto signature = Signature(privateKey.sign(message, TWCurveED25519));
        transaction.signatures[index] = signature;
    }
}

void Signer::signUpdateBlockhash(const std::vector<PrivateKey> &privateKeys,
                                 Transaction &transaction, Solana::Hash &recentBlockhash) {
    transaction.message.recentBlockhash = recentBlockhash;
    Signer::sign(privateKeys, transaction);
}

// This method does not confirm that PrivateKey order matches that encoded in the messageData
// That order must be correct for the Transaction to succeed on Solana
Data Signer::signRawMessage(const std::vector<PrivateKey> &privateKeys, const Data messageData) {
    std::vector<Signature> signatures;
    for (auto privateKey : privateKeys) {
        auto signature = Signature(privateKey.sign(messageData, TWCurveED25519));
        signatures.push_back(signature);
    }
    Data buffer;
    append(buffer, shortVecLength<Signature>(signatures));
    for (auto signature : signatures) {
        Data signature_vec(signature.bytes.begin(), signature.bytes.end());
        append(buffer, signature_vec);
    }
    append(buffer, messageData);

    return buffer;
}
