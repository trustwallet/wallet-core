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

PrivateKey Entry::decodePrivateKey(TWCoinType coin, const std::string& privateKey) const {
    auto data = Base58::decode(privateKey);
    if (data.size() == 64) {
        const auto privateKeyData = subData(data, 0, 32);
        const auto publicKeyData = subData(data, 32, 32);
        auto privKey = PrivateKey(privateKeyData, TW::curve(coin));
        auto publicKey = privKey.getPublicKey(TWPublicKeyType::TWPublicKeyTypeED25519);
        if (publicKey.bytes != publicKeyData) {
            throw std::invalid_argument("Invalid private key");
        }
        return privKey;
    } else if (data.size() == 32) {
        return PrivateKey(data, TW::curve(coin));
    } else {
        auto hexData = parse_hex(privateKey);
        return PrivateKey(hexData, TW::curve(coin));
    }
}

} // namespace TW::Solana
