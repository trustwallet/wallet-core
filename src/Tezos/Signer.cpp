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

#include <TrustWalletCore/TWCurve.h>

using namespace TW;
using namespace TW::Tezos;

std::string Signer::signOperationList(const PrivateKey& privateKey, OperationList operationList) {
  auto forgedBytes = operationList.forge();
  return signHexString(privateKey, forgedBytes);
}

std::string Signer::signHexString(const PrivateKey& privateKey, std::string forgedBytes) {
  auto watermark = "03";
  auto watermarkedForgedBytesHex = parse_hex(watermark + forgedBytes);
  auto hash = Hash::blake2b(watermarkedForgedBytesHex, 32);
  TW::PublicKey pk = privateKey.getPublicKey(PublicKeyType::ed25519);
  Data signature = privateKey.sign(hash, TWCurve::TWCurveEd25519);

  return forgedBytes + hex(signature);
}
