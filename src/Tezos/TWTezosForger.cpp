// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include <string>
#include <sstream>
#include <stdexcept>
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
      throw std::invalid_argument( "Invalid Prefix" );
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
  uint8_t decoded[capacity];
  size_t prefixLength = 2;
  uint8_t prefix[] = {1, 52};

  int decodedLength = checkDecodeAndDropPrefix(branch, prefixLength, prefix, decoded);
  return TW::hex(decoded, decoded + decodedLength);
}

// Forge the given boolean into a hex encoded string.
std::string forgeBool(bool input) {
  return input ? "ff" : "00";
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
std::string forgePublicKeyHash(const std::string &publicKeyHash) {
  std::string result = "0";
  size_t prefixLength = 3;
  uint8_t prefix[] = {6, 161, 0};
  size_t capacity = 128;
  uint8_t decoded[capacity];

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
      throw std::invalid_argument( "Invalid Prefix" );
  }
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
    uint8_t prefix[3] = {2, 90, 121};
    size_t capacity = 128;
    uint8_t decoded[capacity];

    int decodedLength = checkDecodeAndDropPrefix(address, prefixLength, prefix, decoded);
    result += "01";
    result += TW::hex(decoded, decoded + decodedLength);
    result += "00";
  } else {
    // tz1 address
    result += "00";
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
  size_t prefixLength = 4;
  uint8_t prefix[] = {13, 15, 37, 217};
  size_t capacity = 128;
  uint8_t decoded[capacity];
  int decodedLength = checkDecodeAndDropPrefix(publicKey, prefixLength, prefix, decoded);

  return "00" + TW::hex(decoded, decoded + decodedLength);
}

// Forge an operation with TransactionOperationData.
std::string forgeTransactionOperation(TW::Tezos::Proto::Operation operation) {
  if (!operation.has_transaction_operation_data()) {
    throw std::invalid_argument( "Operation does not have transaction operation data" );
  };

  auto forgedSource = forgeAddress(operation.source());
  auto forgedFee = forgeZarith(operation.fee());
  auto forgedCounter = forgeZarith(operation.counter());
  auto forgedGasLimit = forgeZarith(operation.gas_limit());
  auto forgedStorageLimit = forgeZarith(operation.storage_limit());
  auto forgedAmount = forgeZarith(operation.transaction_operation_data().amount());
  auto forgedDestination = forgeAddress(operation.transaction_operation_data().destination());

  return "08" + forgedSource + forgedFee + forgedCounter + forgedGasLimit
      + forgedStorageLimit + forgedAmount + forgedDestination + forgeBool(false);
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
      throw std::invalid_argument( "Invalid Operation Kind" );
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
