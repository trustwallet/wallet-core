// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/LegacyMessage.h"

namespace TW::Solana {
void Message::addAccount(const AccountMeta& account) {
    bool inSigned = (std::find(signedAccounts.begin(), signedAccounts.end(), account.account) != signedAccounts.end());
    bool inUnsigned = (std::find(unsignedAccounts.begin(), unsignedAccounts.end(), account.account) != unsignedAccounts.end());
    bool inReadOnly = (std::find(readOnlyAccounts.begin(), readOnlyAccounts.end(), account.account) != readOnlyAccounts.end());
    if (account.isSigner) {
        if (!inSigned) {
            signedAccounts.push_back(account.account);
        }
    } else if (!account.isReadOnly) {
        if (!inSigned && !inUnsigned) {
            unsignedAccounts.push_back(account.account);
        }
    } else {
        if (!inSigned && !inUnsigned && !inReadOnly) {
            readOnlyAccounts.push_back(account.account);
        }
    }
}

void Message::addAccountKeys(const Address& account) {
    if (std::find(accountKeys.begin(), accountKeys.end(), account) == accountKeys.end()) {
        accountKeys.push_back(account);
    }
}

void Message::compileAccounts() {
    for (auto& instr : instructions) {
        for (auto& address : instr.accounts) {
            addAccount(address);
        }
    }
    // add programIds (read-only, at end)
    for (auto& instr : instructions) {
        addAccount(AccountMeta{instr.programId, false, true});
    }

    header = MessageHeader{
        (uint8_t)signedAccounts.size(),
        0,
        (uint8_t)readOnlyAccounts.size()};

    // merge the three buckets
    accountKeys.clear();
    for (auto& a : signedAccounts) {
        addAccountKeys(a);
    }
    for (auto& a : unsignedAccounts) {
        addAccountKeys(a);
    }
    for (auto& a : readOnlyAccounts) {
        addAccountKeys(a);
    }

    compileInstructions();
}

void Message::compileInstructions() {
    compiledInstructions.clear();
    for (auto instruction : instructions) {
        compiledInstructions.emplace_back(CompiledInstruction(instruction, accountKeys));
    }
}
}
