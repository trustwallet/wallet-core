// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

using namespace TW::Binance;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const { 
    return Signer::signJSON(json, key);
}

HashPubkeyList Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    auto input = Proto::SigningInput();
    Data preImageHash;
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        const auto signer = Signer(input);
        // return preimage hash and dummy pubkeyhash (only one signature anyways)
        preImageHash = signer.preImageHash();
    }
    return HashPubkeyList{std::make_pair(preImageHash, Data())};
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = Proto::SigningInput();
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        const auto signer = Signer(input);
        if (signatures.size() == 1 && publicKeys.size() == 1) {
            const auto output = signer.compile(signatures[0], publicKeys[0]);
            const auto serializedOut = output.SerializeAsString();
            dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
        }
    }
}

Data Entry::buildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId) const {
    auto input = Proto::SigningInput();
    input.set_chain_id(chainId.length() > 0 ? chainId : "Binance-Chain-Nile");
    input.set_account_number(0);
    input.set_sequence(0);
    input.set_source(0);
    input.set_memo(memo);
    // do not set private_key!
    input.set_private_key("");

    auto& order = *input.mutable_send_order();

    Address fromAddress;
    if (!Address::decode(from, fromAddress)) {
        throw std::invalid_argument("Invalid from address");
    }
    const auto fromKeyhash = fromAddress.getKeyHash();
    Address toAddress;
    if (!Address::decode(to, toAddress)) {
        throw std::invalid_argument("Invalid to address");
    }
    const auto toKeyhash = toAddress.getKeyHash();

    {
        auto input = order.add_inputs();
        input->set_address(fromKeyhash.data(), fromKeyhash.size());
        auto inputCoin = input->add_coins();
        inputCoin->set_denom(asset);
        inputCoin->set_amount(static_cast<uint64_t>(amount));
    }
    {
        auto output = order.add_outputs();
        output->set_address(toKeyhash.data(), toKeyhash.size());
        auto outputCoin = output->add_coins();
        outputCoin->set_denom(asset);
        outputCoin->set_amount(static_cast<uint64_t>(amount));
    }

    const auto txInputData = data(input.SerializeAsString());
    return txInputData;
}
