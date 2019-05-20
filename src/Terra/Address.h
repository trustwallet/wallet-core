#pragma once


#include <string>
#include "../PublicKey.h"


namespace TW::TERRA {

    class Address {

      ///Human-readable part.
        std::string hrp;

        /// Public key hash.
        Data keyHash;

    public:
        /// Determines whether a string makes a valid Terra address.
        static bool isValid(const std::string& string);

        /// Initializes an address with a public key.
        explicit Address(std::string hrp, const PublicKey& publicKey);

        /// Returns a string representation of the address.
        std::string string() const;

        /// Initializes an address from a string representation.
        explicit Address(const std::string& string);
    };
} //namespace TW::TERRA
/// Wrapper for C interface.
struct TWTerraAddress {
    TW::TERRA::Address impl;
};
