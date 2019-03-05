// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include <string>
#include <sstream>
#include <array>
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
  std::copy(decodedInput + prefixLength, decodedInput + prefixLength + outputLength, output);

  return outputLength;
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

  return TW::hex(decodedBranch, decodedBranch + decodedBranchLength);
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

  std::string result = "0";

  // Adjust prefix based on tz1, tz2 or tz3.
  switch (publicKeyHash[2]) {
    case '1':
      result += "0";
      prefix[2] = 159;
      break;
    case '2':
      result += "1";
      prefix[2] = 161;
      break;
    case '3':
      result += "2";
      prefix[2] = 164;
      break;
    default:
      assert(false);
      return nullptr;
  }

  size_t capacity = 128;
  uint8_t decoded[capacity];
  int decodedLength = checkDecodeAndDropPrefix(publicKeyHash, prefixLength, prefix, decoded);

  result += TW::hex(decoded, decoded + decodedLength);
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
    result += TW::hex(decoded, decoded + decodedLength);
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
      std::stringstream ss;
      ss << std::hex << input;
      result += ss.str();
      break;
    } else {
      int b = input % 128;
      input -= b;
      input /= 128;
      b += 128;
      std::stringstream ss;
      ss << std::hex << b;
      result += ss.str();
    }
  }
  return result;
}

// Forge the given public key into a hex encoded string.
std::string forgePublicKey(std::string publicKey) {
  std::string result = "00";

  size_t prefixLength = 4;
  uint8_t prefix[prefixLength];
  prefix[0] = 13;
  prefix[1] = 15;
  prefix[2] = 37;
  prefix[3] = 217;

  size_t capacity = 128;
  uint8_t decoded[capacity];
  int decodedLength = checkDecodeAndDropPrefix(publicKey, prefixLength, prefix, decoded);
  result += TW::hex(decoded, decoded + decodedLength);
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

  return "08" + forgedSource + forgedFee + forgedCounter + forgedGasLimit + forgedStorageLimit + forgedAmount +
  forgedDestination + forgeBool(false);
}

// Forge an operation with RevealOperationData.
std::string forgeRevealOperation(TW::Tezos::Proto::Operation operation) {
  auto forgedSource = forgeAddress(operation.source());
  auto forgedFee = forgeZarith(operation.fee());
  auto forgedCounter = forgeZarith(operation.counter());
  auto forgedGasLimit = forgeZarith(operation.gas_limit());
  auto forgedStorageLimit = forgeZarith(operation.storage_limit());
  auto forgedPublicKey = forgePublicKey(operation.reveal_operation_data().public_key());

  return "07" + forgedSource + forgedFee + forgedCounter + forgedGasLimit + forgedStorageLimit + forgedPublicKey;
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

std::string forgeOperationList(TW::Tezos::Proto::OperationList operationList) {
  std::string result = forgeBranch(operationList.branch());

  for (int i = 0; i < operationList.operations_size(); i++) {
    TW::Tezos::Proto::Operation operation = operationList.operations(i);
    result += forgeOperation(operation);
  }
  return result;
}
