// Copyright © 2017-2020 Trust Wallet.
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
using namespace TW::Tezos::Proto;

TW_Tezos_Proto_SigningOutput TWTezosSignerSign(TW_Tezos_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto operationList = TW::Tezos::OperationList(input.operation_list().branch());
    for (TW::Tezos::Proto::Operation operation : input.operation_list().operations()) {
      operationList.addOperation(operation);
    }

    auto signer = Signer();
    PrivateKey key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data signedBytes = signer.signOperationList(key, operationList);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signed_bytes(signedBytes.data(), signedBytes.size());
    auto serialized = protoOutput.SerializeAsString();

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
