// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#include "OperationList.h"
#include "Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCurve.h>

using namespace TW;
using namespace TW::Tezos;

Data Signer::signOperationList(const PrivateKey& privateKey, OperationList operationList) {
    auto forged = operationList.forge();
    return signData(privateKey, forged);
}

Data Signer::signData(const PrivateKey& privateKey, Data data) {
    Data watermarkedData = Data();
    watermarkedData.push_back(0x03);
    append(watermarkedData, data);
    
    Data hash = Hash::blake2b(watermarkedData, 32);
    TW::PublicKey pk = privateKey.getPublicKey(PublicKeyType::ed25519);
    Data signature = privateKey.sign(hash, TWCurve::TWCurveEd25519);

    Data signedData = Data();
    append(signedData, data);
    append(signedData, signature);
    return signedData;
}
