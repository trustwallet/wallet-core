// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "BinaryCoding.h"
#include "Solana/Address.h"
#include "Solana/AccountMeta.h"
#include "Solana/Constants.h"

namespace TW::Solana {

// System instruction types
enum SystemInstruction {
    CreateAccount,
    Assign,
    Transfer,
    CreateAccountWithSeed,
    AdvanceNonceAccount,
    WithdrawNonceAccount,
    InitializeNonceAccount
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

// An instruction to execute a program
struct Instruction {
    // The address of the program account that executes this instruction
    Address programId;
    // List of accounts to pass to the program
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

    // create a system advance nonce account instruction to update nonce
    static Instruction advanceNonceAccount(const Address authorizer, const Address nonceAccount) {
        std::vector<AccountMeta> accountMetas = {
            AccountMeta(nonceAccount, false, false),
            AccountMeta(Address(SYSVAR_RECENT_BLOCKHASHS_ADDRESS), false, true),
            AccountMeta(authorizer, true, true),
        };
        const SystemInstruction type = AdvanceNonceAccount;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);

        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accountMetas, data);
    }

    // create a System initialize nonce instruction
    static Instruction createInitializeNonce(const std::vector<AccountMeta>& accounts,
                                             const Address authorizer) {
        const SystemInstruction type = InitializeNonceAccount;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        append(data, authorizer.vector());

        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accounts, data);
    }

    // create a system create account instruction
    static Instruction createAccount(const std::vector<AccountMeta>& accounts, uint64_t value,
                                     uint64_t space, const Address owner) {
        const SystemInstruction type = CreateAccount;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        encode64LE(static_cast<uint64_t>(space), data);
        append(data, owner.vector());

        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accounts, data);
    }

    static Instruction withdrawNonceAccount(const std::vector<AccountMeta>& accounts,
                                            uint64_t value) {
        const SystemInstruction type = WithdrawNonceAccount;
        auto data = Data();
        encode32LE(static_cast<uint32_t>(type), data);
        encode64LE(static_cast<uint64_t>(value), data);
        return Instruction(Address(SYSTEM_PROGRAM_ID_ADDRESS), accounts, data);
    }
};

}
