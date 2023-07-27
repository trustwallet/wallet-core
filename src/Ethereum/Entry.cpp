// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "HexCoding.h"
#include "proto/Ethereum.pb.h"

#include <google/protobuf/util/json_util.h>

namespace TW::Ethereum {

using namespace std;

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    return validateAddressRust(coin, address, addressPrefix);
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    return normalizeAddressRust(coin, address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    return deriveAddressRust(coin, publicKey, derivation, addressPrefix);
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    return addressToDataRust(coin, address);
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signRust(dataIn, coin, dataOut);
}

// TODO call `signRustJSON` when it's done.
string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string& json, const Data& key) const {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());

    auto inputData = data(input.SerializeAsString());
    Data dataOut;
    sign(coin, inputData, dataOut);

    if(dataOut.empty()) {
        return {};
    }

    Proto::SigningOutput output;
    output.ParseFromArray(dataOut.data(), static_cast<int>(dataOut.size()));

    return hex(output.encoded());
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return preImageHashesRust(coin, txInputData);
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    compileRust(coin, txInputData, signatures, publicKeys, dataOut);
}

} // namespace TW::Ethereum
