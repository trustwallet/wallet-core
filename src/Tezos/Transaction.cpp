// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "Transaction.h"

#include "../PublicKey.h"
#include <variant>
#include <sstream>

using namespace std;
using namespace TW::Tezos;

// Forge the given zarith hash into a hex encoded string.
std::string forgeZarith(int input) {
    std::string result = "";
    while (true) {
        if (input < 128) {
            if (input < 16) {
                result += "0";
            }
            std::stringstream ss;
            ss << std::hex << input;
            result += ss.str();
            break;
        } else {
            int b = input % 128;
            input -= b;
            input /= 128;
            b += 128;
            std::stringstream ss;
            ss << std::hex << b;
            result += ss.str();
        }
    }
    return result;
}

string Transaction::forge() {
    auto forgedSource = source.forge();
    auto forgedFee = forgeZarith(fee);
    auto forgedCounter = forgeZarith(counter);
    auto forgedGasLimit = forgeZarith(gas_limit);
    auto forgedStorageLimit = forgeZarith(storage_limit);

    if (kind == operationtype::REVEAL) {
        if(auto publicKey = std::get_if<PublicKey>(&destination_or_public_key))
            return "07" + forgedSource + forgedFee + forgedCounter + forgedGasLimit
                + forgedStorageLimit + forgePublicKey(*publicKey);
        else throw std::invalid_argument( "Invalid publicKey" );
    }
    auto forgedAmount = forgeZarith(amount);
    if(auto destination = std::get_if<Address>(&destination_or_public_key))
        return "08" + forgedSource + forgedFee + forgedCounter + forgedGasLimit
            + forgedStorageLimit + forgedAmount + destination->forge() + forgeBool(false);
    else throw std::invalid_argument( "Invalid destination" );
}
