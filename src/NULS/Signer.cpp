// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../BinaryCoding.h"
#include "Address.h"



using namespace TW;
using namespace TW::NULS;

static const uint64_t MIN_PRICE_PRE_1024_BYTES = 100000;

static inline uint64_t calculateFee(uint64_t size) {
  uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
  if (size % 1024 > 0) {
    fee += MIN_PRICE_PRE_1024_BYTES;
  }
  return fee;
}

/// Encodes a 48-bit little-endian value into the provided buffer.
static inline void encode48LE(uint64_t val, std::vector<uint8_t> &data) {
  data.push_back(static_cast<uint8_t>(val));
  data.push_back(static_cast<uint8_t>((val >> 8)));
  data.push_back(static_cast<uint8_t>((val >> 16)));
  data.push_back(static_cast<uint8_t>((val >> 24)));
  data.push_back(static_cast<uint8_t>((val >> 32)));
  data.push_back(static_cast<uint8_t>((val >> 40)));
}

/// Encodes a VarInt little-endian value into the provided buffer.
static inline void encodeVarIntLE(uint64_t val, std::vector<uint8_t> &data) {
  if (val < 0xFD) {
    data.push_back(static_cast<uint8_t>(val));
  } else if (val <= 0xFFFF) {
    data.push_back(static_cast<uint8_t>(0xFD));
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
  } else if (val <= 0xFFFFFFFF) {
    data.push_back(static_cast<uint8_t>(0xFE));
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
  } else if (val > 0xFFFFFFFF) {
    data.push_back(static_cast<uint8_t>(0xFF));
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 56)));
  }
}

static inline void serializerRemark(std::string remark, std::vector<uint8_t> &data) {
  encodeVarIntLE(remark.length(), data);
  std::copy(remark.begin(), remark.end(), std::back_inserter(data));
}

static inline void serializerInput(std::vector<Proto::TransactionInput> &inputs, std::vector<uint8_t> &data) {
  encodeVarIntLE(inputs.size(), data);
  for (const auto &input : inputs) {
    Data owner = parse_hex(input.from_hash());
    // Input owner is txid and index
    encodeVarIntLE((uint16_t) input.from_index(), owner);

    encodeVarIntLE(owner.size(), data);
    std::copy(owner.begin(), owner.end(), std::back_inserter(data));

    encode64LE((uint64_t) input.amount(), data);
    encode48LE((uint64_t) input.lock_time(), data);
  }
}

static inline void serializerOutput(std::vector<Proto::TransactionOutput> &outputs, std::vector<uint8_t> &data) {
  encodeVarIntLE(outputs.size(), data);
  for (const auto &output : outputs) {
    const auto &toAddress = output.to_address();
    if (!NULS::Address::isValid(toAddress)) {
      throw std::invalid_argument("Invalid address");
    }

    const auto &addr = NULS::Address(toAddress);
    encodeVarIntLE(addr.bytes.size() - 1, data);
    std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
    encode64LE((uint64_t) output.amount(), data);
    encode48LE((uint64_t) output.lock_time(), data);
  }
}

static inline Data calcTransactionDigest(Data &data) {
  Data hash1 = Hash::sha256(data);
  Data hash2 = Hash::sha256(hash1);
  return hash2;
}

static inline Data makeTransactionSignature(PrivateKey &privateKey, Data &txHash) {
  PublicKey pubKey = privateKey.getPublicKey(PublicKeyType::secp256k1);

  Data transactionSignature = Data();
  encodeVarIntLE(pubKey.bytes.size(), transactionSignature);
  std::copy(pubKey.bytes.begin(), pubKey.bytes.end(), std::back_inserter(transactionSignature));
  auto signature = privateKey.signAsDER(txHash, TWCurve::TWCurveSECP256k1);

  transactionSignature.push_back(static_cast<uint8_t>(0x00));
  encodeVarIntLE(signature.size(), transactionSignature);

  std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));
  return transactionSignature;
}

std::vector<uint8_t> Signer::sign(uint64_t timestamp) const {
  if (tx.priv_key().empty()) {
    throw std::invalid_argument("Must have private key string");
  } else if (tx.inputs_size() == 0) {
    throw std::invalid_argument("Not enough input balance to do the transaction");
  } else if (tx.outputs_size() == 0) {
    throw std::invalid_argument("There must be at least one output, something is missed");
  }
  auto priv = Address::importHexPrivateKey(tx.priv_key());

  auto data = Data();
  // Transaction Type
  encode16LE(2, data);
  // Timestamp
  encode48LE(timestamp, data);
  // Remark
  serializerRemark(tx.remark(), data);
  // txData
  encode32LE(0xffffffff, data);
  // CoinData Input
  std::vector<Proto::TransactionInput> inputs;
  std::copy(tx.inputs().begin(), tx.inputs().end(), std::back_inserter(inputs));
  serializerInput(inputs, data);
  // CoinData Output
  std::vector<Proto::TransactionOutput> outputs;
  std::copy(tx.outputs().begin(), tx.outputs().end(), std::back_inserter(outputs));
  serializerOutput(outputs, data);

  // Calc transaction hash
  Data txHash = calcTransactionDigest(data);

  auto transactionSignature = makeTransactionSignature(priv, txHash);

  encodeVarIntLE(transactionSignature.size(), data);
  std::copy(transactionSignature.begin(), transactionSignature.end(), std::back_inserter(data));

  return data;
}

uint64_t Signer::getFee(uint32_t inputCount, uint32_t outputCount, uint32_t remarkSize) {
  uint64_t size = 124 + 50 * inputCount + 38 * outputCount + remarkSize;
  return calculateFee(size);
}
