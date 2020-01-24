// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Hash.h"
#include "../PublicKey.h"
#include "../PrivateKey.h"
#include "Script.h"
#include "Signer.h"

using namespace TW;
using namespace TW::NEO;

Signer::Signer(const TW::PrivateKey &priKey) : privateKey(std::move(priKey)) {
  auto pub = privateKey.getPublicKey(TWPublicKeyTypeNIST256p1);
  publicKey = pub.bytes;
  address = Address(pub).string();
}

PrivateKey Signer::getPrivateKey() const {
  return privateKey;
}

TW::PublicKey Signer::getPublicKey() const {
  return TW::PublicKey(publicKey, TWPublicKeyTypeNIST256p1);
}

Address Signer::getAddress() const {
  return Address(address);
}

void Signer::sign(Transaction &tx) const {
  auto unsignedTx = tx.serialize();
  auto signature = sign(unsignedTx);
  tx.witnesses.clear();
  Witness witness;
  witness.invocationScript = Script::CreateInvocationScript(signature);
  witness.verificationScript = Script::CreateSignatureRedeemScript(publicKey);
  tx.witnesses.push_back(witness);
}

Data Signer::sign(const Data &data) const {
  auto signature = getPrivateKey().sign(TW::Hash::sha256(data), TWCurveNIST256p1);
  signature.pop_back();
  return signature;
}
