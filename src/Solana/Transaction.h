// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Base58.h"
#include "../BinaryCoding.h"

#include <vector>
#include <cassert>

namespace TW::Solana {

// https://docs.solana.com/developing/programming-model/transactions

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
enum SystemInstruction { CreateAccount, Assign, Transfer, CreateAccountWithSeed };

// Stake instruction types
enum StakeInstruction {
    Initialize = 0,
    DelegateStake = 2,
    Withdraw = 4,
    Deactivate = 5,
};

// Token instruction types
enum TokenIntruction {
    Token_InitializeAccount = 0,
    Token_Transfer
};

struct AccountMeta {
    /// An account's public key
    Address pubkey;
    /// True if an Instruction requires a Transaction signature matching `pubkey`.
    bool isSigner;
    /// True if the `pubkey` can be loaded as a read-write account.
    bool isWritable;

    AccountMeta(const Address& pubkey, bool isSigner, bool isWritable) : pubkey(pubkey), isSigner(isSigner), isWritable(isWritable) {}
};

// An instruction to execute a program
struct Instruction {
    // Index into the transaction keys array indicating the program account that
    // executes this instruction
    Address programId;
    // Ordered indices into the transaction keys array indicating which accounts
    // to pass to the program
    Data accounts;
    // The program input data
    Data data;

    Instruction(const Address& programId, const Data& accounts, const Data& data)
        : programId(programId), accounts(accounts), data(data) {}

    // This constructor creates a default System Transfer instruction
    Instruction(Data accountIndexes, uint64_t value)
        : programId(Address(SYSTEM_ADDRESS)) {
        this->accounts = accountIndexes;
        SystemInstruction type = Transfer;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        this->data = data;
    }

    // This constructor creates a System CreateAccountWithSeed instruction
    Instruction(uint64_t value, uint64_t space, Address programId,
                        Address voteAddress, uint64_t seedLength, Address signer)
        : programId(Address(SYSTEM_ADDRESS)) {
        std::vector<uint8_t> accounts = {0, 1};
        this->accounts = accounts;
        SystemInstruction type = CreateAccountWithSeed;
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
        this->data = data;
    }

    // This constructor creates an Initialize Stake instruction
    Instruction(StakeInstruction type, Address signer)
        : programId(Address(STAKE_ADDRESS)) {
        std::vector<uint8_t> accounts = {1, 2};
        this->accounts = accounts;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        append(data, signer.vector());
        append(data, signer.vector());
        auto lockup = Data(48);
        append(data, lockup);
        this->data = data;
    }

    // This constructor creates a Withdraw Stake instruction
    Instruction(StakeInstruction type, uint64_t value)
        : programId(Address(STAKE_ADDRESS)) {
        std::vector<uint8_t> accounts = {1, 0, 2, 3};
        this->accounts = accounts;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        this->data = data;
    }

    // This constructor creates a Stake instruction
    Instruction(StakeInstruction type)
        : programId(Address(STAKE_ADDRESS)) {
        std::vector<uint8_t> accounts;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        if (type == DelegateStake) {
            accounts = {1, 3, 4, 5, 0};
        } else if (type == Deactivate) {
            accounts = {1, 2, 0};
        }
        this->accounts = accounts;
        this->data = data;
    }

    // This constructor creates a Token instruction
    Instruction(TokenIntruction type)
        : programId(Address(STAKE_ADDRESS)) {
        std::vector<uint8_t> accounts;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        // accounts = ? TODO
        this->accounts = accounts;
        this->data = data;
    }
};

// A compiled instruction
struct CompiledInstruction {
    // Index into the transaction keys array indicating the program account that executes this instruction
    uint8_t programIdIndex;
    // Ordered indices into the transaction keys array indicating which accounts
    // to pass to the program
    Data accounts;
    // The program input data
    Data data;

    // Reference to the address vector
    const std::vector<Address>& addresses;

    CompiledInstruction(const Instruction& instruction, const std::vector<Address>& addresses): addresses(addresses) {
        programIdIndex = findAccount(instruction.programId);
        accounts = instruction.accounts;
        data = instruction.data;
    }

    uint8_t findAccount(const Address& address) {
        auto it = std::find(addresses.begin(), addresses.end(), address);
        assert(it != addresses.end());
        if (it == addresses.end()) {
            throw new std::invalid_argument("address not found");
        }
        auto dist = std::distance(addresses.begin(), it);
        assert(dist < 256);
        return (uint8_t)dist;
    }
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
    uint8_t numRequiredSignatures;
    // The last numCreditOnlySignedAccounts of the signed keys are
    // credit-only accounts.
    uint8_t numCreditOnlySignedAccounts;
    // The last numCreditOnlyUnsignedAccounts of the unsigned keys are
    // credit-only accounts.
    uint8_t numCreditOnlyUnsignedAccounts;
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

    Message() : recentBlockhash(Hash("11111111111111111111111111111111")){};

    Message(MessageHeader header, std::vector<Address> accountKeys, Hash recentBlockhash,
            std::vector<Instruction> instructions)
        : header(header)
        , accountKeys(accountKeys)
        , recentBlockhash(recentBlockhash)
        , instructions(instructions) {}

