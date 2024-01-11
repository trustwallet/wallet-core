// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"

#include "Base64.h"

#include "TheOpenNetwork/wallet/WalletV4R2.h"
#include "TheOpenNetwork/Payloads.h"
#include "WorkchainType.h"

namespace TW::TheOpenNetwork {
    
Data Signer::createTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::Transfer& transfer) {
    const auto msg = wallet->createTransferMessage(
        privateKey,
        Address(transfer.dest(), transfer.bounceable()),
        transfer.amount(),
        transfer.sequence_number(),
        static_cast<uint8_t>(transfer.mode()),
        transfer.expire_at(),
        transfer.comment()
    );

    Data result{};
    msg->serialize(result);
    return result;
}

Data Signer::createJettonTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::JettonTransfer& jettonTransfer) {
    const Proto::Transfer& transferData = jettonTransfer.transfer();
    
    const auto payload = jettonTransferPayload(
        Address(jettonTransfer.response_address()),
        Address(jettonTransfer.to_owner()),
        jettonTransfer.jetton_amount(),
        jettonTransfer.forward_amount(),
        transferData.comment(),
        jettonTransfer.query_id()
    );
    
    const auto msg = wallet->createQueryMessage(
        privateKey,
        Address(transferData.dest(), transferData.bounceable()),
        transferData.amount(),
        transferData.sequence_number(),
        static_cast<uint8_t>(transferData.mode()),
        payload,
        transferData.expire_at()
    );

    Data result{};
    msg->serialize(result);
    return result;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    const auto& privateKey = PrivateKey(input.private_key());
    const auto& publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    auto protoOutput = Proto::SigningOutput();

    switch (input.action_oneof_case()) {
    case Proto::SigningInput::ActionOneofCase::kTransfer: {
        const auto& transfer = input.transfer();

        try {
            switch (transfer.wallet_version()) {
            case Proto::WalletVersion::WALLET_V4_R2: {
                const int8_t workchainId = WorkchainType::Basechain;
                auto wallet = std::make_shared<WalletV4R2>(publicKey, workchainId);
                const auto& transferMessage = Signer::createTransferMessage(wallet, privateKey, transfer);
                protoOutput.set_encoded(TW::Base64::encode(transferMessage));
                break;
            }
            default:
                protoOutput.set_error(Common::Proto::Error_invalid_params);
                protoOutput.set_error_message("Unsupported wallet version");
                break;
            }
        } catch (...) { }
        break;
    }
    case Proto::SigningInput::ActionOneofCase::kJettonTransfer: {
        const auto& jettonTransfer = input.jetton_transfer();
        try {
            switch (jettonTransfer.transfer().wallet_version()) {
            case Proto::WalletVersion::WALLET_V4_R2: {
                const int8_t workchainId = WorkchainType::Basechain;
                auto wallet = std::make_shared<WalletV4R2>(publicKey, workchainId);
                const auto& transferMessage = Signer::createJettonTransferMessage(wallet, privateKey, jettonTransfer);
                protoOutput.set_encoded(TW::Base64::encode(transferMessage));
                break;
            }
            default:
                protoOutput.set_error(Common::Proto::Error_invalid_params);
                protoOutput.set_error_message("Unsupported wallet version");
                break;
            }
        } catch (...) { }
    }
    default:
        break;
    }
    return protoOutput;
}

} // namespace TW::TheOpenNetwork
