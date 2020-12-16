// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Program.h"
#include "../Base58.h"
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

    switch (input.transaction_type_case()) {
        case Proto::SigningInput::TransactionTypeCase::kTransferTransaction:
            {
                auto protoMessage = input.transfer_transaction();
                message = Message(
                    /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
                    /* to */ Address(protoMessage.recipient()),
                    /* value */ protoMessage.value(),
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kStakeTransaction:
            {
                auto protoMessage = input.stake_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto validatorAddress = Address(protoMessage.validator_pubkey());
                auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
                auto stakeAddress = StakeProgram::addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
                message = Message(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress,
                    /* voteAddress */ validatorAddress,
                    /* value */ protoMessage.value(),
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kDeactivateStakeTransaction:
            {
                auto protoMessage = input.deactivate_stake_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto validatorAddress = Address(protoMessage.validator_pubkey());
                auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
                auto stakeAddress = StakeProgram::addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
                message = Message(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress,
                    /* type */ Deactivate,
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kWithdrawTransaction:
            {
                auto protoMessage = input.withdraw_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto validatorAddress = Address(protoMessage.validator_pubkey());
                auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
                auto stakeAddress = StakeProgram::addressFromValidatorSeed(userAddress, validatorAddress, stakeProgramId);
                message = Message(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress,
                    /* value */ protoMessage.value(),
                    /* type */ Withdraw,
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kCreateTokenAccountTransaction:
            {
                auto protoMessage = input.create_token_account_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto mainAddress = Address(protoMessage.main_address());
                auto tokenMintAddress = Address(protoMessage.token_mint_address());
                auto tokenAddress = Address(protoMessage.token_address());
                message = Message(userAddress, TokenInstruction::CreateTokenAccount, mainAddress, tokenMintAddress, tokenAddress, blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kTokenTransferTransaction:
            {
                auto protoMessage = input.token_transfer_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto tokenMintAddress = Address(protoMessage.token_mint_address());
                auto senderTokenAddress = Address(protoMessage.sender_token_address());
                auto recipientTokenAddress = Address(protoMessage.recipient_token_address());
                auto amount = protoMessage.amount();
                auto decimals = static_cast<uint8_t>(protoMessage.decimals());
                message = Message(userAddress, TokenInstruction::TokenTransfer, tokenMintAddress, senderTokenAddress, recipientTokenAddress, amount, decimals, blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kCreateAndTransferTokenTransaction:
            {
                auto protoMessage = input.create_and_transfer_token_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto recipientMainAddress = Address(protoMessage.recipient_main_address());
                auto tokenMintAddress = Address(protoMessage.token_mint_address());
                auto recipientTokenAddress = Address(protoMessage.recipient_token_address());
                auto senderTokenAddress = Address(protoMessage.sender_token_address());
                auto amount = protoMessage.amount();
                auto decimals = static_cast<uint8_t>(protoMessage.decimals());
                message = Message(userAddress, recipientMainAddress, tokenMintAddress, recipientTokenAddress, senderTokenAddress, amount, decimals, blockhash);
                signerKeys.push_back(key);                
            }
            break;

        default:
            assert(input.transaction_type_case() != Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET);
    }
    auto transaction = Transaction(message);

    sign(signerKeys, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded);

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
