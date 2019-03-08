// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#include "OperationList.h"
#include "Signer.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWString.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/ed25519.h>

using namespace TW;
using namespace TW::Tezos;

Data Signer::signOperationList(const PrivateKey& privateKey, OperationList operationList) {
  auto forgedBytes = operationList.forge();
  return signHexString(privateKey, forgedBytes);
}

Data Signer::signHexString(const PrivateKey& privateKey, std::string forgedBytes) {
  auto watermark = "03";
  auto watermarkedForgedBytesHex = parse_hex(watermark + forgedBytes);
  auto hash = Hash::blake2b(watermarkedForgedBytesHex, 32);

  ed25519_public_key pk;
  ed25519_publickey(privateKey.bytes.data(), pk);

  ed25519_signature signature;
  ed25519_sign(hash.data(), hash.size(), privateKey.bytes.data(), pk, signature);

  Data result = Data(hash.begin(), hash.end());
  Data signature_data = Data(signature, signature + 64);
  append(result, signature_data);
  return signature_data;
}
