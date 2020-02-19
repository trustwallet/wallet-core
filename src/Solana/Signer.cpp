// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include <TrezorCrypto/ed25519.h>

#include <algorithm>

using namespace TW;
using namespace TW::Solana;

void Signer::sign(const std::vector<PrivateKey>& privateKeys, Transaction& transaction) {
    for (auto privateKey : privateKeys) {
        auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
        auto index = transaction.getAccountIndex(address);
        auto message = transaction.messageData();
        auto signature = Signature(privateKey.sign(message, TWCurveED25519));
        transaction.signatures[index] = signature;
    }
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto blockhash = Solana::Hash(input.recent_blockhash());
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Message message;
    std::string stakePubkey;
    std::vector<PrivateKey> signerKeys;

    if (input.has_transfer_transaction()) {
        auto protoMessage = input.transfer_transaction();
        message = Message(
            /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_stake_transaction()) {
        auto protoMessage = input.stake_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto validatorAddress = Address(protoMessage.validator_pubkey());
        auto stakeProgramId = Address(STAKE_ADDRESS);
        auto stakeAddress = addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
        message = Message(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress,
            /* voteAddress */ validatorAddress,
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_deactivate_stake_transaction()) {
        auto protoMessage = input.deactivate_stake_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto validatorAddress = Address(protoMessage.validator_pubkey());
        auto stakeProgramId = Address(STAKE_ADDRESS);
        auto stakeAddress = addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
        message = Message(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress,
            /* type */ Deactivate,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } else if (input.has_withdraw_transaction()) {
        auto protoMessage = input.withdraw_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto validatorAddress = Address(protoMessage.validator_pubkey());
        auto stakeProgramId = Address(STAKE_ADDRESS);
        auto stakeAddress = addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
        message = Message(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress,
            /* value */ protoMessage.value(),
            /* type */ Withdraw,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    }
    auto transaction = Transaction(message);

    sign(signerKeys, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    return protoOutput;
}

void Signer::signUpdateBlockhash(const std::vector<PrivateKey>& privateKeys,
                                 Transaction& transaction, Solana::Hash& recentBlockhash) {
    transaction.message.recentBlockhash = recentBlockhash;
    Signer::sign(privateKeys, transaction);
}

// This method does not confirm that PrivateKey order matches that encoded in the messageData
// That order must be correct for the Transaction to succeed on Solana
Data Signer::signRawMessage(const std::vector<PrivateKey>& privateKeys, const Data messageData) {
    std::vector<Signature> signatures;
    for (auto privateKey : privateKeys) {
        auto signature = Signature(privateKey.sign(messageData, TWCurveED25519));
        signatures.push_back(signature);
    }
    Data buffer;
    append(buffer, shortVecLength<Signature>(signatures));
    for (auto signature : signatures) {
        Data signature_vec(signature.bytes.begin(), signature.bytes.end());
        append(buffer, signature_vec);
    }
    append(buffer, messageData);

    return buffer;
}
