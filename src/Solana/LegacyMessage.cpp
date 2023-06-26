// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Solana/LegacyMessage.h"
#include "Solana/Encoding.h"

namespace TW::Solana {
void LegacyMessage::addAccount(const AccountMeta& account) {
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

void LegacyMessage::addAccountKeys(const Address& account) {
    if (std::find(accountKeys.begin(), accountKeys.end(), account) == accountKeys.end()) {
        accountKeys.push_back(account);
    }
}

Data LegacyMessage::serialize() const {
    Data buffer;

    buffer.push_back(this->header.numRequiredSignatures);
    buffer.push_back(this->header.numReadOnlySignedAccounts);
    buffer.push_back(this->header.numReadOnlyUnsignedAccounts);
    append(buffer, shortVecLength<Address>(this->accountKeys));
    for (auto account_key : this->accountKeys) {
        Data account_key_vec(account_key.bytes.begin(), account_key.bytes.end());
        append(buffer, account_key_vec);
    }
    append(buffer, mRecentBlockHash);

    // apppend compiled instructions
    append(buffer, shortVecLength<CompiledInstruction>(compiledInstructions));
    for (auto instruction : compiledInstructions) {
        buffer.push_back(instruction.programIdIndex);
        append(buffer, shortVecLength<uint8_t>(instruction.accounts));
        append(buffer, instruction.accounts);
        append(buffer, shortVecLength<uint8_t>(instruction.data));
        append(buffer, instruction.data);
    }

    return buffer;
}

void LegacyMessage::compileAccounts(const std::string& feePayerStr) {
    if (Address::isValid(feePayerStr)) {
        addAccount(AccountMeta(Address(feePayerStr), true, false));
    }

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

void LegacyMessage::compileInstructions() {
    compiledInstructions.clear();
    for (auto instruction : instructions) {
        compiledInstructions.emplace_back(CompiledInstruction(instruction, accountKeys));
    }
}
}
