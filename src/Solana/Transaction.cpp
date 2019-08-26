// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Hash.h"
#include "Signer.h"
#include "../BinaryCoding.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Solana;

void Transaction::writeBytes(const Data &bytes, Data &buffer) {
    buffer.insert(buffer.end(), bytes.begin(), bytes.end());
}

Data Transaction::serialize() const {
    std::vector<uint8_t> buffer;

    this->writeBytes(shortVecLength<Signature>(this->signatures), buffer);
    for (auto signature : this->signatures) {
        Data signature_vec(signature.bytes.begin(), signature.bytes.end());
        this->writeBytes(signature_vec, buffer);
    }
    this->writeBytes(this->messageData(), buffer);

    return buffer;
}

std::vector<uint8_t> Transaction::messageData() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(this->message.header.numRequiredSignatures);
    buffer.push_back(this->message.header.numCreditOnlySignedAccounts);
    buffer.push_back(this->message.header.numCreditOnlyUnsignedAccounts);
    this->writeBytes(shortVecLength<Address>(this->message.accountKeys), buffer);
    for (auto account_key : this->message.accountKeys) {
        Data account_key_vec(account_key.bytes.begin(), account_key.bytes.end());
        this->writeBytes(account_key_vec, buffer);
    }
    Data recentBlockhash(this->message.recentBlockhash.bytes.begin(),
                         this->message.recentBlockhash.bytes.end());
    this->writeBytes(recentBlockhash, buffer);
    this->writeBytes(shortVecLength<CompiledInstruction>(this->message.instructions), buffer);
    for (auto instruction : this->message.instructions) {
        buffer.push_back(instruction.programIdIndex);
        this->writeBytes(shortVecLength<uint8_t>(instruction.accounts), buffer);
        this->writeBytes(instruction.accounts, buffer);
        this->writeBytes(shortVecLength<uint8_t>(instruction.data), buffer);
        this->writeBytes(instruction.data, buffer);
    }

    return buffer;
}

uint8_t Transaction::getAccountIndex(Address publicKey) {
    std::vector<Address>::iterator item =
        std::find(this->message.accountKeys.begin(), this->message.accountKeys.end(), publicKey);
    if (item == this->message.accountKeys.end()) {
        throw std::invalid_argument("publicKey not found in message.accountKeys");
    }
    return (uint8_t)std::distance(this->message.accountKeys.begin(), item);
}

bool Signature::operator==(const Signature &v) const {
    return bytes == v.bytes;
}
