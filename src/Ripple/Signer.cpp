// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include <limits>

using namespace TW;
using namespace TW::Ripple;

static const int64_t fullyCanonical = 0x80000000;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();

    const int64_t tag = input.destination_tag();
    if (tag > std::numeric_limits<uint32_t>::max() || tag < 0) {
        output.set_error(Common::Proto::SigningError::Error_invalid_memo);
        return output;
    }

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        /* amount */input.amount(),
        /* fee */input.fee(),
        /* flags */input.flags(),
        /* sequence */input.sequence(),
        /* last_ledger_sequence */input.last_ledger_sequence(),
        /* account */Address(input.account()),
        /* destination */input.destination(),
        /* destination_tag*/tag
    );

    auto signer = Signer();
    signer.sign(key, transaction);

    auto encoded = transaction.serialize();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
    /// See https://github.com/trezor/trezor-core/blob/master/src/apps/ripple/sign_tx.py#L59
    transaction.flags |= fullyCanonical;
    transaction.pub_key = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1).bytes;

    auto unsignedTx = transaction.getPreImage();
    auto hash = Hash::sha512(unsignedTx);
    auto half = Data(hash.begin(), hash.begin() + 32);

    transaction.signature = privateKey.signAsDER(half, TWCurveSECP256k1);
}
