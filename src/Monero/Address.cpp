// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "stdint.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include <PrivateKey.h>
#include <fcntl.h>

#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <Monero/crypto/crypto_ops.h>
#include <unistd.h>
#include "Monero/crypto/base58.h"
#include "Monero/crypto/varint.h"

using namespace TW;
using namespace TW::Monero;

bool Address::isValid(const std::string &string){
    // TODO
    // 1. Valid Standard Address
    // 2. Valid Integrated Address
    // 3. Valid Subaddress
    // 4. Valid Stealth Address

    if (string.size() != 95 && string.size() != 106){
        std::cout<<"String size: "<<(string.size() == 106)<<std::endl;
        return false;
    }
    std::string decodedAddress;
    tools::base58::decode(string, decodedAddress);
    std::cout<<"what";
    return Address::isValid(parse_hex(decodedAddress));
}

Address::Address(const uint8_t networkBytes, PublicKey &spendKey, PublicKey &viewKey){

    std::string address_hex = std::to_string(networkBytes) + hex(spendKey.bytes) + hex(viewKey.bytes) ;
    Data checksum_hash = Hash::keccak256(parse_hex(address_hex));
    address_hex += hex(checksum_hash.begin(), checksum_hash.begin()+4);
    std::string base58_address = tools::base58::encode(address_hex.data());
    std::vector<char> base58bytes = std::vector<char>(base58_address.begin(), base58_address.end());
    std::copy(base58bytes.begin(), base58bytes.end(), this->bytes.begin());
}

Address::Address(const std::string &string){
    // TODO add base58 decode check
    // TODO add string validation
//    if (!isValid(string)){
//        throw std::invalid_argument("Invalid address data");
//    }

    const std::vector<char> addressBytes = std::vector<char>(string.begin(), string.end());
    std::copy(addressBytes.begin(), addressBytes.begin()+95, this->bytes.begin());
}

Address::Address(const uint8_t networkBytes, PublicKey &spendKey, PublicKey &viewKey, const Data &paymentID){
    std::string address_hex = std::to_string(networkBytes) + hex(spendKey.bytes) + hex(viewKey.bytes) + hex(paymentID);
    Data checksum_hash = Hash::keccak256(parse_hex(address_hex));
    address_hex += hex(checksum_hash.begin(), checksum_hash.begin()+4);
    std::string base58_address = tools::base58::encode(address_hex.data());
    std::vector<char> base58bytes = std::vector<char>(base58_address.begin(), base58_address.end());
    std::copy(base58bytes.begin(), base58bytes.end(), this->bytes.begin());
}


std::string Address::string() const {
    return std::string(bytes.begin(), bytes.end());
}

PrivateKey Address::privateViewKeyFromPrivateSpendKey(PrivateKey privateSpendKey) {
    auto pskHash = TW::Hash::keccak256(privateSpendKey.bytes);
    sc_reduce32(pskHash.data());
    PrivateKey privateViewKey = PrivateKey(pskHash);

    return privateViewKey;
}

PublicKey Address::publicKeyFromPrivateKey(PrivateKey privateKey) {
    ge_p3 ge_p3_point;
    TW::Data publicKeyData = TW::Data(32);

    ge_scalarmult_base(&ge_p3_point, privateKey.bytes.data());
    ge_p3_tobytes(publicKeyData.data(), &ge_p3_point);
    PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);

    return publicKey;
}

std::string Address::getStealthAddress(PublicKey publicViewKey, PublicKey publicSpendKey){

    auto r = randomScalar();
    PrivateKey randomKey = PrivateKey(parse_hex(r));
    PublicKey publicRandomKey = TW::Monero::Address::publicKeyFromPrivateKey(randomKey);


    PublicKey publicKey = PublicKey(parse_hex("6bb8297dc3b54407ac78ffa4efa4afbe5f1806e5e41aa56ae98c2fe53032bb4b"), TWPublicKeyTypeED25519);
    PrivateKey privateKey = PrivateKey(parse_hex("c91ae3053f640fcad393fb6c74ad9f064c25314c8993c5545306154e070b1f0f"));

    Data keyDerivation = Data(32);
    generateKeyDerivation(publicKey, privateKey, reinterpret_cast<char *>(keyDerivation.data()));
    auto scalar = derivationToScalar(reinterpret_cast<char *>(keyDerivation.data()), 0); // output index 0 by default

    PublicKey pub_scalar = TW::Monero::Address::publicKeyFromPrivateKey(PrivateKey(scalar));


    ge_p3 point1_m2;
    ge_p3 point2_m2;


    if (ge_frombytes_vartime(&point1_m2, publicSpendKey.bytes.data()) != 0) {
        throw std::invalid_argument("Invalid key data");
    }
    if (ge_frombytes_vartime(&point2_m2, pub_scalar.bytes.data()) != 0) {
        throw std::invalid_argument("Invalid key data");
    }

    ge_cached p3_m;
    ge_p1p1 p1_m;
    ge_p2 p2_m;
    unsigned char sum_m[32];

    ge_p3_to_cached(&p3_m, &point2_m2);
    ge_add(&p1_m, &point1_m2, &p3_m);

    ge_p1p1_to_p2(&p2_m, &p1_m);
    ge_tobytes(reinterpret_cast<unsigned char *>(&sum_m), &p2_m);

    std::string hexAddress = hex(sum_m, sum_m+sizeof(sum_m));
    return hexAddress;
}

std::string Address::randomScalar() {
    unsigned char buff[32]={0};
    int randomData = open("/dev/urandom", O_RDONLY);
    read(randomData, buff, sizeof(buff));
    close(randomData);
    sc_reduce32(buff);

    return hex(buff);
}

bool Address::generateKeyDerivation(const PublicKey &key1, const PrivateKey &key2, char *derivation) {
    ge_p3 point;
    ge_p2 point2;
    ge_p1p1 point3;

    assert(sc_check(key2.bytes.data()) == 0);
    if (ge_frombytes_vartime(&point, key1.bytes.data()) != 0) {
        return false;
    }

    ge_scalarmult(&point2, key2.bytes.data(), &point);
    ge_mul8(&point3, &point2);
    ge_p1p1_to_p2(&point2, &point3);
    ge_tobytes(reinterpret_cast<unsigned char *>(derivation), &point2);

    return true;
}

std::vector<byte> Address::derivationToScalar(char *derivation, size_t output_index) {
    auto output_index_hex = hex((uint64_t)output_index); // up to 127
    auto buf = hex(derivation, derivation+32) + output_index_hex.substr(output_index_hex.length()-2);
    auto scalar = Hash::keccak256(parse_hex(buf));
    sc_reduce32(scalar.data());
    return scalar;
}
