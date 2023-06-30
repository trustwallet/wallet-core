// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "OperationList.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCurve.h>
#include <google/protobuf/util/json_util.h>

#include <string>

using namespace TW;

namespace TW::Tezos {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer();
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data encoded;
    if (input.encoded_operations().empty()) {
        auto operationList = Tezos::OperationList(input.operation_list().branch());
        for (Proto::Operation operation : input.operation_list().operations()) {
            operationList.addOperation(operation);
        }
        encoded = signer.signOperationList(key, operationList);
    } else {
        encoded = signer.signData(key, TW::data(input.encoded_operations()));
    }

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

Data Signer::buildUnsignedTx(const OperationList& operationList) {
    Data txData = operationList.forge();
    return txData;
}

Data Signer::buildSignedTx(const OperationList& operationList, Data signature) {
    Data signedData = Data();

    Data txData = operationList.forge();

    append(signedData, txData);
    append(signedData, signature);

    return signedData;
}

} // namespace TW::Tezos
