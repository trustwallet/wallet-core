// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <memory>

#include "Signer.h"
#include "Address.h"
#include "Messages.h"

using namespace TW;
using namespace std::chrono;

namespace TW::Everscale {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    auto protoOutput = Proto::SigningOutput();

    switch (input.action_type_case()) {
    case Proto::SigningInput::ActionTypeCase::kTransfer: {
        auto bounce = input.transfer().bounce();
        auto flags = input.transfer().flags();
        auto amount = input.transfer().amount();
        auto expiredAt = input.transfer().expired_at();

        std::optional<MsgAddressInt> destination;
        if (input.transfer().has_address()) {
            auto addr = Address(input.transfer().address());
            destination = std::make_pair(addr._wc, addr._address);
        }

        std::optional<Data> stateInit;
        if (input.transfer().has_state_init()) {
            auto data = TW::data(input.transfer().state_init());
            stateInit = data;
        }

        auto signedMessage = createSignedMessage(publicKey, key, bounce, flags, amount, expiredAt, destination, stateInit);
        protoOutput.set_message(signedMessage.data(), signedMessage.size());
    } break;

    default:
        assert(input.action_type_case() != Proto::SigningInput::ActionTypeCase::ACTION_TYPE_NOT_SET);
    }

    return protoOutput;
}

}
