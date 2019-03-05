// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <string>

#include "Signer.h"
#include "proto/Tezos.pb.h"
#include "Tezos/TWTezosForger.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWString.h>

using namespace TW;
using namespace TW::Tezos;

std::string Signer::signOperation(TW::Tezos::Proto::OperationList operationList) {
  auto forgedBytesHex = forgeBranch(operationList.branch());
  for (auto operation : operationList.operations()) {
    forgedBytesHex += forgeOperation(operation);
  }

  auto watermark = "03";
  auto watermarkedForgedBytesHex = watermark + forgedBytesHex;
  auto twStringRep = TWStringCreateWithUTF8Bytes("TODO: Put watermarekdForgedBytesHexHere");
  auto watermarkedBytes = TWDataCreateWithHexString(twStringRep);
  auto hashedBytes = TWHashBlake2b(watermarkedBytes, 32);

  // TODO: Figure out how PriveKey is injected, and sign.

  return forgedBytesHex;
}
