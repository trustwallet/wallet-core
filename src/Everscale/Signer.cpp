// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Address.h"
#include "Messages.h"
#include "Wallet.h"

#include "Base64.h"

namespace TW::Everscale {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    auto protoOutput = Proto::SigningOutput();

    switch (input.action_oneof_case()) {
    case Proto::SigningInput::ActionOneofCase::kTransfer: {
        const auto& transfer = input.transfer();

        uint8_t flags;
        switch (transfer.behavior()) {
        case Proto::MessageBehavior::SendAllBalance: {
            flags = Wallet::sendAllBalanceFlags;
            break;
        }
        default: {
            flags = Wallet::simpleTransferFlags;
            break;
        }
        }

        Cell::Ref contractData{};
        switch (transfer.account_state_oneof_case()) {
        case Proto::Transfer::AccountStateOneofCase::kEncodedContractData: {
            contractData = Cell::fromBase64(transfer.encoded_contract_data());
            break;
        }
        default:
            break;
        }

        auto signedMessage = createSignedMessage(
            publicKey,
            key,
            transfer.bounce(),
            flags,
            transfer.amount(),
            transfer.expired_at(),
            Address(transfer.to()),
            contractData);
        Data result{};
        Message(signedMessage).intoCell()->serialize(result);

        protoOutput.set_encoded(TW::Base64::encode(result));
        break;
    }
    default:
        break;
    }

    return protoOutput;
}

} // namespace TW::Everscale
