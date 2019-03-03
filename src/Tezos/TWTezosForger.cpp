// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>
#include <sstream>

#include <TrezorCrypto/base58.h>

#include "proto/Tezos.pb.h"

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

// Convert the given byte buffer to a hex string.
// TODO: Figure out how to use TrustCore's hex functions.
std::string hexStr(uint8_t *data, int len) {
  std::stringstream ss;
  ss<<std::hex;
  for(int i(0);i<len;++i)
    ss<<(int)data[i];
  return ss.str();
}

// Forge the given branch to a hex encoded string.
std::string forgeBranch(const std::string branch) {
  size_t capacity = 128;
  uint8_t decodedBranch[capacity];

  // TODO: There must be a better way to initialize arrays.
  size_t prefixLength = 2;
  uint8_t prefix[prefixLength];
  prefix[0] = 1;
  prefix[1] = 52;
  int decodedBranchLength = checkDecodeAndDropPrefix(branch, prefixLength, prefix, decodedBranch);

  std::string result = "";
  result += hexStr(decodedBranch, decodedBranchLength);
  return result;
}

// Forge the given boolean into a hex encoded string.
std::string forgeBool(bool input) {
  return input ? "ff" : "00";
}

// Forge the given public key hash into a hex encoded string.
// Note: This function only supports tz1 addresses.
std::string forgePublicKeyHash(const std::string &publicKeyHash) {
  size_t prefixLength = 3;
  uint8_t prefix[prefixLength];
  prefix[0] = 6;
  prefix[1] = 161;
  prefix[2] = 159;

  size_t capacity = 128;
  uint8_t decoded[capacity];

  int decodedLength = checkDecodeAndDropPrefix(publicKeyHash, prefixLength, prefix, decoded);

  std::string result = "01";
  result += hexStr(decoded, decodedLength);
  return result;
}

// Forge the given public key hash into a hex encoded string.
// Note: This function only supports tz1 and KT1 addresses.
std::string forgeAddress(const std::string address) {
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
    result += hexStr(decoded, decodedLength);
    result += "00";
  } else {
    result = result + "00";
    result += forgePublicKeyHash(address);
  }
  return result;
}

// Forge the given zarith hash into a hex encoded string.
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

// Forge an operation with TransactionOperationData.
std::string forgeTransactionOperation(TW::Tezos::Proto::Operation operation) {
  assert(operation.has_transaction_operation_data());

  auto forgedSource = forgeAddress(operation.source());
  auto forgedFee = forgeZarith(operation.fee());
  auto forgedCounter = forgeZarith(operation.counter());
  auto forgedGasLimit = forgeZarith(operation.gas_limit());
  auto forgedStorageLimit = forgeZarith(operation.storage_limit());
  auto forgedAmount = forgeZarith(operation.transaction_operation_data().amount());
  auto forgedDestination = forgeAddress(operation.transaction_operation_data().destination());

  return forgedSource + forgedFee + forgedCounter + forgedGasLimit + forgedStorageLimit + forgedAmount +
  forgedDestination + forgeBool(false);
}

// Forge an operation with RevealOperationData.
std::string forgeRevealOperation(TW::Tezos::Proto::Operation operation) {
  // TODO: Implement.
  return nullptr;
}

std::string forgeOperation(TW::Tezos::Proto::Operation operation) {
  switch (operation.kind()) {
    case TW::Tezos::Proto::Operation_OperationKind_REVEAL:
      return forgeRevealOperation(operation);
    case TW::Tezos::Proto::Operation_OperationKind_TRANSACTION:
      return forgeTransactionOperation(operation);
    default:
      assert(false);
      return nullptr;
  }
}
