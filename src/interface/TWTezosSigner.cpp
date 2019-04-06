// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWTezosSigner.h>

#include "../Tezos/BinaryCoding.h"
#include "../Tezos/Signer.h"
#include "../proto/Tezos.pb.h"

using namespace TW;
using namespace TW::Tezos;

TW_Tezos_Proto_SigningOutput TWTezosSignerSign(TW_Tezos_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto operationList = OperationList(input.operation_list().branch());
    for (auto& operationProto : input.operation_list().operations()) {
        if (operationProto.has_reveal_operation_data()) {
            auto source = Address(operationProto.source());
            auto publicKey = PublicKey(operationProto.reveal_operation_data().public_key());
            auto operation = Transaction(source,
                                         operationProto.fee(),
                                         operationProto.counter(),
                                         operationProto.gas_limit(),
                                         operationProto.storage_limit(),
                                         0,
                                         publicKey,
                                         operationtype::REVEAL);
            operationList.addOperation(operation);
        } else {
            auto source = Address(operationProto.source());
            auto destination = Address(operationProto.transaction_operation_data().destination());
            auto operation = Transaction(source,
                                         operationProto.fee(),
                                         operationProto.counter(),
                                         operationProto.gas_limit(),
                                         operationProto.storage_limit(),
                                         operationProto.transaction_operation_data().amount(),
                                         destination,
                                         operationtype::TRANSACTION);
            operationList.addOperation(operation);
        }
    }

    auto signer = Signer();
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data signedBytes = signer.signOperationList(key, operationList);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signed_bytes(signedBytes.data(), signedBytes.size());
    auto serialized = protoOutput.SerializeAsString();

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
