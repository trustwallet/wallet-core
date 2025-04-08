// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Base58.h"
#include "Coin.h"
#include "HexCoding.h"
#include "proto/Solana.pb.h"

using namespace TW;
using namespace std;

namespace TW::Solana {

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return signJSONHelper<Proto::SigningInput, Proto::SigningOutput>(
        coin,
        json,
        key,
        [](const Proto::SigningOutput& output) { return output.encoded(); }
    );
}

Data Entry::decodePrivateKey(TWCoinType coin, const std::string& privateKey) const {
    auto data = Base58::decode(privateKey);
    if (data.size() == 64) {
        const auto privateKeyData = Data(data.begin(), data.begin() + 32);
        const auto publicKeyData = Data(data.begin() + 32, data.end());
        auto privKey = PrivateKey(privateKeyData);
        auto publicKey = privKey.getPublicKey(TWPublicKeyType::TWPublicKeyTypeED25519);
        if (publicKey.bytes != publicKeyData) {
            throw std::invalid_argument("Invalid private key");
        }
        return privateKeyData;
    } else if (data.size() == 32) {
        if (!PrivateKey::isValid(data, TW::curve(coin))) {
            throw std::invalid_argument("Invalid private key");
        }
        return data;
    } else {
        auto hexData = parse_hex(privateKey);
        if (!PrivateKey::isValid(hexData, TW::curve(coin))) {
            throw std::invalid_argument("Invalid private key");
        }
        return hexData;
    }
}

} // namespace TW::Solana