    // This constructor creates a default single-signer Transfer message
    Message(Address from, Address to, uint64_t value, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 1};
        this->header = header;
        auto programId = Address(SYSTEM_ADDRESS);
        std::vector<Address> accountKeys;
        Data accountIndexes;
        uint8_t programIdIndex;
        if (from.vector() != to.vector()) {
            accountKeys = {from, to, programId};
            accountIndexes = {0, 1};
            programIdIndex = 2;
        } else {
            accountKeys = {from, programId};
            accountIndexes = {0, 0};
            programIdIndex = 1;
        }
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(accountIndexes, value);
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a create_account_with_seed_and_delegate_stake message
    Message(Address signer, Address stakeAddress, Address voteAddress, uint64_t value,
            Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 6};
        this->header = header;

        auto sysvarRentId = Address("SysvarRent111111111111111111111111111111111");
        auto sysvarClockId = Address("SysvarC1ock11111111111111111111111111111111");
        auto stakeConfigId = Address("StakeConfig11111111111111111111111111111111");
        auto systemProgramId = Address(SYSTEM_ADDRESS);
        auto stakeProgramId = Address(STAKE_ADDRESS);
        std::vector<Address> accountKeys = {signer,          stakeAddress,  sysvarRentId,
                                            voteAddress,     sysvarClockId, stakeConfigId,
                                            systemProgramId, stakeProgramId};
        this->accountKeys = accountKeys;

        std::vector<Instruction> instructions;
        // create_account_with_seed instruction
        auto createAccountInstruction =
            Instruction(value, 2008, stakeProgramId, voteAddress, 32, signer);
        instructions.push_back(createAccountInstruction);
        // initialize instruction
        auto initializeInstruction = Instruction(Initialize, signer);
        instructions.push_back(initializeInstruction);
        // delegate_stake instruction
        auto delegateInstruction = Instruction(DelegateStake);
        instructions.push_back(delegateInstruction);

        this->instructions = instructions;
    }

    // This constructor creates a deactivate_stake message
    Message(Address signer, Address stakeAddress, StakeInstruction type, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 2};
        this->header = header;

        auto sysvarClockId = Address("SysvarC1ock11111111111111111111111111111111");
        auto programId = Address(STAKE_ADDRESS);
        std::vector<Address> accountKeys = {signer, stakeAddress, sysvarClockId, programId};
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(Deactivate);
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a withdraw message, with the signer as the default recipient
    Message(Address signer, Address stakeAddress, uint64_t value, StakeInstruction type,
            Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 3};
        this->header = header;

        auto sysvarClockId = Address("SysvarC1ock11111111111111111111111111111111");
        auto sysvarStakeHistoryId = Address("SysvarStakeHistory1111111111111111111111111");
        auto programId = Address(STAKE_ADDRESS);
        std::vector<Address> accountKeys = {signer, stakeAddress, sysvarClockId,
                                            sysvarStakeHistoryId, programId};
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(Withdraw, value);
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a create_account message
    Message(Address signer, Address token, Address account, StakeInstruction type, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 99}; // TODO
        this->header = header;

        //auto sysvarRentId = Address("SysvarRent111111111111111111111111111111111");
        //auto sysvarClockId = Address("SysvarC1ock11111111111111111111111111111111");
        //auto stakeConfigId = Address("StakeConfig11111111111111111111111111111111");
        auto systemProgramId = Address(SYSTEM_ADDRESS);
        auto stakeProgramId = Address(STAKE_ADDRESS);
        //std::vector<Address> accountKeys = {signer,          stakeAddress,  sysvarRentId,
        //                                    voteAddress,     sysvarClockId, stakeConfigId,
        //                                    systemProgramId, stakeProgramId};
        std::vector<Address> accountKeys = {signer,          token,         account,
                                            systemProgramId, stakeProgramId};
        this->accountKeys = accountKeys;

        std::vector<Instruction> instructions;
        // create_account instruction
        //auto createAccountInstruction =
        //    Instruction(99, Token_InitializeAccount);
        //instructions.push_back(createAccountInstruction);
        // initialize instruction
        auto initializeInstruction = Instruction(Token_InitializeAccount);
        instructions.push_back(initializeInstruction);

        this->instructions = instructions;
    }
};

class Transaction {
  public:
    // Signatures
    std::vector<Signature> signatures;
    // The message to sign
    Message message;

    Transaction(Message message) : message(message) {
        this->signatures.resize(message.header.numRequiredSignatures, Signature(defaultSignature));
    }

    // Default basic transfer transaction
    Transaction(Address from, Address to, uint64_t value, Hash recentBlockhash)
        : message(Message(from, to, value, recentBlockhash)) {
        this->signatures.resize(1, Signature(defaultSignature));
    }

  public:
    std::string serialize() const;
    std::vector<uint8_t> messageData() const;
    uint8_t getAccountIndex(Address publicKey);

  private:
    std::array<uint8_t, 64> defaultSignature;
};

} // namespace TW::Solana

/// Wrapper for C interface.
struct TWSolanaTransaction {
    TW::Solana::Transaction impl;
};
