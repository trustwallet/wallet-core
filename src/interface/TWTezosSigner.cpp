// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTezosSigner.h>

#include "../Tezos/Signer.h"
#include "../proto/Tezos.pb.h"

using namespace TW;
using namespace TW::Tezos;

TW_Tezos_Proto_SigningOutput TWTezosSignerSign(TW_Tezos_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    auto operationList = OperationList(input.operation_list().branch());
//    for (TW::Tezos::Proto::Operation operationProto : input.operation_list().operations()) {
//        std::variant<Address, PublicKey> operationData;
//        if (operationProto.has_reveal_operation_data()) {
//            operationData = PublicKey(operationProto.reveal_operation_data().public_key())
//        }
//
//        auto transaction = Transaction(
//            operationProto.source(),
//            operationProto.fee(),
//            operationProto.counter(),
//            operationProto.counter(),
//            operationProto.gas_limit(),
//            operationProto.storage_limit(),
//            operationProto
//            operationProto.kind(),
//        )
//    }

    auto signer = Signer();
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto signedBytesHex = signer.signOperationList(key, operationList);

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(signedBytesHex.data()), signedBytesHex.size());
}
