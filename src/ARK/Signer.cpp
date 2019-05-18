// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Hash.h"
#include "../PublicKey.h"

#include <TrustWalletCore/TWPublicKeyType.h>

using namespace TW::ARK;

Proto::SigningOutput Signer::sign(PrivateKey &privateKey, Transaction &tx) {
    Data hash = Hash::sha256(tx.encoded());
    tx.signature = privateKey.signAsDER(hash, TWCurveSECP256k1);

    Proto::SigningOutput output;
    output.set_signature(tx.signature.data(), tx.signature.size());
    output.set_encoded(tx.encodedJson());

    return output;
}
