// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Solana/MessageHeader.h"
#include "Solana/Address.h"
#include "Solana/Instruction.h"
#include "Solana/CompiledInstruction.h"
#include "Solana/Constants.h"

#include <vector>

namespace TW::Solana {

class LegacyMessage {
public:
    // The message header, identifying signed and credit-only `accountKeys`
    MessageHeader header;
    // All the account keys used by this transaction
    std::vector<Address> accountKeys;
    // The id of a recent ledger entry.
    Data mRecentBlockHash;
    // Programs that will be executed in sequence and committed in one atomic
    // transaction if all succeed.
    std::vector<Instruction> instructions;

    // three buckets of different account types
    std::vector<Address> signedAccounts;
    std::vector<Address> unsignedAccounts;
    std::vector<Address> readOnlyAccounts;
    std::vector<CompiledInstruction> compiledInstructions;

    LegacyMessage()
        : mRecentBlockHash(Base58::decode(NULL_ID_ADDRESS)){};

    LegacyMessage(Data recentBlockHash, const std::vector<Instruction>& instructions)
        : mRecentBlockHash(recentBlockHash)
        , instructions(instructions) {
        compileAccounts();
    }

    // add an acount, to the corresponding bucket
    void addAccount(const AccountMeta& account);
    // add an account to accountKeys if not yet present
    void addAccountKeys(const Address& account);
    // compile the single accounts lists from the buckets
    void compileAccounts();
    // compile the instructions; replace instruction accounts with indices
    void compileInstructions();

    // Serialize to msg data
    Data serialize() const;

    static void appendReferences(std::vector<AccountMeta>& accountMetas, const std::vector<Address>& references) {
        for (auto&& reference : references) {
            accountMetas.emplace_back(reference, false, true);
        }
    }

    // This constructor creates a default single-signer Transfer message
    static LegacyMessage createTransfer(const Address& from, const Address& to, uint64_t value, Data mRecentBlockHash,
                                  std::string memo = "", std::vector<Address> references = {}) {
        std::vector<Instruction> instructions;
        if (memo.length() > 0) {
            // Optional memo. Order: before transfer, as per documentation.
            instructions.push_back(Instruction::createMemo(memo));
        }
        std::vector<AccountMeta> accountMetas = {
            AccountMeta(from, true, false),
            AccountMeta(to, false, false),
        };
        appendReferences(accountMetas, references);
        instructions.push_back(Instruction::createTransfer(accountMetas, value));
        return LegacyMessage(mRecentBlockHash, instructions);
    }

