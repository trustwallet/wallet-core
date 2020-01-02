// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Aeternity/Signer.h"
#include "../Aeternity/Transaction.h"
#include "../proto/Aeternity.pb.h"
#include "../uint256.h"
#include <TrustWalletCore/TWAeternitySigner.h>

using namespace TW;
using namespace TW::Aeternity;

TW_Aeternity_Proto_SigningOutput TWAeternitySignerSign(TW_Aeternity_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    std::string sender_id = input.from_address();
    std::string recipient_id = input.to_address();
    std::string payload = input.payload();

    auto tx = Transaction(sender_id, recipient_id, load(input.amount()), load(input.fee()), payload, input.ttl(), input.nonce());

    auto output = Signer::sign(privateKey, tx);

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
