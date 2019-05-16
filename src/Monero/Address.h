// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Hash.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include <TrustWalletCore/TWX509.h>

#include <string>

namespace TW::Monero{

    class Address{
    public:
        /// Number of base58 encoded bytes in an address;
        static const size_t size = 95;
        static const size_t standardAddressSize = 95;
        static const size_t integratedAddressSize = 106;

        static const uint8_t mainnetPrefix = 12;
        static const uint8_t testnetPrefix = 35;
        static const uint8_t integratedPrefix = 13;
        static const uint8_t truncatedPrefix = 11;

        /// Address data followed by the public key
        std::array<uint8_t, size> bytes{};

        /// Determines whether a collection of bytes makes a valid address.
        static bool isValid(const Data &data){ return data.size() == size; }

        /// Determines whether a string makes a valid address
        static bool isValid(const std::string &string);

        Address(const uint8_t networkBytes, PublicKey &spendKey, PublicKey &viewKey);

        /// Initializes an address from a string representation
        Address(const std::string &string);


        Address(const uint8_t networkBytes, PublicKey &spendKey, PublicKey &viewKey, const Data &paymentID);

        /// Returns a string representation of the address. 20-bytes hex-encoded
        std::string string() const;

        static PrivateKey privateViewKeyFromPrivateSpendKey(PrivateKey privateSpendKey);

        static PublicKey publicKeyFromPrivateKey(PrivateKey privateKey);

        static std::string getStealthAddress(PublicKey publicViewKey, PublicKey publicSpendKey);

        static std::string randomScalar();

        static bool generateKeyDerivation(const PublicKey& key1, const PrivateKey& key2, char* derivation);

        static std::vector<byte> derivationToScalar(char* derivation, size_t output_index);


    };

    inline bool operator==(const Address &lhs, const Address &rhs){
        return lhs.bytes == rhs.bytes;
    }


} // namespace TW::Monero

/// Wrapper for C interface
struct TWMoneroAddress{
    TW::Monero::Address impl;
};