// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <cstdint>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/memzero.h>
#include <TrezorCrypto/sha2.h>
#include <cstring>
#include <iostream>
#include <boost/crc.hpp>
#include <vector>
#include <HexCoding.h>
#include <Monero/crypto/int-util.h>

#include "english.h"
#include "mnemonic.h"

namespace TW::Monero {

    const size_t wordlist_size = sizeof(wordlist)/sizeof(wordlist[0])-1;

    uint32_t getChecksumIndex(std::vector<std::string> words, uint32_t prefix_length) {
        if (words.size() != 12 && words.size() != 24) {
            throw std::invalid_argument("Word list must be 12 or 24 words");
        }
        std::string trimmed_words;
        for (auto &word : words) {
            trimmed_words.append(word.substr(0, prefix_length));
        }
        boost::crc_32_type result;
        result.process_bytes(trimmed_words.data(), trimmed_words.length());
        uint32_t checksum_index = result.checksum() % (trimmed_words.length() / prefix_length);

        return checksum_index;
    }

    std::vector<std::string> generateSeed(size_t seed_strength) {
        std::string mnemonic;
        if (!mnemonicGenerate(seed_strength, mnemonic.data())) {
            throw std::runtime_error("Mnemonic generation failed");
        }
        std::vector<std::string> words;
        char *token = std::strtok(mnemonic.data(), " ");
        while (token != NULL) {
            words.push_back(token);
            token = strtok(NULL, " ");
        }
        std::string checksum = words[getChecksumIndex(words, 3)];
        words.push_back(checksum);

        return words;
    }

    bool mnemonicFromData(const uint8_t *data, size_t len, char *mnemonic) {
        if (len % 4 || len < 16 || len > 32) {
            return 0;
        }

        uint8_t bits[32 + 1];
        sha256_Raw(data, len, bits);
        // checksum
        bits[len] = bits[0];
        // data
        memcpy(bits, data, len);
        int mlen = len * 3 / 4;
        int i, j, idx;
        char *p = mnemonic;
        for (i = 0; i < mlen; i++) {
            idx = 0;
            for (j = 0; j < 11; j++) {
                idx <<= 1;
                idx += (bits[(i * 11 + j) / 8] & (1 << (7 - ((i * 11 + j) % 8)))) > 0;
                if (idx > wordlist_size) {
                    idx %= wordlist_size;
                }
            }

            strcpy(p, wordlist[idx]);
            p += strlen(wordlist[idx]);
            *p = (i < mlen - 1) ? ' ' : 0;
            p++;
        }
        memzero(bits, sizeof(bits));

        return true;
    }

    bool mnemonicGenerate(int strength, char *mnemonic) {
        if (strength % 32 || strength < 128 || strength > 256) {
            return false;
        }
        uint8_t data[32];
        random_buffer(data, 32);
        bool success = mnemonicFromData(data, strength / 8, mnemonic);
        memzero(data, sizeof(data));

        return success;
    }

    std::string mnemonicDecode(std::string mnemonic){
        std::vector<std::string> words;
        char *token = std::strtok(mnemonic.data(), " ");
        while (token != NULL) {
            words.push_back(token);
            token = strtok(NULL, " ");
        }

        if (words.size() != 13 && words.size() != 25){
            throw std::invalid_argument("Mnemonic must be 13 or 25 words");
        }

        uint32_t w0, w1, w2, w3;
        std::string seed;
        for (int i = 0; i <words.size()-2; i+=3){
           w1 = findWordlistIndex(words[i]);
           w2 = findWordlistIndex(words[i+1]);
           w3 = findWordlistIndex(words[i+2]);

            if (w1 == -1 || w2 == -1 || w3 == -1){
                throw std::invalid_argument("Word not in mnemonic word list.");
            }

            w0 = w1 + wordlist_size * (((wordlist_size - w1) + w2) % wordlist_size) + wordlist_size * wordlist_size * (((wordlist_size - w2) + w3) % wordlist_size);

            if (w0 % wordlist_size != w1){
                throw std::invalid_argument("Something went wrong decoding the private key.");
            }

            auto swap = SWAP32LE(w0);
            seed.append((const char*)&swap, 4);
        }

        return hex(seed);
    }

}