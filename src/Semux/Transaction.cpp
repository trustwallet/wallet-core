// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Hash.h"
#include "Signer.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Semux;

std::vector<uint8_t> Transaction::serialize() const {
    std::vector<uint8_t> data;

    auto hash = this->getHash();
    this->writeBytes(Data(hash.begin(), hash.end()), data);

    auto preimage = this->getPreImage();
    this->writeBytes(Data(preimage.begin(), preimage.end()), data);

    this->writeBytes(Data(this->signature.begin(), this->signature.end()), data);

    return data;
}

std::vector<uint8_t> Transaction::getPreImage() const {
    std::vector<uint8_t> data;

    data.push_back(this->network);
    data.push_back(this->transactionType);
    this->writeBytes(Data(this->to.bytes.begin(), this->to.bytes.end()), data);
    this->writeLong(this->value, data);
    this->writeLong(this->fee, data);
    this->writeLong(this->nonce, data);
    this->writeLong(this->timestamp, data);
    this->writeBytes(this->data, data);

    return data;
}

std::vector<uint8_t> Transaction::getHash() const {
    auto preimage = this->getPreImage();
    return Hash::blake2b(preimage, 32);
}