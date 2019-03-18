// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Forging.h"
#include "Transaction.h"

#include "../PublicKey.h"
#include "../HexCoding.h"
#include <variant>
#include <sstream>

using namespace std;
using namespace TW::Tezos;

// Forge the given zarith hash into a hex encoded string.
// TODO(keefertaylor): Move to forging.cpp, unit test.
Data forgeZarith(int input) {
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
    return parse_hex(result);
}

string Transaction::forge() {
    auto forgedSource = source.forge();
    auto forgedFee = forgeZarith(fee);
    auto forgedCounter = forgeZarith(counter);
    auto forgedGasLimit = forgeZarith(gas_limit);
    auto forgedStorageLimit = forgeZarith(storage_limit);

    if (kind == operationtype::REVEAL) {
        if(auto publicKey = std::get_if<PublicKey>(&destination_or_public_key))
            return "07" + hex(forgedSource) + hex(forgedFee) + hex(forgedCounter) + hex(forgedGasLimit)
                + hex(forgedStorageLimit) + hex(forgePublicKey(*publicKey));
        else throw std::invalid_argument( "Invalid publicKey" );
    }
    auto forgedAmount = forgeZarith(amount);
    if(auto destination = std::get_if<Address>(&destination_or_public_key))
        return "08" + hex(forgedSource) + hex(forgedFee) + hex(forgedCounter) + hex(forgedGasLimit)
            + hex(forgedStorageLimit) + hex(forgedAmount) + hex(destination->forge()) + hex(forgeBool(false));
    else throw std::invalid_argument( "Invalid destination" );
}
