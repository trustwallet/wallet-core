// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "Base64.h"

#include "TheOpenNetwork/wallet/WalletV4R2.h"
#include "WorkchainType.h"

namespace TW::TheOpenNetwork {

static bool isBounceable(const Address& address, const Proto::BounceBehavior& bounceBehavior) {
    switch (bounceBehavior) {
    case Proto::BounceBehavior::BOUNCEABLE:
        return true;
    case Proto::NON_BOUNCEABLE:
        return false;
    case Proto::ADDRESS_DEFAULT:
    default:
        return address.isBounceable;
    }
}

Data Signer::createTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::Transfer& transfer) {
    return createTransferMessage(wallet, privateKey, transfer, nullptr);
}

// TANGEM
Data Signer::createTransferMessage(std::shared_ptr<Wallet> wallet, const PrivateKey& privateKey, const Proto::Transfer& transfer, const std::function<Data(Data)> externalSigner) {
    const Address address = Address(transfer.dest());
    const bool isBounceable = TW::TheOpenNetwork::isBounceable(address, transfer.bounce_behavior());
    
    const auto msg = wallet->createTransferMessage(
        privateKey,
        externalSigner,
        address,
        isBounceable,
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

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    const auto& privateKey = PrivateKey(input.private_key());
    const auto& publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    return Signer::sign(input, publicKey.bytes, nullptr);
}

// TANGEM
Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, const Data& publicKeyData, const std::function<Data(Data)> externalSigner) noexcept {
    const auto& privateKey = PrivateKey(input.private_key());
    const PublicKey publicKey(publicKeyData, TWPublicKeyTypeED25519);
    
    auto protoOutput = Proto::SigningOutput();

    switch (input.action_oneof_case()) {
    case Proto::SigningInput::ActionOneofCase::kTransfer: {
        const auto& transfer = input.transfer();

        try {
            switch (transfer.wallet_version()) {
            case Proto::WalletVersion::WALLET_V4_R2: {
                const int8_t workchainId = WorkchainType::Basechain;
                auto wallet = std::make_shared<WalletV4R2>(publicKey, workchainId);
                const auto& transferMessage = Signer::createTransferMessage(wallet, privateKey, transfer, externalSigner);
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
    default:
        break;
    }
    return protoOutput;
}

} // namespace TW::TheOpenNetwork
