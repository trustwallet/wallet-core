// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#include "OperationList.h"
#include "Signer.h"
#include "../Hash.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWString.h>
#include <TrezorCrypto/blake2b.h>
#include <TrustWalletCore/TWCurve.h>

using namespace TW;
using namespace TW::Tezos;

Data Signer::signOperationList(const PrivateKey& privateKey, OperationList operationList) {
    auto forgedBytesHex = operationList.forge();
    return signHexString(privateKey, forgedBytesHex);
}

Data Signer::signHexString(const PrivateKey& privateKey, std::string forgedBytesHex) {
  auto watermark = "03";
  auto watermarkedForgedBytesHex = watermark + forgedBytesHex;
  auto hash = Hash::blake2b(watermarkedForgedBytesHex, 32);
  auto signature = privateKey.sign(hash, TWCurveEd25519);

  Data result = Data(watermarkedForgedBytesHex.begin(), watermarkedForgedBytesHex.end());
  Data signature_data = Data(signature.begin(), signature.end());
  append(result, signature_data);
  return result;
}
