// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "../Data.h"
#include "../PublicKey.h"
#include "../HexCoding.h"


namespace TW::ARK{

  class Address{

    public:
      static byte mainnetPrefix;
      std::string address;

      static bool isValid(const Data &data);

      static bool isValid(const std::string &string);

      Address(const std::string &string);

      Address(const Data &data);

      Address(const PublicKey &publicKey);

      Address(){};

      std::string string() const;
      Data bytes();
  };
  inline bool operator==(const Address& lhs, const Address& rhs) {
      return lhs.address == rhs.address;
  }
};

// Wrapper for C interface.
struct TWARKAddress {
  TW::ARK::Address impl;
};