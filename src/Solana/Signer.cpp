// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Program.h"
#include "../Base58.h"
#include <TrezorCrypto/ed25519.h>

#include <google/protobuf/util/json_util.h>

#include <algorithm>
#include <optional>

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

// Helper to convert protobuf-string-collection references to Address vector
std::vector<Address> convertReferences(const google::protobuf::RepeatedPtrField<std::string>& references) {
    std::vector<Address> ret;
    for (auto i = 0; i < references.size(); ++i) {
        if (Address::isValid(references[i])) {
            ret.emplace_back(references[i]);
        }
    }
    return ret;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto blockhash = Solana::Hash(input.recent_blockhash());
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Message message;
    std::vector<PrivateKey> signerKeys;

    switch (input.transaction_type_case()) {
        case Proto::SigningInput::TransactionTypeCase::kTransferTransaction:
            {
                auto protoMessage = input.transfer_transaction();
                message = Message::createTransfer(
                    /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
                    /* to */ Address(protoMessage.recipient()),
                    /* value */ protoMessage.value(),
                    /* recent_blockhash */ blockhash,
                    /* memo */ protoMessage.memo(),
                    convertReferences(protoMessage.references()));
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kDelegateStakeTransaction:
            {
                auto protoMessage = input.delegate_stake_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto validatorAddress = Address(protoMessage.validator_pubkey());
                auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
                std::optional<Address> stakeAddress;
                if (protoMessage.stake_account().size() == 0) {
                    // no stake address specified, generate a new unique
                    stakeAddress = StakeProgram::addressFromRecentBlockhash(userAddress, blockhash, stakeProgramId);
                } else {
                    // stake address specified, use it
                    stakeAddress = Address(protoMessage.stake_account());
                }
                message = Message::createStake(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress.value(),
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
                auto stakeAddress = Address(protoMessage.stake_account());
                message = Message::createStakeDeactivate(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress,
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kDeactivateAllStakeTransaction:
            {
                auto protoMessage = input.deactivate_all_stake_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                std::vector<Address> addresses;
                for (auto i = 0; i < protoMessage.stake_accounts_size(); ++i) {
                    addresses.emplace_back(Address(protoMessage.stake_accounts(i)));
                }
                message = Message::createStakeDeactivateAll(userAddress, addresses, blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kWithdrawTransaction:
            {
                auto protoMessage = input.withdraw_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                auto stakeAddress = Address(protoMessage.stake_account());
                message = Message::createStakeWithdraw(
                    /* signer */ userAddress,
                    /* stakeAddress */ stakeAddress,
                    /* value */ protoMessage.value(),
                    /* recent_blockhash */ blockhash);
                signerKeys.push_back(key);
            }
            break;

        case Proto::SigningInput::TransactionTypeCase::kWithdrawAllTransaction:
            {
                auto protoMessage = input.withdraw_all_transaction();
                auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
                std::vector<std::pair<Address, uint64_t>> stakes;
                for (auto i = 0; i < protoMessage.stake_accounts_size(); ++i) {
                    stakes.push_back(std::make_pair<Address, uint64_t>(
                        Address(protoMessage.stake_accounts(i).stake_account()),
                        protoMessage.stake_accounts(i).value()
                    ));
                }
                message = Message::createStakeWithdrawAll(userAddress, stakes, blockhash);
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
                message = Message::createTokenCreateAccount(userAddress, mainAddress, tokenMintAddress, tokenAddress, blockhash);
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
                const auto memo = protoMessage.memo();
                message = Message::createTokenTransfer(userAddress, tokenMintAddress, senderTokenAddress, recipientTokenAddress, amount, decimals, blockhash,
                    memo, convertReferences(protoMessage.references()));
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
                const auto memo = protoMessage.memo();
                message = Message::createTokenCreateAndTransfer(userAddress, recipientMainAddress, tokenMintAddress, recipientTokenAddress, senderTokenAddress, amount, decimals, blockhash,
                    memo, convertReferences(protoMessage.references()));
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

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    return Signer::sign(input).encoded();
}
