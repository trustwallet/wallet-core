// Copyright © 2017-2020 Trust Wallet.
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
#include <cassert>

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
    SetAuthority = 6,
    TokenTransfer = 12,
};

enum TokenAuthorityType {
    MintTokens = 0,
    FreezeAccount = 1,
    AccountOwner = 2,
    CloseAccount = 3,
};

// An instruction to execute a program
struct Instruction {
    // Index into the transaction keys array indicating the program account that
    // executes this instruction
    Address programId;
    // Ordered indices into the transaction keys array indicating which accounts
    // to pass to the program
    std::vector<Address> accounts;
    // The program input data
    Data data;

    Instruction(const Address& programId, const std::vector<Address>& accounts, const Data& data)
        : programId(programId), accounts(accounts), data(data) {}

    // This constructor creates a default System Transfer instruction
    Instruction(const std::vector<Address>& accounts, uint64_t value) :
        programId(Address(SYSTEM_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        SystemInstruction type = Transfer;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        this->data = data;
    }

    // This constructor creates a System CreateAccountWithSeed instruction
    Instruction(const std::vector<Address>& accounts, uint64_t value, uint64_t space, const Address& programId,
        const Address& voteAddress, uint64_t seedLength, const Address& signer) :
        programId(Address(SYSTEM_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
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
    Instruction(StakeInstruction type, const std::vector<Address>& accounts, const Address& signer) :
        programId(Address(STAKE_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        append(data, signer.vector());
        append(data, signer.vector());
        auto lockup = Data(48);
        append(data, lockup);
        this->data = data;
    }

    // This constructor creates a Withdraw Stake instruction
    Instruction(StakeInstruction type, const std::vector<Address>& accounts, uint64_t value) :
        programId(Address(STAKE_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        this->data = data;
    }

    // This constructor creates a Stake instruction
    Instruction(StakeInstruction type, const std::vector<Address>& accounts) :
        programId(Address(STAKE_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        this->data = data;
    }

    // This constructor creates a createAccount token instruction.
    Instruction(TokenInstruction type, const std::vector<Address>& accounts) :
        programId(Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        //encode32LE(static_cast<uint32_t>(type), data);
        this->data = data;
    }

    // This constructor creates a transfer token instruction.
    Instruction(TokenInstruction type, const std::vector<Address>& accounts, uint64_t value, uint8_t decimals) :
        programId(Address(TOKEN_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        data.push_back(static_cast<uint8_t>(type));
        encode64LE(value, data);
        data.push_back(static_cast<uint8_t>(decimals));
        this->data = data;
    }

    // This constructor creates a setAuthority token instruction.
    Instruction(TokenInstruction type, const std::vector<Address>& accounts, const Address& newAuthorityAddress) :
        programId(Address(TOKEN_PROGRAM_ID_ADDRESS)),
        accounts(accounts)
    {
        auto data = Data();
        data.push_back(static_cast<uint8_t>(type));
        data.push_back(static_cast<uint8_t>(TokenAuthorityType::AccountOwner));
        data.push_back(1);
        append(data, TW::data(newAuthorityAddress.bytes.data(), newAuthorityAddress.bytes.size()));
        this->data = data;
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
            accounts.push_back(findAccount(account));
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

    Message() : recentBlockhash(NULL_ID_ADDRESS) {};

    Message(MessageHeader header, std::vector<Address> accountKeys, Hash recentBlockhash,
            std::vector<Instruction> instructions)
        : header(header)
        , accountKeys(accountKeys)
        , recentBlockhash(recentBlockhash)
        , instructions(instructions) {}

    // This constructor creates a default single-signer Transfer message
    Message(const Address& from, const Address& to, uint64_t value, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 1};
        this->header = header;
        auto programId = Address(SYSTEM_PROGRAM_ID_ADDRESS);
        std::vector<Address> accountKeys;
        if (from.vector() != to.vector()) {
            accountKeys = {from, to, programId};
        } else {
            accountKeys = {from, programId};
        }
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(std::vector<Address>{from, to}, value);
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a create_account_with_seed_and_delegate_stake message
    Message(const Address& signer, const Address& stakeAddress, const Address& voteAddress, uint64_t value,
            Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 6};
        this->header = header;

        auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto stakeConfigId = Address(STAKE_CONFIG_ID_ADDRESS);
        auto systemProgramId = Address(SYSTEM_PROGRAM_ID_ADDRESS);
        auto stakeProgramId = Address(STAKE_PROGRAM_ID_ADDRESS);
        std::vector<Address> accountKeys = {signer,          stakeAddress,  sysvarRentId,
                                            voteAddress,     sysvarClockId, stakeConfigId,
                                            systemProgramId, stakeProgramId};
        this->accountKeys = accountKeys;

        std::vector<Instruction> instructions;
        // create_account_with_seed instruction
        auto createAccountInstruction =
            Instruction(std::vector<Address>{signer, stakeAddress}, value, 2008, stakeProgramId, voteAddress, 32, signer);
        instructions.push_back(createAccountInstruction);
        // initialize instruction
        auto initializeInstruction = Instruction(Initialize, std::vector<Address>{stakeAddress, sysvarRentId}, signer);
        instructions.push_back(initializeInstruction);
        // delegate_stake instruction
        auto delegateInstruction = Instruction(DelegateStake,
            std::vector<Address>{
                stakeAddress,
                voteAddress,
                sysvarClockId,
                stakeConfigId,
                signer
            });
        instructions.push_back(delegateInstruction);

        this->instructions = instructions;
    }

    // This constructor creates a deactivate_stake message
    Message(const Address& signer, const Address& stakeAddress, StakeInstruction type, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 2};
        this->header = header;

        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto programId = Address(STAKE_PROGRAM_ID_ADDRESS);
        std::vector<Address> accountKeys = {signer, stakeAddress, sysvarClockId, programId};
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(Deactivate, std::vector<Address>{stakeAddress, sysvarClockId, signer});
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a withdraw message, with the signer as the default recipient
    Message(const Address& signer, const Address& stakeAddress, uint64_t value, StakeInstruction type,
            Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        MessageHeader header = {1, 0, 3};
        this->header = header;

        auto sysvarClockId = Address(SYSVAR_CLOCK_ID_ADDRESS);
        auto sysvarStakeHistoryId = Address(SYSVAR_STAKE_HISTORY_ID_ADDRESS);
        auto programId = Address(STAKE_PROGRAM_ID_ADDRESS);
        std::vector<Address> accountKeys = {signer, stakeAddress, sysvarClockId,
                                            sysvarStakeHistoryId, programId};
        this->accountKeys = accountKeys;
        std::vector<Instruction> instructions;
        auto instruction = Instruction(Withdraw, std::vector<Address>{stakeAddress, signer, sysvarClockId, sysvarStakeHistoryId}, value);
        instructions.push_back(instruction);
        this->instructions = instructions;
    }

    // This constructor creates a token message:
    // * createAccount: parameters: tokenMintAddress, tokenAddress
    // * setAuthority: parameters: tokenAddress, newOwnerAddress
    // Assume that the mainAccount is the same as the signer
    Message(const Address& signer, TokenInstruction type, const Address& address1, const Address& address2, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        assert(type == TokenInstruction::CreateTokenAccount || TokenInstruction::SetAuthority);
        switch (type) {
            case TokenInstruction::CreateTokenAccount:
                {
                    MessageHeader header = {1, 0, 5};
                    this->header = header;

                    auto sysvarRentId = Address(SYSVAR_RENT_ID_ADDRESS);
                    auto systemProgramId = Address(SYSTEM_PROGRAM_ID_ADDRESS);
                    auto tokenProgramId = Address(TOKEN_PROGRAM_ID_ADDRESS);
                    auto associatedTokenProgramId = Address(ASSOCIATED_TOKEN_PROGRAM_ID_ADDRESS);

                    std::vector<Address> accountKeys = {
                        signer,
                        address2,
                        address1,
                        systemProgramId,
                        tokenProgramId,
                        sysvarRentId,
                        associatedTokenProgramId
                    };
                    this->accountKeys = accountKeys;

                    std::vector<Instruction> instructions;
                    // initialize instruction
                    auto initializeInstruction = Instruction(type, std::vector<Address>{
                        signer, // fundingAddress,
                        address2,
                        signer,
                        address1,
                        systemProgramId,
                        tokenProgramId,
                        sysvarRentId
                    });
                    instructions.push_back(initializeInstruction);
                    this->instructions = instructions;
                }
                break;

            case TokenInstruction::SetAuthority:
            default:
                {
                    assert(TokenInstruction::SetAuthority);
                    MessageHeader header = {1, 0, 1};
                    this->header = header;

                    auto tokenProgramId = Address(TOKEN_PROGRAM_ID_ADDRESS);

                    std::vector<Address> accountKeys = {
                        signer,
                        address1,
                        tokenProgramId
                    };
                    this->accountKeys = accountKeys;

                    std::vector<Instruction> instructions;
                    // initialize instruction
                    auto initializeInstruction = Instruction(type, std::vector<Address>{
                        address1,
                        signer,
                    }, address2);
                    instructions.push_back(initializeInstruction);
                    this->instructions = instructions;
                }
                break;
        }
    }

    // This constructor creates a transfer token message.
    Message(const Address& signer, TokenInstruction type, const Address& tokenMintAddress, 
        const Address& senderTokenAddress, const Address& recipientTokenAddress, uint64_t amount, uint8_t decimals, Hash recentBlockhash)
        : recentBlockhash(recentBlockhash) {
        assert(type == TokenInstruction::TokenTransfer);
        MessageHeader header = {1, 0, 2};
        this->header = header;

        auto tokenProgramId = Address(TOKEN_PROGRAM_ID_ADDRESS);

        std::vector<Address> accountKeys = {
            signer,
            senderTokenAddress,
            recipientTokenAddress,
            tokenMintAddress,
            tokenProgramId,
        };
        this->accountKeys = accountKeys;

        std::vector<Instruction> instructions;
        // initialize instruction
        auto initializeInstruction = Instruction(type, std::vector<Address>{
            senderTokenAddress,
            tokenMintAddress,
            recipientTokenAddress,
            signer
        }, amount, decimals);
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
    Transaction(const Address& from, const Address& to, uint64_t value, Hash recentBlockhash)
        : message(Message(from, to, value, recentBlockhash)) {
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

/// Wrapper for C interface.
struct TWSolanaTransaction {
    TW::Solana::Transaction impl;
};
