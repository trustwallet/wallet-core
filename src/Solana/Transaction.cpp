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

Data Transaction::serialize() const {
    Data buffer;

    append(buffer, shortVecLength<Signature>(this->signatures));
    for (auto signature : this->signatures) {
        Data signature_vec(signature.bytes.begin(), signature.bytes.end());
        append(buffer, signature_vec);
    }
    append(buffer, this->messageData());

    return buffer;
}

Data Transaction::messageData() const {
    Data buffer;

    buffer.push_back(this->message.header.numRequiredSignatures);
    buffer.push_back(this->message.header.numCreditOnlySignedAccounts);
    buffer.push_back(this->message.header.numCreditOnlyUnsignedAccounts);
    append(buffer, shortVecLength<Address>(this->message.accountKeys));
    for (auto account_key : this->message.accountKeys) {
        Data account_key_vec(account_key.bytes.begin(), account_key.bytes.end());
        append(buffer, account_key_vec);
    }
    Data recentBlockhash(this->message.recentBlockhash.bytes.begin(),
                         this->message.recentBlockhash.bytes.end());
    append(buffer, recentBlockhash);
    append(buffer, shortVecLength<CompiledInstruction>(this->message.instructions));
    for (auto instruction : this->message.instructions) {
        buffer.push_back(instruction.programIdIndex);
        append(buffer, shortVecLength<uint8_t>(instruction.accounts));
        append(buffer, instruction.accounts);
        append(buffer, shortVecLength<uint8_t>(instruction.data));
        append(buffer, instruction.data);
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
