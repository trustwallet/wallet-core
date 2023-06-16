// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include <proto/TransactionCompiler.pb.h>
#include "Address.h"
#include "Signer.h"

using namespace TW;
using namespace std;

namespace TW::Cosmos {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    if (auto* hrp = std::get_if<Bech32Prefix>(&addressPrefix); hrp) {
        return Address::isValid(address, *hrp);
    }
    return Address::isValid(coin, address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    if (std::holds_alternative<Bech32Prefix>(addressPrefix)) {
        const std::string hrp = std::get<Bech32Prefix>(addressPrefix);
        if (!hrp.empty()) {
            return Address(hrp, publicKey, coin).string();
        }
    }
    return Address(coin, publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    Address addr;
    if (!Address::decode(address, addr)) {
        return Data();
    }
    return addr.getKeyHash();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    auto input = Proto::SigningInput();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serializedOut = Signer::sign(input, coin).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return Signer::signJSON(json, key, coin);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [&coin](const auto& input, auto& output) {
            auto pkVec = Data(input.public_key().begin(), input.public_key().end());
            auto preimage = Signer().signaturePreimage(input, pkVec, coin);
            auto imageHash = Hash::sha256(preimage);
            output.set_data(preimage.data(), preimage.size());
            output.set_data_hash(imageHash.data(), imageHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [coin](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            auto signedTx = Signer().encodeTransaction(input, signature, publicKey, coin);
            output.set_serialized(signedTx.data(), signedTx.size());
        });
}

} // namespace TW::Cosmos
