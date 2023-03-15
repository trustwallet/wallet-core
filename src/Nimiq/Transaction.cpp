// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Signer.h"
#include "../BinaryCoding.h"

namespace TW::Nimiq {

const uint8_t NETWORK_ID = 42;
const uint8_t EMPTY_FLAGS = 0;

std::vector<uint8_t> Transaction::serialize() const {
    std::vector<uint8_t> data;

    data.push_back(0x00); // Basic TX type
    data.insert(data.end(), sender_pub_key.begin(), sender_pub_key.end());
    data.insert(data.end(), destination.bytes.begin(), destination.bytes.end());
    encode64BE(amount, data);
    encode64BE(fee, data);
    encode32BE(vsh, data);
    data.push_back(NETWORK_ID);
    data.insert(data.end(), signature.begin(), signature.end());

    return data;
}

std::vector<uint8_t> Transaction::getPreImage() const {
    std::vector<uint8_t> data;

    // Build pre-image
    Address sender(Signer::publicKeyFromBytes(sender_pub_key));
    encode16BE(0x00, data); // Data size (+ 0 bytes of data)
    data.insert(data.end(), sender.bytes.begin(), sender.bytes.end());
    data.push_back(0); // Sender is basic account type
    data.insert(data.end(), destination.bytes.begin(), destination.bytes.end());
    data.push_back(0); // Recipient is basic account type
    encode64BE(amount, data);
    encode64BE(fee, data);
    encode32BE(vsh, data);
    data.push_back(NETWORK_ID);
    data.push_back(EMPTY_FLAGS);

    return data;
}

} // namespace TW::Nimiq
