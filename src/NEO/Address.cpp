// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Ontology/ParamsBuilder.h"
#include "../Base58.h"
#include "../Hash.h"
#include "../Data.h"
#include "OpCode.h"

#include "Address.h"

using namespace TW;
using namespace TW::NEO;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    return !(decoded.size() != Address::size || decoded[0] != version);
}

Address::Address(const PublicKey& publicKey) {
    auto publicKeyData = publicKey.bytes;

    auto pkdata = Data(publicKeyData.begin(), publicKeyData.end());
    pkdata.insert(pkdata.begin(), PUSHBYTES21);
    pkdata.push_back(CHECKSIG);

    auto keyHash = Hash::ripemd(Hash::sha256(pkdata));
    keyHash.insert(keyHash.begin(), (byte) Address::version);

    if (keyHash.size() != Address::size) {
        throw std::invalid_argument("Invalid address key data");
    }

    std::copy(keyHash.data(), keyHash.data() + Address::size, bytes.begin());
}

Address::Address(uint8_t m, const std::vector<Data>& publicKeys) {
    auto builderData = toScriptHash(Ontology::ParamsBuilder::fromMultiPubkey(m, publicKeys));
    std::copy(builderData.begin(), builderData.end(), bytes.begin());
}

Data Address::toScriptHash(const Data& data) const {
    return Hash::ripemd(Hash::sha256(data));
}

Data Address::toScriptHash() const {
    byte buf[20];
    Data data(buf, buf + 20);
    std::copy(bytes.begin() + 1, bytes.begin() + 21, data.begin());
    return data;
}
