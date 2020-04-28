// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OperationList.h"
#include "Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCurve.h>
#include <google/protobuf/util/json_util.h>

#include <string>

using namespace TW;
using namespace TW::Tezos;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto operationList = Tezos::OperationList(input.operation_list().branch());
    for (Proto::Operation operation : input.operation_list().operations()) {
      operationList.addOperation(operation);
    }

    auto signer = Signer();
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data encoded = signer.signOperationList(key, operationList);

    auto output = Proto::SigningOutput();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return hex(output.encoded());
}

Data Signer::signOperationList(const PrivateKey& privateKey, const OperationList& operationList) {
    auto forged = operationList.forge(privateKey);
    return signData(privateKey, forged);
}

Data Signer::signData(const PrivateKey& privateKey, const Data& data) {
    Data watermarkedData = Data();
    watermarkedData.push_back(0x03);
    append(watermarkedData, data);

    Data hash = Hash::blake2b(watermarkedData, 32);
    Data signature = privateKey.sign(hash, TWCurve::TWCurveED25519);

    Data signedData = Data();
    append(signedData, data);
    append(signedData, signature);
    return signedData;
}
