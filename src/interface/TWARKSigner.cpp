// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../ARK/Address.h"
#include "../ARK/Signer.h"
#include "../ARK/Transaction.h"
#include "../proto/ARK.pb.h"
#include "../uint256.h"
#include <TrustWalletCore/TWARKSigner.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;
using namespace TW::ARK;

TW_ARK_Proto_SigningOutput TWARKSignerSign(TW_ARK_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto to = Address(input.to_address());

    auto tx = Transaction(
        /* type */ static_cast<uint8_t>(input.type()),
        /* amount */ input.amount(),
        /* fee */ input.fee(),
        /* timestamp */ input.timestamp(),
        /* to */ to,
        /* publicKey*/ pubKey.bytes
    );

    auto output = Signer::sign(key, tx);

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
