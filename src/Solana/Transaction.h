// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../Data.h"

#include <vector>
#include <string>

namespace TW::Solana {

// https://docs.solana.com/developing/programming-model/transactions

const std::string SYSTEM_PROGRAM_ID_ADDRESS = "11111111111111111111111111111111";
const std::string STAKE_PROGRAM_ID_ADDRESS = "Stake11111111111111111111111111111111111111";
const std::string TOKEN_PROGRAM_ID_ADDRESS = "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA";
const std::string ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS = "ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL";
const std::string SYSVAR_RENT_ID_ADDRESS = "SysvarRent111111111111111111111111111111111";
const std::string SYSVAR_CLOCK_ID_ADDRESS = "SysvarC1ock11111111111111111111111111111111";
const std::string STAKE_CONFIG_ID_ADDRESS = "StakeConfig11111111111111111111111111111111";
const std::string NULL_ID_ADDRESS = "11111111111111111111111111111111";
const std::string SYSVAR_STAKE_HISTORY_ID_ADDRESS = "SysvarStakeHistory1111111111111111111111111";
const std::string MEMO_PROGRAM_ID_ADDRESS = "MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr";

template <typename T>
Data shortVecLength(std::vector<T> vec) {
    auto bytes = Data();
    auto remLen = vec.size();
    while (true) {
        uint8_t elem = remLen & 0x7f;
        remLen >>= 7;
        if (remLen == 0) {
            bytes.push_back(elem);
            break;
        } else {
            elem |= 0x80;
            bytes.push_back(elem);
        }
    }
    return bytes;
}

// System instruction types
enum SystemInstruction {
    CreateAccount,
    Assign,
    Transfer,
    CreateAccountWithSeed
};

// Stake instruction types
enum StakeInstruction {
    Initialize = 0,
    DelegateStake = 2,
    Withdraw = 4,
    Deactivate = 5,
};

// Token instruction types
enum TokenInstruction {
    CreateTokenAccount = 1,
    //SetAuthority = 6,
    TokenTransfer = 12,
};

enum TokenAuthorityType {
    MintTokens = 0,
    FreezeAccount = 1,
    AccountOwner = 2,
    CloseAccount = 3,
};

struct AccountMeta {
    Address account;
    bool isSigner;
    bool isReadOnly;
    AccountMeta(const Address& address, bool isSigner, bool isReadOnly): account(address), isSigner(isSigner), isReadOnly(isReadOnly) {}
};

// An instruction to execute a program
struct Instruction {
    // Index into the transaction keys array indicating the program account that
    // executes this instruction
    Address programId;
    // Ordered indices into the transaction keys array indicating which accounts
    // to pass to the program
    std::vector<AccountMeta> accounts;
    // The program input data
    Data data;

    Instruction(const Address& programId, const std::vector<AccountMeta>& accounts, const Data& data)
        : programId(programId), accounts(accounts), data(data) {}

