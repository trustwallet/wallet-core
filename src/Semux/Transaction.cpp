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

void Transaction::writeBytes(const Data &bytes, Data &buffer) const {
    encodeVarInt(bytes.size(), buffer);
    buffer.insert(buffer.end(), bytes.begin(), bytes.end());
}

std::vector<uint8_t> Transaction::serialize() const {
    std::vector<uint8_t> buffer;

    auto hash = this->getHash();
    this->writeBytes(Data(hash.begin(), hash.end()), buffer);

    auto preimage = this->getPreImage();
    this->writeBytes(Data(preimage.begin(), preimage.end()), buffer);

    this->writeBytes(Data(this->signature.begin(), this->signature.end()), buffer);

    return buffer;
}

std::vector<uint8_t> Transaction::getPreImage() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(this->network);
    buffer.push_back(this->transactionType);
    this->writeBytes(Data(this->to.bytes.begin(), this->to.bytes.end()), buffer);
    encode64BE(this->value, buffer);
    encode64BE(this->fee, buffer);
    encode64BE(this->nonce, buffer);
    encode64BE(this->timestamp, buffer);
    this->writeBytes(this->data, buffer);

    return buffer;
}

std::vector<uint8_t> Transaction::getHash() const {
    auto preimage = this->getPreImage();
    return Hash::blake2b(preimage, 32);
}