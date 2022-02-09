#pragma once

#include "../Data.h"
#include "../PublicKey.h"
#include  "../SS58Address.h"
#include  <TrustWalletCore/TWSS58AddressType.h>

#include <string>

namespace TW::Substrate {

class Address: public SS58Address {
  public:
    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string, byte network) { return SS58Address::isValid(string, network); }

    /// Initializes a substrate like address with a string representation.
    Address(const std::string& string, byte network): SS58Address(string, network) {}

    /// Initializes a substrate like address with a public key.
    Address(const PublicKey& publicKey, byte network): SS58Address(publicKey, network) {}
};

}

struct TWSubstrateAddress {
    TW::Substrate::Address impl;
};