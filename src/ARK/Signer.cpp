// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Signer.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../Hash.h"
#include <TrustWalletCore/TWPublicKeyType.h>

using namespace TW;

void TW::ARK::Signer::sign(PrivateKey &pk, Transaction &tx){
    PublicKey pub = pk.getPublicKey(TWPublicKeyTypeSECP256k1);
    tx.publicKey = pub.bytes;
    Data hash = Hash::sha256(tx.encoded());
    tx.signature = pk.signAsDER(hash, TWCurveSECP256k1);
}
