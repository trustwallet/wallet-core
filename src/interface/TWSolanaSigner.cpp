// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PrivateKey.h"
#include "../Solana/Address.h"
#include "../Solana/Signer.h"
#include "../proto/Solana.pb.h"

#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWSolanaSigner.h>
#include <Base64.h>

using namespace TW;
using namespace TW::Solana;

TW_Solana_Proto_SigningOutput TWSolanaSignerSign(TW_Solana_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto blockhash = Solana::Hash(input.recent_blockhash());
    Message message;
    std::vector<PrivateKey> signerKeys;

    if (input.has_transfer_transaction()) {
        auto protoMessage = input.transfer_transaction();
        auto key = PrivateKey(protoMessage.private_key());
        message = Message(
            /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_stake_transaction()) {
        auto protoMessage = input.stake_transaction();
        auto payerKey = PrivateKey(protoMessage.from_private_key());
        auto stakeKey = PrivateKey(protoMessage.stake_private_key());
        message = Message(
            /* from */ Address(payerKey.getPublicKey(TWPublicKeyTypeED25519)),
            /* stakeAccount */ Address(stakeKey.getPublicKey(TWPublicKeyTypeED25519)),
            /* voteAccount */ Address(protoMessage.vote_pubkey()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(payerKey);
        signerKeys.push_back(stakeKey);
    } else if (input.has_deactivate_stake_transaction()) {
        auto protoMessage = input.deactivate_stake_transaction();
        auto stakeKey = PrivateKey(protoMessage.private_key());
        message = Message(
            /* from */ Address(stakeKey.getPublicKey(TWPublicKeyTypeED25519)),
            /* voteAccount */ Address(protoMessage.vote_pubkey()),
            /* type */ Deactivate,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(stakeKey);
    } else if (input.has_withdraw_transaction()) {
        auto protoMessage = input.withdraw_transaction();
        auto stakeKey = PrivateKey(protoMessage.private_key());
        message = Message(
            /* from */ Address(stakeKey.getPublicKey(TWPublicKeyTypeED25519)),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* type */ Withdraw,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(stakeKey);
    }
    auto transaction = Transaction(message);

    auto signer = Signer();
    signer.sign(signerKeys, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

TWData* _Nonnull TWSolanaSignerMessage(TW_Solana_Proto_SigningInput data,
                                       TWData* _Nonnull publicKey) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data pk(TWDataBytes(publicKey), TWDataBytes(publicKey) + TWDataSize(publicKey));
    auto pkStr = std::string(pk.begin(), pk.end());

    auto blockhash = Solana::Hash(input.recent_blockhash());
    Message message;

    // we only support transfer transaction
    if (input.has_transfer_transaction()) {
        auto protoMessage = input.transfer_transaction();
        message = Message(
            /* from */ Address(pkStr),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
    }
    auto transaction = Transaction(message);
    return TWDataCreateWithBytes(transaction.messageData().data(),
                                 transaction.messageData().size());
}

TWString* _Nonnull TWSolanaSignerTransaction(TW_Solana_Proto_SigningInput data,
                                             TWData* _Nonnull publicKey,
                                             TWData* _Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));
    auto encodedSignature = std::string(sig.begin(), sig.end());
    Data pk(TWDataBytes(publicKey), TWDataBytes(publicKey) + TWDataSize(publicKey));
    auto pkStr = std::string(pk.begin(), pk.end());

    auto blockhash = Solana::Hash(input.recent_blockhash());
    Message message;
    Data decodedSignature;

    // we only support transfer transaction
    if (input.has_transfer_transaction()) {
        auto protoMessage = input.transfer_transaction();
        message = Message(
            /* from */ Address(pkStr),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);

        decodedSignature = Base64::decode(encodedSignature);
    }

    auto transaction = Transaction(message);
    auto address = Address(pkStr);
    auto index = transaction.getAccountIndex(address);
    auto signatureBinary = Signature(decodedSignature);
    transaction.signatures[index] = signatureBinary;

    auto encodedTx = transaction.serialize();

    return TWStringCreateWithUTF8Bytes(Base64::encode(encodedTx).c_str());
}