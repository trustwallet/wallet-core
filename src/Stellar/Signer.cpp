// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Signer.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWStellarMemoType.h>

using namespace TW;
using namespace TW::Stellar;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto signer = Signer(input);
    auto output = Proto::SigningOutput();
    output.set_signature(signer.sign());
    return output;
}

std::string Signer::sign() const noexcept {

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto account = Address(input.account());
    auto encoded = encode(input);

    auto encodedWithHeaders = Data();
    auto publicNetwork = input.passphrase(); // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2}; // Header
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                              transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());

    auto hash = Hash::sha256(encodedWithHeaders);
    auto data = Data(hash.begin(), hash.end());

    auto sign = key.sign(data, TWCurveED25519);

    auto signature = Data();
    signature.insert(signature.end(), encoded.begin(), encoded.end());
    encode32BE(1, signature);
    signature.insert(signature.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sign.size()), signature);
    signature.insert(signature.end(), sign.begin(), sign.end());
    return Base64::encode(signature);
}

Data Signer::encode(const Proto::SigningInput& input) const {
    //    Address account, uint32_t fee, uint64_t sequence, uint32_t memoType,
    //    Data memoData, Address destination, uint64_t amount;
    auto data = Data();
    encodeAddress(Address(input.account()), data);
    encode32BE(input.fee(), data);
    encode64BE(input.sequence(), data);
    encode32BE(0, data); // Time bounds
    if (input.has_memo_id()) {
        encode32BE(TWStellarMemoTypeId, data);
        encode64BE(input.memo_id().id(), data);
    } else if (input.has_memo_text()) {
        encode32BE(TWStellarMemoTypeText, data);
        encode32BE(static_cast<uint32_t>(input.memo_text().text().size()), data);
        data.insert(data.end(), input.memo_text().text().begin(), input.memo_text().text().end());
        pad(data);
    } else if (input.has_memo_hash()) {
        encode32BE(TWStellarMemoTypeHash, data);
        data.insert(data.end(), input.memo_hash().hash().begin(), input.memo_hash().hash().end());
    } else if (input.has_memo_return_hash()) {
        encode32BE(TWStellarMemoTypeReturn, data);
        data.insert(data.end(), input.memo_return_hash().hash().begin(),
                    input.memo_return_hash().hash().end());
    } else {
        encode32BE(TWStellarMemoTypeNone, data);
    }
    // Operations
    encode32BE(1, data);                      // Operation list size. Only 1 operation.
    encode32BE(0, data);                      // Source equals account
    encode32BE(input.operation_type(), data); // Operation type - PAYMENT
    encodeAddress(Address(input.destination()), data);
    if (input.operation_type() == Proto::SigningInput_OperationType_PAYMENT) {
        encode32BE(0, data); // Asset type
    }
    encode64BE(input.amount(), data);
    encode32BE(0, data); // Ext
    return data;
}

void Signer::encodeAddress(const Address& address, Data& data) const {
    encode32BE(0, data);
    data.insert(data.end(), address.bytes.begin(), address.bytes.end());
}

void Signer::pad(Data& data) const {
    int pad = 0;
    int mod = static_cast<int>(data.size() % 4);
    if (mod > 0) {
        pad = 4 - mod;
    }
    while (pad-- > 0) {
        data.insert(data.end(), 0);
    }
}