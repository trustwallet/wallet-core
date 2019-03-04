// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.



#include <array>
#include <vector>
#include <gtest/gtest.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWTezosAddress.h>
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

TEST(TWTezosAddress, TestDescription) {
  std::string address = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don";
  TWString *tezosAddressString = TWStringCreateWithUTF8Bytes(address.c_str());
  TWTezosAddress *tezosAddress;
  TWTezosAddressInitWithString(tezosAddress, tezosAddressString);
  ASSERT_EQ(TWTezosAddressDescription(tezosAddress), tezosAddressString);
}

TEST(TWTezosAddress, TestValidAddresses) {
  std::array<std::string, 3> validAddresses { 
    "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don"
  };
   
  for (std::string address : validAddresses) {
    TWString *tezosAddressString = TWStringCreateWithUTF8Bytes(address.c_str());
    TWTezosAddress *tezosAddress;
    ASSERT_TRUE(TWTezosAddressIsValidString(tezosAddressString));    
    ASSERT_TRUE(TWTezosAddressInitWithString(tezosAddress, tezosAddressString));
  }
}

TEST(TWTezosAddress, TestInalidAddresses) {
  std::array<std::string, 3> invalidAddresses { 
    "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt", // Invalid prefix, valid checksum
    "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA", // Valid prefix, invalid checksum
    "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"  // Invalid prefix, invalid checksum
  };
   
  for (std::string address : invalidAddresses) {
    TWString *tezosAddressString = TWStringCreateWithUTF8Bytes(address.c_str());
    TWTezosAddress *tezosAddress;
    ASSERT_FALSE(TWTezosAddressIsValidString(tezosAddressString));    
    ASSERT_FALSE(TWTezosAddressInitWithString(tezosAddress, tezosAddressString));
  }
}

TEST(TWTezosAddress, TestAddressFromPrivateKey) {
  std::string expectedAddress = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
  TWString *expectedAddressString = TWStringCreateWithUTF8Bytes(expectedAddress.c_str());
  
  std::string privateKeyHex= "2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54";
  TWString *privateKeyString = TWStringCreateWithUTF8Bytes(privateKeyHex.c_str());
  TWData *privateKeyData = TWDataCreateWithHexString(privateKeyString);
  TWPrivateKey *privateKey = TWPrivateKeyCreateWithData(privateKeyData);
  TWPublicKey publicKey = TWPrivateKeyGetPublicKey(privateKey, false);
  
  TWTezosAddress *tezosAddress;
  ASSERT_TRUE(TWTezosAddressInitWithPublicKey(tezosAddress, publicKey));
  ASSERT_EQ(TWTezosAddressDescription(tezosAddress), expectedAddressString);
}