    // This constructor creates a create_account_with_seed_and_delegate_stake message
    // see delegate_stake() solana/programs/stake/src/stake_instruction.rs
    static LegacyMessage createStake(const Address& signer, const Address& stakeAddress, const Address& voteAddress, uint64_t value, Data mRecentBlockHash) {
        auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto stakeConfigId = Address(STAKE_CONFIG_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
        std::vector<Instruction> instructions;
        instructions.reserve(3);
        // create_account_with_seed instruction
        Address seed = Address(mRecentBlockHash);
        auto createAccountInstruction = Instruction::createAccountWithSeed(std::vector<AccountMeta>{
                                                                               AccountMeta(signer, true, true),
                                                                               AccountMeta(stakeAddress, false, false),
                                                                               AccountMeta(signer, true, true),
                                                                           },
                                                                           value, 200, stakeProgramId, seed, 32, signer);
        instructions.push_back(createAccountInstruction);
        // initialize instruction
        auto initializeInstruction = Instruction::createStakeInitialize(std::vector<AccountMeta>{
                                                                            AccountMeta(stakeAddress, false, false),
                                                                            AccountMeta(sysvarRentId, false, true)},
                                                                        signer);
        instructions.push_back(initializeInstruction);
        // delegate_stake instruction
        auto delegateInstruction = Instruction::createStake(DelegateStake,
                                                            std::vector<AccountMeta>{
                                                                AccountMeta(stakeAddress, false, false),        // 0. `[WRITE]` Initialized stake account to be delegated
                                                                AccountMeta(voteAddress, false, true),          // 1. `[]` Vote account to which this stake will be delegated
                                                                AccountMeta(sysvarClockId, false, true),        // 2. `[]` Clock sysvar
                                                                AccountMeta(sysvarStakeHistoryId, false, true), // 3. `[]` Stake history sysvar that carries stake warmup/cooldown history
                                                                AccountMeta(stakeConfigId, false, true),        // 4. `[]` Address of config account that carries stake config
                                                                AccountMeta(signer, true, true),                // 5. `[SIGNER]` Stake authority
                                                            });
        instructions.push_back(delegateInstruction);
        return LegacyMessage(mRecentBlockHash, instructions);
    }

    // This constructor creates a deactivate_stake message
    static LegacyMessage createStakeDeactivate(const Address& signer, const Address& stakeAddress, Data mRecentBlockHash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto instruction = Instruction::createStake(Deactivate, std::vector<AccountMeta>{
                                                                    AccountMeta(stakeAddress, false, false), // 0. `[WRITE]` Delegated stake account
                                                                    AccountMeta(sysvarClockId, false, true), // 1. `[]` Clock sysvar
                                                                    AccountMeta(signer, true, false),        // 2. `[SIGNER]` Stake authority
                                                                });
        return LegacyMessage(mRecentBlockHash, {instruction});
    }

    // This constructor creates a deactivate_stake message with multiple stake accounts
    static LegacyMessage createStakeDeactivateAll(const Address& signer, const std::vector<Address>& stakeAddresses, Data mRecentBlockHash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        std::vector<Instruction> instructions;
        for (auto& address : stakeAddresses) {
            auto instruction = Instruction::createStake(Deactivate, std::vector<AccountMeta>{
                                                                        AccountMeta(address, false, false),      // 0. `[WRITE]` Delegated stake account
                                                                        AccountMeta(sysvarClockId, false, true), // 1. `[]` Clock sysvar
                                                                        AccountMeta(signer, true, false),        // 2. `[SIGNER]` Stake authority
                                                                    });
            instructions.push_back(instruction);
        }
        return LegacyMessage(mRecentBlockHash, instructions);
    }

    // This constructor creates a withdraw message, with the signer as the default recipient
    static LegacyMessage createStakeWithdraw(const Address& signer, const Address& stakeAddress, uint64_t value, Data mRecentBlockHash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        auto instruction = Instruction::createStakeWithdraw(std::vector<AccountMeta>{
                                                                AccountMeta(stakeAddress, false, false),        // 0. `[WRITE]` Stake account from which to withdraw
                                                                AccountMeta(signer, false, false),              // 1. `[WRITE]` Recipient account
                                                                AccountMeta(sysvarClockId, false, true),        // 2. `[]` Clock sysvar
                                                                AccountMeta(sysvarStakeHistoryId, false, true), // 3. `[]` Stake history sysvar that carries stake warmup/cooldown history
                                                                AccountMeta(signer, true, false),               // 4. `[SIGNER]` Withdraw authority
                                                            },
                                                            value);
        return LegacyMessage(mRecentBlockHash, {instruction});
    }

    // This constructor creates a withdraw message, with multiple stake accounts
    static LegacyMessage createStakeWithdrawAll(const Address& signer, const std::vector<std::pair<Address, uint64_t>>& stakes, Data mRecentBlockHash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        std::vector<Instruction> instructions;
        for (auto& stake : stakes) {
            auto instruction = Instruction::createStakeWithdraw(std::vector<AccountMeta>{
                                                                    AccountMeta(stake.first, false, false),         // 0. `[WRITE]` Stake account from which to withdraw
                                                                    AccountMeta(signer, false, false),              // 1. `[WRITE]` Recipient account
                                                                    AccountMeta(sysvarClockId, false, true),        // 2. `[]` Clock sysvar
                                                                    AccountMeta(sysvarStakeHistoryId, false, true), // 3. `[]` Stake history sysvar that carries stake warmup/cooldown history
                                                                    AccountMeta(signer, true, false),               // 4. `[SIGNER]` Withdraw authority
                                                                },
                                                                stake.second);
            instructions.push_back(instruction);
        }
        return LegacyMessage(mRecentBlockHash, instructions);
    }

    // This constructor creates a createAccount token message
    // see create_associated_token_account() solana-program-library/associated-token-account/program/src/lib.rs
    static LegacyMessage createTokenCreateAccount(const Address& signer, const Address& otherMainAccount, const Address& tokenMintAddress, const Address& tokenAddress, Data mRecentBlockHash) {
        auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
        auto systemProgramId = Address(SYSTEM_PROGRAM_ID_ADDRESS);
        auto tokenProgramId = Address(TOKEN_PROGRAM_ID_ADDRESS);
        auto instruction = Instruction::createTokenCreateAccount(std::vector<AccountMeta>{
            AccountMeta(signer, true, false), // fundingAddress,
            AccountMeta(tokenAddress, false, false),
            AccountMeta(otherMainAccount, false, true),
            AccountMeta(tokenMintAddress, false, true),
            AccountMeta(systemProgramId, false, true),
            AccountMeta(tokenProgramId, false, true),
            AccountMeta(sysvarRentId, false, true),
        });
        return LegacyMessage(mRecentBlockHash, {instruction});
    }

    // This constructor creates a transfer token message.
    // see transfer_checked() solana-program-library/token/program/src/instruction.rs
    static LegacyMessage createTokenTransfer(const Address& signer, const Address& tokenMintAddress,
                                       const Address& senderTokenAddress, const Address& recipientTokenAddress, uint64_t amount, uint8_t decimals, Data mRecentBlockHash,
                                       std::string memo = "", std::vector<Address> references = {}) {
        std::vector<Instruction> instructions;
        if (memo.length() > 0) {
            // Optional memo. Order: before transfer, as per documentation.
            instructions.push_back(Instruction::createMemo(memo));
        }
        std::vector<AccountMeta> accountMetas = {
            AccountMeta(senderTokenAddress, false, false),
            AccountMeta(tokenMintAddress, false, true),
            AccountMeta(recipientTokenAddress, false, false),
            AccountMeta(signer, true, false),
        };
        appendReferences(accountMetas, references);
        instructions.push_back(Instruction::createTokenTransfer(accountMetas, amount, decimals));
        return LegacyMessage(mRecentBlockHash, instructions);
    }

    // This constructor creates a createAndTransferToken message, combining createAccount and transfer.
    static LegacyMessage createTokenCreateAndTransfer(const Address& signer, const Address& recipientMainAddress, const Address& tokenMintAddress,
                                                const Address& recipientTokenAddress, const Address& senderTokenAddress, uint64_t amount, uint8_t decimals, Data mRecentBlockHash,
                                                std::string memo = "", std::vector<Address> references = {}) {
        const auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
        const auto systemProgramId = Address(SYSTEM_PROGRAM_ID_ADDRESS);
        const auto tokenProgramId = Address(TOKEN_PROGRAM_ID_ADDRESS);
        std::vector<Instruction> instructions;
        instructions.reserve(3);
        instructions.emplace_back(Instruction::createTokenCreateAccount(std::vector<AccountMeta>{
            AccountMeta(signer, true, false), // fundingAddress,
            AccountMeta(recipientTokenAddress, false, false),
            AccountMeta(recipientMainAddress, false, true),
            AccountMeta(tokenMintAddress, false, true),
            AccountMeta(systemProgramId, false, true),
            AccountMeta(tokenProgramId, false, true),
            AccountMeta(sysvarRentId, false, true),
        }));
        if (memo.length() > 0) {
            // Optional memo. Order: before transfer, as per documentation.
            instructions.emplace_back(Instruction::createMemo(memo));
        }
        std::vector<AccountMeta> accountMetas = {
            AccountMeta(senderTokenAddress, false, false),
            AccountMeta(tokenMintAddress, false, true),
            AccountMeta(recipientTokenAddress, false, false),
            AccountMeta(signer, true, false),
        };
        appendReferences(accountMetas, references);
        instructions.push_back(Instruction::createTokenTransfer(accountMetas, amount, decimals));
        return LegacyMessage(mRecentBlockHash, instructions);
    }
};

}
