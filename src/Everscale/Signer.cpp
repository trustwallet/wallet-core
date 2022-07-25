// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <chrono>

#include "Signer.h"
#include "Address.h"
#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;
using namespace TW::Everscale;
using namespace std::chrono;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    switch (input.action_type_case()) {
    case Proto::SigningInput::ActionTypeCase::kTransfer:
    {
        auto bounce = input.transfer().bounce();
        auto flags = input.transfer().flags();
        auto amount = input.transfer().amount();

        InitData initData(publicKey);

        Address::MsgAddressInt destination;
        if (!input.transfer().has_address()) {
            destination = initData.computeAddr(WorkchainType::Basechain);
        } else {
            auto addr = Address(input.transfer().address());
            destination = std::make_pair(addr.wc_, addr.address_);
        }

        auto gift = Wallet::Gift {
            .bounce = bounce,
            .amount = amount,
            .destination = destination,
            .flags =  static_cast<uint8_t>(flags),
        };

        const uint32_t expiredAt = static_cast<uint32_t>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count());
        auto [hash, payload] = initData.makeTransferPayload(expiredAt, gift);

        // TODO: Message implementation

    }
    break;
    case Proto::SigningInput::ActionTypeCase::ACTION_TYPE_NOT_SET:
        break;
    }

    auto protoOutput = Proto::SigningOutput();
    return protoOutput;
}

