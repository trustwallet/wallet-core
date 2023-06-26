// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Program.h"
#include "Solana/Encoding.h"
#include "Solana/VersionedTransaction.h"

#include <google/protobuf/util/json_util.h>

#include <optional>

namespace TW::Solana {

void Signer::sign(const std::vector<PrivateKey>& privateKeys, VersionedTransaction& transaction) {
    for (auto privateKey : privateKeys) {
        auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
        auto index = transaction.getAccountIndex(address);
        auto message = transaction.messageData();
        transaction.signatures[index] = privateKey.sign(message, TWCurveED25519);
    }
}

// Helper to convert protobuf-string-collection references to Address vector
std::vector<Address>
convertReferences(const google::protobuf::RepeatedPtrField<std::string>& references) {
    std::vector<Address> ret;
    for (auto i = 0; i < references.size(); ++i) {
        if (Address::isValid(references[i])) {
            ret.emplace_back(references[i]);
        }
    }
    return ret;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto blockhash = Base58::decode(input.recent_blockhash());
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    LegacyMessage message;
    std::vector<PrivateKey> signerKeys;

    if (Address::isValid(input.fee_payer())) {
        auto feePayerKey = PrivateKey(Data(input.fee_payer_private_key().begin(), input.fee_payer_private_key().end()));
        signerKeys.push_back(feePayerKey);
    }

    switch (input.transaction_type_case()) {
    case Proto::SigningInput::TransactionTypeCase::kTransferTransaction: {
        auto protoMessage = input.transfer_transaction();
        message = LegacyMessage::createTransfer(
            /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* to */ Address(protoMessage.recipient()),
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash,
            /* memo */ protoMessage.memo(),
            convertReferences(protoMessage.references()));
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kDelegateStakeTransaction: {
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
        message = LegacyMessage::createStake(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress.value(),
            /* voteAddress */ validatorAddress,
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kDeactivateStakeTransaction: {
        auto protoMessage = input.deactivate_stake_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto stakeAddress = Address(protoMessage.stake_account());
        message = LegacyMessage::createStakeDeactivate(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress,
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kDeactivateAllStakeTransaction: {
        auto protoMessage = input.deactivate_all_stake_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        std::vector<Address> addresses;
        for (auto i = 0; i < protoMessage.stake_accounts_size(); ++i) {
            addresses.emplace_back(Address(protoMessage.stake_accounts(i)));
        }
        message = LegacyMessage::createStakeDeactivateAll(userAddress, addresses, blockhash);
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kWithdrawTransaction: {
        auto protoMessage = input.withdraw_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto stakeAddress = Address(protoMessage.stake_account());
        message = LegacyMessage::createStakeWithdraw(
            /* signer */ userAddress,
            /* stakeAddress */ stakeAddress,
            /* value */ protoMessage.value(),
            /* recent_blockhash */ blockhash);
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kWithdrawAllTransaction: {
        auto protoMessage = input.withdraw_all_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        std::vector<std::pair<Address, uint64_t>> stakes;
        for (auto i = 0; i < protoMessage.stake_accounts_size(); ++i) {
            stakes.push_back(std::make_pair<Address, uint64_t>(
                Address(protoMessage.stake_accounts(i).stake_account()),
                protoMessage.stake_accounts(i).value()));
        }
        message = LegacyMessage::createStakeWithdrawAll(userAddress, stakes, blockhash);
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kCreateTokenAccountTransaction: {
        auto protoMessage = input.create_token_account_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto mainAddress = Address(protoMessage.main_address());
        auto tokenMintAddress = Address(protoMessage.token_mint_address());
        auto tokenAddress = Address(protoMessage.token_address());
        message = LegacyMessage::createTokenCreateAccount(userAddress, mainAddress, tokenMintAddress,
                                                    tokenAddress, blockhash, input.nonce_account());
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kTokenTransferTransaction: {
        auto protoMessage = input.token_transfer_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto tokenMintAddress = Address(protoMessage.token_mint_address());
        auto senderTokenAddress = Address(protoMessage.sender_token_address());
        auto recipientTokenAddress = Address(protoMessage.recipient_token_address());
        auto amount = protoMessage.amount();
        auto decimals = static_cast<uint8_t>(protoMessage.decimals());
        const auto memo = protoMessage.memo();
        message = LegacyMessage::createTokenTransfer(userAddress, tokenMintAddress, senderTokenAddress,
                                               recipientTokenAddress, amount, decimals, blockhash,
                                               memo, convertReferences(protoMessage.references()),
                                               input.nonce_account(), input.fee_payer());
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kCreateAndTransferTokenTransaction: {
        auto protoMessage = input.create_and_transfer_token_transaction();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto recipientMainAddress = Address(protoMessage.recipient_main_address());
        auto tokenMintAddress = Address(protoMessage.token_mint_address());
        auto recipientTokenAddress = Address(protoMessage.recipient_token_address());
        auto senderTokenAddress = Address(protoMessage.sender_token_address());
        auto amount = protoMessage.amount();
        auto decimals = static_cast<uint8_t>(protoMessage.decimals());
        const auto memo = protoMessage.memo();
        message = LegacyMessage::createTokenCreateAndTransfer(
            userAddress, recipientMainAddress, tokenMintAddress, recipientTokenAddress,
            senderTokenAddress, amount, decimals, blockhash, memo,
            convertReferences(protoMessage.references()), input.nonce_account(), input.fee_payer());
        signerKeys.push_back(key);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
        auto createNonceAccountTransaction = input.create_nonce_account();
        auto nonceAccountKey =
            PrivateKey(Data(createNonceAccountTransaction.nonce_account_private_key().begin(),
                            createNonceAccountTransaction.nonce_account_private_key().end()));
        message = LegacyMessage::createNonceAccount(
            /* owner */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* new nonce_account */ Address(nonceAccountKey.getPublicKey(TWPublicKeyTypeED25519)),
            /* rent */ createNonceAccountTransaction.rent(),
            /* recent_blockhash */ blockhash,
            /* nonce_account */ input.nonce_account());
        signerKeys.push_back(key);
        signerKeys.push_back(nonceAccountKey);
    } break;

    case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount: {
        auto withdrawNonceAccountTransaction = input.withdraw_nonce_account();
        message = LegacyMessage::createWithdrawNonceAccount(
            /* owner */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
            /* sender */ Address(withdrawNonceAccountTransaction.nonce_account()),
            /* recipient */ Address(withdrawNonceAccountTransaction.recipient()),
            /* value */ withdrawNonceAccountTransaction.value(),
            /* recent_blockhash */ blockhash,
            /* nonce_account */ input.nonce_account());
        signerKeys.push_back(key);
    } break;
    case Proto::SigningInput::TransactionTypeCase::kAdvanceNonceAccount: {
        auto advanceNonceAccountTransaction = input.advance_nonce_account();
        auto userAddress = Address(key.getPublicKey(TWPublicKeyTypeED25519));
        auto nonceAccountAddress = Address(advanceNonceAccountTransaction.nonce_account());
        message = LegacyMessage::advanceNonceAccount(userAddress, nonceAccountAddress, blockhash);
        signerKeys.push_back(key);
    } break;

    default:
        assert(input.transaction_type_case() != Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET);
    }
    auto msg = VersionedMessage(message);
    if (input.v0_msg()) {
        msg = VersionedMessage(V0Message{.msg = message});
    }
    auto transaction = VersionedTransaction(msg);

    sign(signerKeys, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded);

    auto unsignedTx = Base58::encode(transaction.messageData());
    protoOutput.set_unsigned_tx(unsignedTx.data(), unsignedTx.size());

    return protoOutput;
}

void Signer::signUpdateBlockhash(const std::vector<PrivateKey>& privateKeys,
                                 VersionedTransaction& transaction, Data& recentBlockhash) {
    updateRecentHash(transaction.message, recentBlockhash);
    Signer::sign(privateKeys, transaction);
}

// This method does not confirm that PrivateKey order matches that encoded in the messageData
// That order must be correct for the Transaction to succeed on Solana
Data Signer::signRawMessage(const std::vector<PrivateKey>& privateKeys, const Data messageData) {
    std::vector<Data> signatures;
    for (auto &&privateKey : privateKeys) {
        signatures.emplace_back(privateKey.sign(messageData, TWCurveED25519));
    }
    Data buffer;
    append(buffer, shortVecLength<Data>(signatures));
    for (auto &&signature : signatures) {
        append(buffer, signature);
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

TW::Data Signer::preImageHash() const {
    TW::Data preImageHash;
    auto recentBlockhash = Base58::decode(input.recent_blockhash());
    switch (input.transaction_type_case()) {
    case Proto::SigningInput::TransactionTypeCase::kTransferTransaction: {
        auto transferTransaction = input.transfer_transaction();
        auto from = input.sender();
        auto message = LegacyMessage::createTransfer(
            /* from */ Address(from),
            /* to */ Address(transferTransaction.recipient()),
            /* value */ transferTransaction.value(),
            /* recent_blockhash */ recentBlockhash,
            /* memo */ transferTransaction.memo(),
            /* references */ convertReferences(transferTransaction.references()),
            /* nonce_account */ input.nonce_account());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
        auto createNonceAccountTransaction = input.create_nonce_account();
        auto from = input.sender();
        auto nonceAccount = createNonceAccountTransaction.nonce_account();
        auto message = LegacyMessage::createNonceAccount(
            /* owner */ Address(from),
            /* new nonce_account */ Address(createNonceAccountTransaction.nonce_account()),
            /* rent */ createNonceAccountTransaction.rent(),
            /* recent_blockhash */ recentBlockhash,
            /* nonce_account */ input.nonce_account());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount: {
        auto withdrawNonceAccountTransaction = input.withdraw_nonce_account();
        auto owner = input.sender();
        auto message = LegacyMessage::createWithdrawNonceAccount(
            /* owner */ Address(owner),
            /* sender */ Address(withdrawNonceAccountTransaction.nonce_account()),
            /* recipient */ Address(withdrawNonceAccountTransaction.recipient()),
            /* value */ withdrawNonceAccountTransaction.value(),
            /* recent_blockhash */ recentBlockhash,
            /* nonce_account */ input.nonce_account());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateTokenAccountTransaction: {
        auto createTokenAccontTransaction = input.create_token_account_transaction();
        auto userAddress = Address(input.sender());
        auto mainAddress = Address(createTokenAccontTransaction.main_address());
        auto tokenMintAddress = Address(createTokenAccontTransaction.token_mint_address());
        auto tokenAddress = Address(createTokenAccontTransaction.token_address());
        auto message =
            LegacyMessage::createTokenCreateAccount(userAddress, mainAddress, tokenMintAddress,
                                              tokenAddress, recentBlockhash, input.nonce_account());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kTokenTransferTransaction: {
        auto tokenTransferTransaction = input.token_transfer_transaction();
        auto userAddress = Address(input.sender());
        auto tokenMintAddress = Address(tokenTransferTransaction.token_mint_address());
        auto senderTokenAddress = Address(tokenTransferTransaction.sender_token_address());
        auto recipientTokenAddress = Address(tokenTransferTransaction.recipient_token_address());
        auto amount = tokenTransferTransaction.amount();
        auto decimals = static_cast<uint8_t>(tokenTransferTransaction.decimals());
        const auto memo = tokenTransferTransaction.memo();
        auto message = LegacyMessage::createTokenTransfer(
            userAddress, tokenMintAddress, senderTokenAddress, recipientTokenAddress, amount,
            decimals, recentBlockhash, memo,
            convertReferences(tokenTransferTransaction.references()), input.nonce_account(), input.fee_payer());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateAndTransferTokenTransaction: {
        auto createAndTransferTokenTransaction = input.create_and_transfer_token_transaction();
        auto userAddress = Address(input.sender());
        auto recipientMainAddress =
            Address(createAndTransferTokenTransaction.recipient_main_address());
        auto tokenMintAddress = Address(createAndTransferTokenTransaction.token_mint_address());
        auto recipientTokenAddress =
            Address(createAndTransferTokenTransaction.recipient_token_address());
        auto senderTokenAddress = Address(createAndTransferTokenTransaction.sender_token_address());
        auto amount = createAndTransferTokenTransaction.amount();
        auto decimals = static_cast<uint8_t>(createAndTransferTokenTransaction.decimals());
        const auto memo = createAndTransferTokenTransaction.memo();
        auto message = LegacyMessage::createTokenCreateAndTransfer(
            userAddress, recipientMainAddress, tokenMintAddress, recipientTokenAddress,
            senderTokenAddress, amount, decimals, recentBlockhash, memo,
            convertReferences(createAndTransferTokenTransaction.references()),
            input.nonce_account(), input.fee_payer());
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    case Proto::SigningInput::TransactionTypeCase::kAdvanceNonceAccount: {
        auto advanceNonceAccountTransaction = input.advance_nonce_account();
        auto userAddress = Address(input.sender());
        auto nonceAccountAddress = Address(advanceNonceAccountTransaction.nonce_account());
        auto message =
            LegacyMessage::advanceNonceAccount(userAddress, nonceAccountAddress, recentBlockhash);
        auto transaction = Transaction(message);
        preImageHash = transaction.messageData();
    } break;
    default:
        if (input.transaction_type_case() ==
            Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET) {
            throw std::invalid_argument("transaction type not set");
        }
    }
    return preImageHash;
};

std::vector<std::string> Signer::signers() const {
    std::vector<std::string> signers;
    if (Address::isValid(input.fee_payer())) {
        signers.push_back(input.fee_payer());
    }
    switch (input.transaction_type_case()) {
    case Proto::SigningInput::TransactionTypeCase::kTransferTransaction:
    case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount:
    case Proto::SigningInput::TransactionTypeCase::kCreateTokenAccountTransaction:
    case Proto::SigningInput::TransactionTypeCase::kTokenTransferTransaction:
    case Proto::SigningInput::TransactionTypeCase::kCreateAndTransferTokenTransaction:
    case Proto::SigningInput::TransactionTypeCase::kAdvanceNonceAccount: {
        auto sender = input.sender();
        signers.push_back(sender);
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
        auto from = input.sender();
        auto createNonceAccountTransaction = input.create_nonce_account();
        auto nonceAccount = createNonceAccountTransaction.nonce_account();
        signers.push_back(from);
        signers.push_back(nonceAccount);
    } break;
    default:
        if (input.transaction_type_case() ==
            Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET) {
            throw std::invalid_argument("transaction type not set");
        }
    }
    return signers;
};

Proto::SigningOutput Signer::compile(const std::vector<Data>& signatures,
                                     const std::vector<PublicKey>& publicKeys) const {
    auto output = Proto::SigningOutput();
    LegacyMessage message;
    auto recentBlockhash = Base58::decode(input.recent_blockhash());
    switch (input.transaction_type_case()) {
    case Proto::SigningInput::TransactionTypeCase::kTransferTransaction: {
        if (signatures.size() < 1) {
            throw std::invalid_argument("too few signatures");
        }
        auto transferTransaction = input.transfer_transaction();
        message = LegacyMessage::createTransfer(
            /* from */ Address(input.sender()),
            /* to */ Address(transferTransaction.recipient()),
            /* value */ transferTransaction.value(),
            /* recent_blockhash */ recentBlockhash,
            /* memo */ transferTransaction.memo(),
            /* references */ convertReferences(transferTransaction.references()),
            /* nonce_account */ input.nonce_account());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
        if (signatures.size() < 2) {
            throw std::invalid_argument("too few signatures");
        }
        auto createNonceAccountTransaction = input.create_nonce_account();
        auto from = input.sender();
        auto nonceAccount = createNonceAccountTransaction.nonce_account();
        message = LegacyMessage::createNonceAccount(
            /* owner */ Address(from),
            /* nonce_account */ Address(createNonceAccountTransaction.nonce_account()),
            /* rent */ createNonceAccountTransaction.rent(),
            /* recent_blockhash */ recentBlockhash,
            /* nonce_account */ input.nonce_account());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount: {
        if (signatures.size() < 1) {
            throw std::invalid_argument("too few signatures");
        }
        auto withdrawNonceAccountTransaction = input.withdraw_nonce_account();
        message = LegacyMessage::createWithdrawNonceAccount(
            /* owner */ Address(input.sender()),
            /* sender */ Address(withdrawNonceAccountTransaction.nonce_account()),
            /* recipient */ Address(withdrawNonceAccountTransaction.recipient()),
            /* value */ withdrawNonceAccountTransaction.value(),
            /* recent_blockhash */ recentBlockhash,
            /* nonce_account */ input.nonce_account());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateTokenAccountTransaction: {
        if (signatures.size() < 1) {
            throw std::invalid_argument("too few signatures");
        }
        auto createTokenAccontTransaction = input.create_token_account_transaction();
        auto userAddress = Address(input.sender());
        auto mainAddress = Address(createTokenAccontTransaction.main_address());
        auto tokenMintAddress = Address(createTokenAccontTransaction.token_mint_address());
        auto tokenAddress = Address(createTokenAccontTransaction.token_address());
        message =
            LegacyMessage::createTokenCreateAccount(userAddress, mainAddress, tokenMintAddress,
                                              tokenAddress, recentBlockhash, input.nonce_account());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kTokenTransferTransaction: {
        if (signatures.size() < 1) {
            throw std::invalid_argument("too few signatures");
        }
        auto tokenTransferTransaction = input.token_transfer_transaction();
        auto userAddress = Address(input.sender());
        auto tokenMintAddress = Address(tokenTransferTransaction.token_mint_address());
        auto senderTokenAddress = Address(tokenTransferTransaction.sender_token_address());
        auto recipientTokenAddress = Address(tokenTransferTransaction.recipient_token_address());
        auto amount = tokenTransferTransaction.amount();
        auto decimals = static_cast<uint8_t>(tokenTransferTransaction.decimals());
        const auto memo = tokenTransferTransaction.memo();
        message = LegacyMessage::createTokenTransfer(
            userAddress, tokenMintAddress, senderTokenAddress, recipientTokenAddress, amount,
            decimals, recentBlockhash, memo,
            convertReferences(tokenTransferTransaction.references()), input.nonce_account(), input.fee_payer());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kCreateAndTransferTokenTransaction: {
        auto createAndTransferTokenTransaction = input.create_and_transfer_token_transaction();
        auto userAddress = Address(input.sender());
        auto recipientMainAddress =
            Address(createAndTransferTokenTransaction.recipient_main_address());
        auto tokenMintAddress = Address(createAndTransferTokenTransaction.token_mint_address());
        auto recipientTokenAddress =
            Address(createAndTransferTokenTransaction.recipient_token_address());
        auto senderTokenAddress = Address(createAndTransferTokenTransaction.sender_token_address());
        auto amount = createAndTransferTokenTransaction.amount();
        auto decimals = static_cast<uint8_t>(createAndTransferTokenTransaction.decimals());
        const auto memo = createAndTransferTokenTransaction.memo();
        message = LegacyMessage::createTokenCreateAndTransfer(
            userAddress, recipientMainAddress, tokenMintAddress, recipientTokenAddress,
            senderTokenAddress, amount, decimals, recentBlockhash, memo,
            convertReferences(createAndTransferTokenTransaction.references()),
            input.nonce_account(), input.fee_payer());
    } break;
    case Proto::SigningInput::TransactionTypeCase::kAdvanceNonceAccount: {
        if (signatures.size() < 1) {
            throw std::invalid_argument("too few signatures");
        }
        auto advanceNonceAccountTransaction = input.advance_nonce_account();
        auto userAddress = Address(input.sender());
        auto nonceAccountAddress = Address(advanceNonceAccountTransaction.nonce_account());
        message = LegacyMessage::advanceNonceAccount(userAddress, nonceAccountAddress, recentBlockhash);
    } break;
    default:
        if (input.transaction_type_case() ==
            Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET) {
            throw std::invalid_argument("transaction type not set");
        }
    }
    auto transaction = Transaction(message);
    auto preImageHash = transaction.messageData();
    if (publicKeys.size() != signatures.size()) {
        throw std::invalid_argument(
            "the number of public keys and the number of signatures not aligned");
    }
    for (auto i = 0ul; i < signatures.size(); i++) {
        if (!publicKeys[i].verify(signatures[i], preImageHash)) {
            throw std::invalid_argument("invalid signature at " + std::to_string(i));
        }
        auto addressIdx = transaction.getAccountIndex(Address(publicKeys[i]));
        transaction.signatures[addressIdx] = signatures[i];
    }
    // construst the output
    auto encoded = transaction.serialize();
    output.set_encoded(encoded);
    return output;
};

} // namespace TW::Solana
