// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"

#include "../Base58.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

#include <cassert>

namespace TW::FIO {

using namespace std;

Data Signer::signData(const PrivateKey& privKey, const Data& data) {
    Data hash = Hash::sha256(data);
    Data signature = privKey.sign(hash, TWCurveSECP256k1, isCanonical);
    return signature;
}

std::string Signer::signatureToBsase58(const Data& sig) {
    Data sigWithSuffix(sig);
    append(sigWithSuffix, TW::data(SignatureSuffix));
    // take hash, ripemd, first 4 bytes 
    Data hash = Hash::ripemd(sigWithSuffix);
    Data sigWithChecksum(sig);
    append(sigWithChecksum, TW::data(hash.data(), 4));
    string s = SignaturePrefix + Base58::bitcoin.encode(sigWithChecksum);
    return s;
}

bool Signer::verify(const PublicKey& pubKey, const Data& data, const Data& signature) {
    return pubKey.verify(TW::data(signature.data() + 1, signature.size() - 1), data);
}

// canonical check for FIO, both R and S lenght is 32
int Signer::isCanonical(uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80)
        && !(sig[0] == 0 && !(sig[1] & 0x80))
        && !(sig[32] & 0x80)
        && !(sig[32] == 0 && !(sig[33] & 0x80));
}

string Actor::actor(const Address& addr)
{
    uint64_t shortenedKey = shortenKey(addr.bytes);
    string name13 = name(shortenedKey);
    // trim to 12 chracters
    return name13.substr(0, 12);
}

uint64_t Actor::shortenKey(const array<byte, Address::size>& addrKey)
{
    uint64_t res = 0;
    int i = 1; // Ignore key head
    int len = 0;
    while (len <= 12) {
        assert(i < 33); // Means the key has > 20 bytes with trailing zeroes...
        
        auto trimmed_char = uint64_t(addrKey[i] & (len == 12 ? 0x0f : 0x1f));
        if (trimmed_char == 0) { i++; continue; }  // Skip a zero and move to next

        auto shuffle = len == 12 ? 0 : 5 * (12 - len) - 1;
        res |= trimmed_char << shuffle;

        len++; i++;
    }
    return res;
}

string Actor::name(uint64_t shortKey) {
    static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";

    string str(13,'.'); //We are forcing the string to be 13 characters

    uint64_t tmp = shortKey;
    for(uint32_t i = 0; i <= 12; i++ ) {
        char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
        str[12 - i] = c;
        tmp >>= (i == 0 ? 4 : 5);
    }

    return str;
}

} // namespace TW::FIO
