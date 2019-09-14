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
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
