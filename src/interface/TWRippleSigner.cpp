// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWRippleSigner.h>

#include "../Ripple/Signer.h"
#include "../proto/Ripple.pb.h"

using namespace TW;
using namespace TW::Ripple;

TW_Ripple_Proto_SigningOutput TWRippleSignerSign(TW_Ripple_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        /* amount */input.amount(),
        /* fee */input.fee(),
        /* flags */input.flags(),
        /* sequence */input.sequence(),
        /* last_ledger_sequence */input.last_ledger_sequence(),
        /* account */Address(input.account()),
        /* destination */input.destination(),
        /* destination_tag*/input.destination_tag()
    );

    auto signer = Signer();
    signer.sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
