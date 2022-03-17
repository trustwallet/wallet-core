// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

using namespace TW::Solana;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte,
                            const char*) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte,
                            const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return Signer::signJSON(json, key);
}

TW::Data Entry::preImageHashes(TWCoinType coin, const TW::Data& txInputData) const {
    auto input = Proto::SigningInput();
    auto ret = Proto::PreSigningOutput();
    auto hashList = ret.mutable_hashpublickeys();
    try {
        if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
            switch (input.transaction_type_case()) {
            case Proto::SigningInput::TransactionTypeCase::kTransferTransaction: {
                auto transferTransaction = input.transfer_transaction();
                auto recentBlockhash = Hash(input.recent_blockhash());
                auto from = input.sender();
                auto message = Message::createTransfer(
                    /* from */ Address(from),
                    /* to */ Address(transferTransaction.recipient()),
                    /* value */ transferTransaction.value(),
                    /* recent_blockhash */ recentBlockhash,
                    /* memo */ transferTransaction.memo(),
                    /* references */ convertReferences(transferTransaction.references()),
                    /* nonce_account */ input.nonce_account());
                auto transaction = Transaction(message);
                auto preImageHash = transaction.messageData();
                auto publicKey = Data(from.begin(), from.end());
                auto hpl = hashList->Add();
                hpl->set_datahash(preImageHash.data(), preImageHash.size());
                hpl->set_publickeyhash(publicKey.data(), publicKey.size());
            } break;
            case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
                auto createNonceAccountTransaction = input.create_nonce_account();
                auto recentBlockhash = Hash(input.recent_blockhash());
                auto from = input.sender();
                auto nonceAccount = createNonceAccountTransaction.nonce_account();
                auto message = Message::createNonceAccount(
                    /* owner */ Address(from),
                    /* new nonce_account */ Address(createNonceAccountTransaction.nonce_account()),
                    /* rent */ createNonceAccountTransaction.rent(),
                    /* recent_blockhash */ recentBlockhash,
                    /* nonce_account */ input.nonce_account());
                auto transaction = Transaction(message);
                auto preImageHash = transaction.messageData();
                auto publicKey = Data(from.begin(), from.end());
                auto hpl = hashList->Add();
                hpl->set_datahash(preImageHash.data(), preImageHash.size());
                hpl->set_publickeyhash(publicKey.data(), publicKey.size());
            } break;
            case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount: {
                auto withdrawNonceAccountTransaction = input.withdraw_nonce_account();
                auto recentBlockhash = Hash(input.recent_blockhash());
                auto owner = input.sender();
                auto message = Message::createWithdrawNonceAccount(
                    /* owner */ Address(owner),
                    /* sender */ Address(withdrawNonceAccountTransaction.nonce_account()),
                    /* recipient */ Address(withdrawNonceAccountTransaction.recipient()),
                    /* value */ withdrawNonceAccountTransaction.value(),
                    /* recent_blockhash */ recentBlockhash,
                    /* nonce_account */ input.nonce_account());
                auto transaction = Transaction(message);
                auto preImageHash = transaction.messageData();
                auto publicKey = Data(owner.begin(), owner.end());
                auto hpl = hashList->Add();
                hpl->set_datahash(preImageHash.data(), preImageHash.size());
                hpl->set_publickeyhash(publicKey.data(), publicKey.size());
            } break;
            default:
                assert(input.transaction_type_case() !=
                       Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET);
            }
        }
    } catch (std::invalid_argument ia) {
        ret.clear_hashpublickeys();
        ret.set_error(ia.what());
        ret.set_errorcode(Proto::SigningError::Error_invalid_argument);
    } catch(std::exception e) {
        ret.clear_hashpublickeys();
        ret.set_error(e.what());
        ret.set_errorcode(Proto::SigningError::Error_internal);
    }
    auto serialized = ret.SerializeAsString();
    auto dataOut = Data(serialized.begin(), serialized.end());
    return dataOut;
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = Proto::SigningInput();
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        Message message;
        switch (input.transaction_type_case()) {
        case Proto::SigningInput::TransactionTypeCase::kTransferTransaction: {
            auto transferTransaction = input.transfer_transaction();
            auto recentBlockhash = Hash(input.recent_blockhash());
            message = Message::createTransfer(
                /* from */ Address(input.sender()),
                /* to */ Address(transferTransaction.recipient()),
                /* value */ transferTransaction.value(),
                /* recent_blockhash */ recentBlockhash,
                /* memo */ transferTransaction.memo(),
                /* references */ convertReferences(transferTransaction.references()),
                /* nonce_account */ input.nonce_account());
        } break;
        case Proto::SigningInput::TransactionTypeCase::kCreateNonceAccount: {
            auto createNonceAccountTransaction = input.create_nonce_account();
            auto recentBlockhash = Hash(input.recent_blockhash());
            auto from = input.sender();
            auto nonceAccount = createNonceAccountTransaction.nonce_account();
            message = Message::createNonceAccount(
                /* owner */ Address(from),
                /* nonce_account */ Address(createNonceAccountTransaction.nonce_account()),
                /* rent */ createNonceAccountTransaction.rent(),
                /* recent_blockhash */ recentBlockhash,
                /* nonce_account */ input.nonce_account());
        } break;
        case Proto::SigningInput::TransactionTypeCase::kWithdrawNonceAccount: {
            auto withdrawNonceAccountTransaction = input.withdraw_nonce_account();
            auto recentBlockhash = Hash(input.recent_blockhash());
            message = Message::createWithdrawNonceAccount(
                /* owner */ Address(input.sender()),
                /* sender */ Address(withdrawNonceAccountTransaction.nonce_account()),
                /* recipient */ Address(withdrawNonceAccountTransaction.recipient()),
                /* value */ withdrawNonceAccountTransaction.value(),
                /* recent_blockhash */ recentBlockhash,
                /* nonce_account */ input.nonce_account());
        } break;
        default:
            assert(input.transaction_type_case() !=
                   Proto::SigningInput::TransactionTypeCase::TRANSACTION_TYPE_NOT_SET);
        }
        auto transaction = Transaction(message);
        auto preImageHash = transaction.messageData();
        // verify the signatures
        if (publicKeys.size() != signatures.size()) {
            throw std::invalid_argument(
                "the number of public keys and the number of signatures not aligned");
        }
        for (int i = 0; i < signatures.size(); i++) {
            if (!publicKeys[i].verify(signatures[i], preImageHash)) {
                throw std::invalid_argument("invalid signature at " + std::to_string(i));
            }
            auto addressIdx = transaction.getAccountIndex(Address(publicKeys[i]));
            transaction.signatures[addressIdx] = Signature(signatures[i]);
        }

        // construst the output
        auto protoOutput = Proto::SigningOutput();
        auto encoded = transaction.serialize();
        protoOutput.set_encoded(encoded);
        auto serialized = protoOutput.SerializeAsString();
        dataOut = Data(serialized.begin(), serialized.end());
    }
    return;
}
