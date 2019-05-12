// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Address.h"
#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW;

byte TW::ARK::Address::mainnetPrefix = 0x17;

bool TW::ARK::Address::isValid(const Data &data){
    return isValid(Base58::bitcoin.encode(data));
}

bool TW::ARK::Address::isValid(const std::string &string ){
    Data decoded = Base58::bitcoin.decodeCheck(string);

    if ( decoded.size() == 0 ){
        return false;
    }
    
    // mainnet
    if (decoded[0] != TW::ARK::Address::mainnetPrefix){
        return false;
    }
    
    return true;
}

TW::ARK::Address::Address(const std::string &string){
    address = string;
}

TW::ARK::Address::Address(const Data &data){
    address = Base58::bitcoin.encode(data);
}

TW::ARK::Address::Address(const PublicKey &publicKey){
    Data hash = Data();

    hash.insert(hash.end(), TW::ARK::Address::mainnetPrefix);
    append(hash, Hash::ripemd(publicKey.bytes));

    address = Base58::bitcoin.encodeCheck(hash);
}

std::string TW::ARK::Address::string() const{
    return address;
}

Data TW::ARK::Address::bytes(){
    return Base58::bitcoin.decode(address);
}

