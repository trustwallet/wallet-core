// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Base64.h>
#include "Signer.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Stellar;

std::string Signer::sign() const noexcept {

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Address account = input.account();
    auto encoded = encode(account, input.fee(), input.sequence(), Address(input.destination()), input.amount());

    auto encodedWithHeaders = Data();
    auto publicNetwork = std::string("Public Global Stellar Network ; September 2015");  // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2}; // Header
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(), transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());

    auto hash = Hash::sha256(encodedWithHeaders);
    auto data = Data(hash.begin(), hash.end());

    auto sign = key.sign(data, TWCurveEd25519);

    auto signature = Data();
    signature.insert(signature.end(), encoded.begin(), encoded.end());
    encode32BE(1, signature);
    signature.insert(signature.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sign.size()), signature);
    signature.insert(signature.end(), sign.begin(), sign.end());
    return Base64::encode(signature);
}

Data Signer::encode(Address account, uint32_t fee, uint64_t sequence, Address destination, uint64_t amount) const {
    auto data = Data();
    encodeAddress(account, data);
    encode32BE(fee, data);
    encode64BE(sequence, data);
    encode32BE(0, data); // Time bounds
    encode32BE(0, data); // Memo
    // Operations
    encode32BE(1, data); // Operation list size. Only 1 operation.
    encode32BE(0, data); // Source equals account
    encode32BE(1, data); // Operation type - PAYMENT
    encodeAddress(destination, data);
    encode32BE(0, data); // Asset type
    encode64BE(amount, data);
    encode32BE(0, data); // Ext
    return data;
}

void Signer::encodeAddress(Address address, Data& data) const {
    encode32BE(0, data);
    data.insert(data.end(), address.bytes.begin(), address.bytes.end());
}