    // This creator creates a default System Transfer instruction
    static Instruction createTransfer(const std::vector<AccountMeta>& accounts, uint64_t value) {
        const SystemInstruction type = Transfer;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);

        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accounts, data);
    }

    static Instruction createAccountWithSeed(const std::vector<AccountMeta>& accounts, uint64_t value, uint64_t space, const Address& programId,
        const Address& voteAddress, uint64_t seedLength, const Address& signer) {
        const SystemInstruction type = CreateAccountWithSeed;
        auto data = Data();
        std::string seed = voteAddress.string();
        Data vecSeed(seed.begin(), seed.end());
        vecSeed.resize(static_cast<size_t>(seedLength));
        encode32LE(static_cast<uint32_t>(type), data);
        append(data, signer.vector());
        encode64LE(static_cast<uint64_t>(seedLength), data);
        append(data, vecSeed);
        encode64LE(static_cast<uint64_t>(value), data);
        encode64LE(static_cast<uint64_t>(space), data);
        append(data, programId.vector());

        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // creates an Initialize Stake instruction
    static Instruction createStakeInitialize(const std::vector<AccountMeta>& accounts, const Address& signer) {
        const StakeInstruction type = Initialize;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        append(data, signer.vector());
        append(data, signer.vector());
        auto lockup = Data(48);
        append(data, lockup);

        return Instruction(Address(STAKE_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // creates a Withdraw Stake instruction
    static Instruction createStakeWithdraw(const std::vector<AccountMeta>& accounts, uint64_t value) {
        const StakeInstruction type = Withdraw;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);

        return Instruction(Address(STAKE_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // creates a Stake instruction
    static Instruction createStake(StakeInstruction type, const std::vector<AccountMeta>& accounts) {
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);

        return Instruction(Address(STAKE_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // creates a createAccount token instruction.
    static Instruction createTokenCreateAccount(const std::vector<AccountMeta>& accounts) {
        auto data = Data();
        return Instruction(Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // creates a transfer token instruction.
    static Instruction createTokenTransfer(const std::vector<AccountMeta>& accounts, uint64_t value, uint8_t decimals) {
        const TokenInstruction type = TokenTransfer;
        auto data = Data();
        data.push_back(static_cast<uint8_t>(type));
        encode64LE(value, data);
        data.push_back(static_cast<uint8_t>(decimals));

        return Instruction(Address(TOKEN_PROGRAM_ID_ADDRESS), accounts, data);
    }

    static Instruction createMemo(std::string memo) {
        auto data = TW::data(memo);
        std::vector<AccountMeta> accounts; // empty
        return Instruction(Address(MEMO_PROGRAM_ID_ADDRESS), accounts, data);
    }
};

// A compiled instruction
struct CompiledInstruction {
    // Index into the transaction keys array indicating the program account that executes this instruction
    uint8_t programIdIndex;
    // Ordered indices into the transaction keys array indicating which accounts
    // to pass to the program
    std::vector<uint8_t> accounts;
    // The program input data
    Data data;

    // Reference to the address vector
    const std::vector<Address>& addresses;

    /// Supplied address vector is expected to contain all addresses and programId from the instruction; they are replaced by index into the address vector.
    CompiledInstruction(const Instruction& instruction, const std::vector<Address>& addresses): addresses(addresses) {
        programIdIndex = findAccount(instruction.programId);
        for (auto& account: instruction.accounts) {
            accounts.push_back(findAccount(account.account));
        }
        data = instruction.data;
    }

    uint8_t findAccount(const Address& address);
};

class Hash {
  public:
    static const size_t size = 32;
    /// Hash data
    std::array<uint8_t, size> bytes;

    Hash(const std::string& string) {
        const auto data = Base58::bitcoin.decode(string);
        std::copy(data.begin(), data.end(), this->bytes.begin());
    }

    std::string encoded() const { return Base58::bitcoin.encode(bytes); }
};

class Signature {
  public:
    static const size_t size = 64;
    /// Signature data
    std::array<uint8_t, size> bytes;

    Signature(const std::string& string) {
        const auto data = Base58::bitcoin.decode(string);
        std::copy(data.begin(), data.end(), this->bytes.begin());
    }
    Signature(const std::array<uint8_t, 64>& bytes) { this->bytes = bytes; }
    Signature(const Data& bytes) { std::copy(bytes.begin(), bytes.end(), this->bytes.begin()); }

    bool operator==(const Signature& v) const;
};

struct MessageHeader {
    // The number of signatures required for this message to be considered
    // valid. The signatures must match the first `numRequiredSignatures` of
    // `accountKeys`.
    uint8_t numRequiredSignatures = 0;
    // The last numCreditOnlySignedAccounts of the signed keys are
    // credit-only accounts.
    uint8_t numCreditOnlySignedAccounts = 0;
    // The last numCreditOnlyUnsignedAccounts of the unsigned keys are
    // credit-only accounts.
    uint8_t numCreditOnlyUnsignedAccounts = 0;
};

class Message {
  public:
    // The message header, identifying signed and credit-only `accountKeys`
    MessageHeader header;
    // All the account keys used by this transaction
    std::vector<Address> accountKeys;
    // The id of a recent ledger entry.
    Hash recentBlockhash;
    // Programs that will be executed in sequence and committed in one atomic
    // transaction if all succeed.
    std::vector<Instruction> instructions;

    // three buckets of different account types
    std::vector<Address> signedAccounts;
    std::vector<Address> unsignedAccounts;
    std::vector<Address> readOnlyAccounts;
    std::vector<CompiledInstruction> compiledInstructions;

    Message() : recentBlockhash(NULL_ID_ADDRESS) {};

    Message(Hash recentBlockhash, const std::vector<Instruction>& instructions)
        : recentBlockhash(recentBlockhash)
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

    static void appendReferences(std::vector<AccountMeta>& accountMetas, const std::vector<Address>& references) {
        for (auto &&reference: references) {
            accountMetas.emplace_back(reference, false, true);
        }
    }

    // This constructor creates a default single-signer Transfer message
    static Message createTransfer(const Address& from, const Address& to, uint64_t value, Hash recentBlockhash,
        std::string memo = "", std::vector<Address> references = {}
    ) {
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
        return Message(recentBlockhash, instructions);
    }

    // This constructor creates a create_account_with_seed_and_delegate_stake message
    // see delegate_stake() solana/programs/stake/src/stake_instruction.rs
    static Message createStake(const Address& signer, const Address& stakeAddress, const Address& voteAddress, uint64_t value, Hash recentBlockhash) {
        auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto stakeConfigId = Address(STAKE_CONFIG_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
        std::vector<Instruction> instructions;
        instructions.reserve(3);
        // create_account_with_seed instruction
        Address seed = Address(data(recentBlockhash.bytes.data(), recentBlockhash.bytes.size()));
        auto createAccountInstruction = Instruction::createAccountWithSeed(std::vector<AccountMeta>{
                AccountMeta(signer, true, true),
                AccountMeta(stakeAddress, false, false),
                AccountMeta(signer, true, true),
            }, value, 200, stakeProgramId, seed, 32, signer);
        instructions.push_back(createAccountInstruction);
        // initialize instruction
        auto initializeInstruction = Instruction::createStakeInitialize(std::vector<AccountMeta>{
            AccountMeta(stakeAddress, false, false),
            AccountMeta(sysvarRentId, false, true)
        }, signer);
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
        return Message(recentBlockhash, instructions);
    }

    // This constructor creates a deactivate_stake message
    static Message createStakeDeactivate(const Address& signer, const Address& stakeAddress, Hash recentBlockhash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto instruction = Instruction::createStake(Deactivate, std::vector<AccountMeta>{
            AccountMeta(stakeAddress, false, false),    // 0. `[WRITE]` Delegated stake account
            AccountMeta(sysvarClockId, false, true),    // 1. `[]` Clock sysvar
            AccountMeta(signer, true, false),           // 2. `[SIGNER]` Stake authority
        });
        return Message(recentBlockhash, {instruction});
    }

    // This constructor creates a deactivate_stake message with multiple stake accounts
    static Message createStakeDeactivateAll(const Address& signer, const std::vector<Address>& stakeAddresses, Hash recentBlockhash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        std::vector<Instruction> instructions;
        for(auto& address: stakeAddresses) {
            auto instruction = Instruction::createStake(Deactivate, std::vector<AccountMeta>{
                AccountMeta(address, false, false),         // 0. `[WRITE]` Delegated stake account
                AccountMeta(sysvarClockId, false, true),    // 1. `[]` Clock sysvar
                AccountMeta(signer, true, false),           // 2. `[SIGNER]` Stake authority
            });
            instructions.push_back(instruction);
        }
        return Message(recentBlockhash, instructions);
    }

    // This constructor creates a withdraw message, with the signer as the default recipient
    static Message createStakeWithdraw(const Address& signer, const Address& stakeAddress, uint64_t value, Hash recentBlockhash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        auto instruction = Instruction::createStakeWithdraw(std::vector<AccountMeta>{
            AccountMeta(stakeAddress, false, false),            // 0. `[WRITE]` Stake account from which to withdraw
            AccountMeta(signer, false, false),                  // 1. `[WRITE]` Recipient account
            AccountMeta(sysvarClockId, false, true),            // 2. `[]` Clock sysvar
            AccountMeta(sysvarStakeHistoryId, false, true),     // 3. `[]` Stake history sysvar that carries stake warmup/cooldown history
            AccountMeta(signer, true, false),                   // 4. `[SIGNER]` Withdraw authority
        }, value);
        return Message(recentBlockhash, {instruction});
    }

    // This constructor creates a withdraw message, with multiple stake accounts
    static Message createStakeWithdrawAll(const Address& signer, const std::vector<std::pair<Address, uint64_t>>& stakes, Hash recentBlockhash) {
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        std::vector<Instruction> instructions;
        for(auto& stake: stakes) {
            auto instruction = Instruction::createStakeWithdraw(std::vector<AccountMeta>{
                AccountMeta(stake.first, false, false),         // 0. `[WRITE]` Stake account from which to withdraw
                AccountMeta(signer, false, false),              // 1. `[WRITE]` Recipient account
                AccountMeta(sysvarClockId, false, true),        // 2. `[]` Clock sysvar
                AccountMeta(sysvarStakeHistoryId, false, true), // 3. `[]` Stake history sysvar that carries stake warmup/cooldown history
                AccountMeta(signer, true, false),               // 4. `[SIGNER]` Withdraw authority
            }, stake.second);
            instructions.push_back(instruction);
        }
        return Message(recentBlockhash, instructions);
    }

    // This constructor creates a createAccount token message
    // see create_associated_token_account() solana-program-library/associated-token-account/program/src/lib.rs
    static Message createTokenCreateAccount(const Address& signer, const Address& otherMainAccount, const Address& tokenMintAddress, const Address& tokenAddress, Hash recentBlockhash) {
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
        return Message(recentBlockhash, {instruction});
    }

    // This constructor creates a transfer token message.
    // see transfer_checked() solana-program-library/token/program/src/instruction.rs
    static Message createTokenTransfer(const Address& signer, const Address& tokenMintAddress,
        const Address& senderTokenAddress, const Address& recipientTokenAddress, uint64_t amount, uint8_t decimals, Hash recentBlockhash,
        std::string memo = "", std::vector<Address> references = {}
    ) {
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
        return Message(recentBlockhash, instructions);
    }

    // This constructor creates a createAndTransferToken message, combining createAccount and transfer.
    static Message createTokenCreateAndTransfer(const Address& signer, const Address& recipientMainAddress, const Address& tokenMintAddress,
        const Address& recipientTokenAddress, const Address& senderTokenAddress, uint64_t amount, uint8_t decimals, Hash recentBlockhash,
        std::string memo = "", std::vector<Address> references = {}
    ) {
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
        return Message(recentBlockhash, instructions);
    }
};

class Transaction {
  public:
    // Signatures
    std::vector<Signature> signatures;
    // The message to sign
    Message message;

    Transaction(const Message& message) : message(message) {
        this->signatures.resize(message.header.numRequiredSignatures, Signature(defaultSignature));
    }

    // Default basic transfer transaction
    Transaction(const Address& from, const Address& to, uint64_t value, Hash recentBlockhash, std::string memo = "", std::vector<Address> references = {})
        : message(Message::createTransfer(from, to, value, recentBlockhash, memo, references)) {
        this->signatures.resize(1, Signature(defaultSignature));
    }

  public:
    std::string serialize() const;
    std::vector<uint8_t> messageData() const;
    uint8_t getAccountIndex(Address publicKey);

  private:
    TW::Data defaultSignature = TW::Data(64);
};

} // namespace TW::Solana
