// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

<<<<<<< HEAD
 #pragma once

 #include "../Data.h"
#include "../PublicKey.h"

 #include <string>
=======
#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>
>>>>>>> de47d7ee9c2dec97dbf7f5626e4c2270e7008ac8
#include <vector>

namespace TW {
namespace Tezos {

<<<<<<< HEAD
 class Address {
=======
class Address {
>>>>>>> de47d7ee9c2dec97dbf7f5626e4c2270e7008ac8
public:
    static const size_t addressSize = 27;
    static const size_t checksumLength = 4;
    /// Number of bytes in an address.
    static const size_t size = 23;
    std::array<byte, size> bytes;
    std::string public_key_hash;

<<<<<<< HEAD
     /// Initializes a Tezos address with a public_key hash.
=======
    /// Initializes a Tezos address with a public_key hash.
>>>>>>> de47d7ee9c2dec97dbf7f5626e4c2270e7008ac8
    Address(const std::string& string);
    Address(const std::vector<uint8_t>& data);
    Address(const PublicKey& publicKey);
    static bool isValid(const std::string& string);
    /// Returns a string representation of the address.
    std::string string() const;
};

<<<<<<< HEAD
 }} // namespace
=======
}} // namespace
>>>>>>> de47d7ee9c2dec97dbf7f5626e4c2270e7008ac8
