// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTezosAddress.h>

#include <TrustWalletCore/TWPublicKey.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cstring>
#include <string>
#include <vector>
#include <array>

bool TWTezosAddressEqual(struct TWTezosAddress lhs, struct TWTezosAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, TWTezosAddressSize) == 0;
}

bool TWTezosAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TWTezosAddressSize;
}

// Decode the given base 58 string and drop the given prefix from decoded data.
// Returns output length which output data placed in the inparameter.
int checkDecodeAndDropPrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output) {
  size_t capacity = 128;
  uint8_t decodedInput[capacity];
  int decodedLength = base58_decode_check(input.data(), HASHER_SHA2D, decodedInput, (int)capacity);

  // Verify that the prefix was correct.
  for (int i = 0; i < prefixLength; i++) {
    if (decodedInput[i] != prefix[i]) {
      return 0;
    }
  }

  // Drop the prefix from branch.
  int outputLength = decodedLength - prefixLength;
  for (int i = 0; i < outputLength; i++) {
    output[i] = decodedInput[i + prefixLength];
  }

  return outputLength;
}


std::string forgePublicKeyHash(const std::string &publicKeyHash) {
  size_t prefixLength = 3;
  uint8_t prefix[prefixLength];
  prefix[0] = 6;
  prefix[1] = 161;
  prefix[2] = 159;

  size_t capacity = 128;
  uint8_t decoded[capacity];

  int decodedLength = checkDecodeAndDropPrefix(publicKeyHash, prefixLength, prefix, decoded);

  std::string result = "01"; // TODO: Hardcoded to tz1, consider expanding.
  // TODO: Append decoded here as a hex string.

  return result;
}


std::string forgeBranch(const std::string& branch) {
  size_t capacity = 128;
  uint8_t decodedBranch[capacity];

  // TODO: There must be a better way to initialize arrays.
  size_t prefixLength = 2;
  uint8_t prefix[prefixLength];
  prefix[0] = 1;
  prefix[1] = 52;
  int decodedBranchLength = checkDecodeAndDropPrefix(branch, prefixLength, prefix, decodedBranch);

  std::string result = "";
  // TODO: Hex encode decoded branch and append to result
  return result;
}

std::string forgeAddress(const std::string &address) {
  std::string result = "";
  if (address[0] == 'K') {
    size_t prefixLength = 3;
    uint8_t prefix[prefixLength];
    prefix[0] = 2;
    prefix[1] = 90;
    prefix[2] = 121;

    size_t capacity = 128;
    uint8_t decoded[capacity];

    int decodedLength = checkDecodeAndDropPrefix(address, prefixLength, prefix, decoded);

    result += "01";
    // TODO: Append decoded here as a hex string.
    result += "00";
  } else {
    result = result + "00";
    result += forgePublicKeyHash(address);
  }
  return result;
}

std::string forgeZarith(int input) {
  std::string result = "";
  while (true) {
    if (input < 128) {
      if (input < 16) {
        result += "0";
      }
      result += input; // TODO: encode input to hex.
      break;
    } else {
      int b = input % 128;
      input -= b;
      input /= 128;
      b += 128;
      result += b; // TODO: encode b to hex.
    }
  }
  return result;
}

std::string forgeBool(bool input) {
  return input ? "ff" : "00";
}

TWString * TWTezosForge() {
  // TODO: This is hardcoded to forge a transaction, support reveals.
  // TODO: Tests. Inputs are different address types (tz1, kt1) for source, dest.
  auto branch = "BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW";
  auto source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW";
  auto destination = "tz1hx8hMmmeyDBi6WJgpKwK4n5S2qAEpavx2";

  auto fee = 1272;
  auto gasLimit = 10100;
  auto storageLimit = 257;

  auto addressCounter = 30738;

  auto amount = 1;

  auto forgedBranch = forgeBranch(branch);
  auto forgedSource = forgeAddress(source);
  auto forgedFee = forgeZarith(fee);
  auto forgedAddressCounter = forgeZarith(addressCounter);
  auto forgedGasLimit = forgeZarith(gasLimit);
  auto forgedStorageLimit = forgeZarith(storageLimit);
  auto forgedAmount = forgeZarith(amount);
  auto forgedDestination = forgeAddress(destination);

  auto result = forgedBranch + forgedSource + forgedFee + forgedAddressCounter + forgedGasLimit + forgedStorageLimit + forgedAmount + forgedDestination + forgeBool(false);
  return TWStringCreateWithUTF8Bytes(result.data());
}

bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != TWTezosAddressSize) {
        return false;
    }

    return true;
}

bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != TWTezosAddressSize) {
        return false;
    }

    memcpy(address->bytes, buffer, TWTezosAddressSize);
    return true;
}

bool TWTezosAddressInitWithData(struct TWTezosAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != TWTezosAddressSize) {
        return false;
    }
    TWDataCopyBytes(data, 0, TWTezosAddressSize, address->bytes);
    return true;
}

bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey, uint8_t prefix) {
    // Zcash taddr has two prefix bytes, the first byte is the same 0x1c -> t
    address->bytes[0] = 0x1c;
    address->bytes[1] = prefix;

    auto compressed = TWPublicKeyCompressed(publicKey);
    ecdsa_get_pubkeyhash(compressed.bytes, HASHER_SHA2_RIPEMD,  address->bytes + 2);

    return true;
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress address) {
    size_t size = 0;
    b58enc(nullptr, &size, address.bytes, TWTezosAddressSize);
    size += 16;

    std::vector<char> str(size);
    base58_encode_check(address.bytes, TWTezosAddressSize, HASHER_SHA2D, str.data(), size);

    return TWStringCreateWithUTF8Bytes(str.data());
}

TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress address) {
    return TWDataCreateWithBytes(address.bytes, TWTezosAddressSize);
}
