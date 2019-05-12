// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Transaction.h"
#include "../Data.h"
#include "../BinaryCoding.h"
#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

using namespace TW;

ARK::Transaction::Transaction(){
    type = 0; // transfer
    fee = 10000000; // default fee
}

Data ARK::Transaction::encoded(){
    
    Data result = Data();

    // type should be 0 for transfer
    result.insert(result.end(), type);

    encode32LE(timestamp, result);   
    
    append(result, publicKey);

    append(result, Base58::bitcoin.decodeCheck(recipientId.string()));
    
    // vendor field
    std::vector<uint8_t> empty(64, 0);
    append(result, empty);

    encode64LE(amount, result);
    encode64LE(fee, result);

    if( signature.size() > 0 ){
        append(result, signature);
    }
    return result;
}

void ARK::Transaction::setTimestamp(uint32_t &timeInput){
    if (timeInput > 1490101200){
        // ark subracts that number from unix Tuesday, March 21, 2017 1:00:00 PM
        timestamp = timeInput - 1490101200;
    }else{
        timestamp = timeInput;
    }
}

uint32_t ARK::Transaction::getTimestamp(){
    return timestamp;
}

std::string ARK::Transaction::toJson(){
    json j = {
        {"amount", amount},
        {"asset", {}},
        {"fee", fee},
        {"id", hex(Hash::sha256(encoded()))},
        {"recipientId", recipientId.string()},
        {"senderPublicKey", hex(publicKey)},
        {"signature", hex(signature)},
        {"timestamp", timestamp},
        {"type", type}
    };
    return j.dump();
}