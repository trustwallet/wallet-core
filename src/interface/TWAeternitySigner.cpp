// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Aeternity/Signer.h"
#include "../Aeternity/Transaction.h"
#include "../proto/Aeternity.pb.h"
#include "../uint256.h"
#include <TrustWalletCore/TWAeternitySigner.h>
#include <TrustWalletCore/TWAeternityAddress.h>
#include <Aeternity/Identifiers.h>
#include <Base58.h>

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

TWData *_Nonnull TWAeternitySignerMessage(TW_Aeternity_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    std::string sender_id = input.from_address();
    std::string recipient_id = input.to_address();
    std::string payload = input.payload();

    auto tx = Transaction(sender_id, recipient_id, load(input.amount()), load(input.fee()), payload, input.ttl(), input.nonce());

    auto txRlp = tx.encode();
    auto msg = Signer::buildMessageToSign(txRlp);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(msg.data()), msg.size());
}

TWString *_Nonnull TWAeternitySignerTransaction(TW_Aeternity_Proto_SigningInput data, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));

    std::string sender_id = input.from_address();
    std::string recipient_id = input.to_address();
    std::string payload = input.payload();

    auto tx = Transaction(sender_id, recipient_id, load(input.amount()), load(input.fee()), payload, input.ttl(), input.nonce());

    auto txRlp = tx.encode();

    /// encode the message using rlp
    auto rlpTxRaw = Signer::buildRlpTxRaw(txRlp, sig);

    /// encode the rlp message with the prefix
    auto signedEncodedTx = Signer::encodeBase64WithChecksum(Identifiers::prefixTransaction, rlpTxRaw);
    return TWStringCreateWithUTF8Bytes(signedEncodedTx.c_str());
}