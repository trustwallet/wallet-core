// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PrivateKey.h"
#include "../Solana/Address.h"
#include "../Solana/Signer.h"
#include "../proto/Solana.pb.h"

#include <TrezorCrypto/rand.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWSolanaSigner.h>

using namespace TW;
using namespace TW::Solana;

Address generateRandomPubkey() {
    byte buf[32];
    random_buffer(buf, 32);
    auto data = Data();
    data.insert(data.begin(), buf, buf + 32);
    return Address(PublicKey(data, TWPublicKeyTypeED25519));
}

TW_Solana_Proto_SigningOutput TWSolanaSignerSign(TW_Solana_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto blockhash = Solana::Hash(input.recent_blockhash());
    Message message;
    std::string stakePubkey;
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
        auto key = PrivateKey(protoMessage.private_key());
        auto stakeAccount = protoMessage.stake_pubkey().length() > 0
                                ? Address(protoMessage.stake_pubkey())
                                : generateRandomPubkey();
        stakePubkey = stakeAccount.string();
        message = Message(
            /* signer */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* stakeAccount */ stakeAccount,
            /* voteAccount */ Address(protoMessage.vote_pubkey()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_deactivate_stake_transaction()) {
        auto protoMessage = input.deactivate_stake_transaction();
        auto key = PrivateKey(protoMessage.private_key());
        message = Message(
            /* signer */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* stakeAccount */ Address(protoMessage.stake_pubkey()),
            /* type */ Deactivate,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_withdraw_transaction()) {
        auto protoMessage = input.withdraw_transaction();
        auto key = PrivateKey(protoMessage.private_key());
        message = Message(
            /* signer */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* stakeAccount */ Address(protoMessage.stake_pubkey()),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* type */ Withdraw,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    }
    auto transaction = Transaction(message);

    auto signer = Signer();
    signer.sign(signerKeys, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    if (stakePubkey.length() > 0) {
      protoOutput.set_stake_pubkey(stakePubkey);
    }

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
