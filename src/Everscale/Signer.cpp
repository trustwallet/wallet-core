// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <chrono>
#include <memory>

#include "Signer.h"
#include "Address.h"
#include "Messages.h"
#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;
using namespace TW::Everscale;
using namespace std::chrono;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    auto protoOutput = Proto::SigningOutput();

    switch (input.action_type_case()) {
        case Proto::SigningInput::ActionTypeCase::kTransfer:
            {
                auto bounce = input.transfer().bounce();
                auto flags = input.transfer().flags();
                auto amount = input.transfer().amount();

                auto getInitData = [&](){
                    if (input.transfer().has_state_init()) {
                        auto data = TW::data(input.transfer().state_init());
                        auto cell = Cell::deserialize(data.data(), data.size());
                        auto cellSlice = CellSlice(cell.get());
                        return std::make_pair(InitData(cellSlice), true);
                    } else {
                        return std::make_pair(InitData(publicKey), false);
                    }
                };

                auto getDstAddress = [&](InitData& initData) {
                    if (!input.transfer().has_address()) {
                        return initData.computeAddr(WorkchainType::Basechain);
                    } else {
                        auto addr = Address(input.transfer().address());
                        return std::make_pair(addr.wc_, addr.address_);
                    }
                };

                auto [initData, withInitState] = getInitData();
                auto destination = getDstAddress(initData);

                auto gift = Wallet::Gift {
                    .bounce = bounce,
                    .amount = amount,
                    .destination = destination,
                    .flags =  static_cast<uint8_t>(flags),
                };

                const uint32_t expiredAt = static_cast<uint32_t>(duration_cast<seconds>(system_clock::now().time_since_epoch()).count());
                auto [hash, payload] = initData.makeTransferPayload(expiredAt, gift);

                auto dst = InitData(publicKey).computeAddr(0);
                Message::HeaderRef header = std::make_shared<ExternalInboundMessageHeader>(dst);
                auto message = Message(header);

                if (withInitState) {
                    message.setStateInit(initData.makeStateInit());
                }

                Data data(hash.begin(), hash.end());
                auto signature = key.sign(data, TWCurveED25519);

                payload.prependRaw(signature, signature.size() * 8);

                auto body = CellSlice(payload.intoCell().get());
                message.setBody(body);

                Data serializedMessage;
                message.intoCell()->serialize(serializedMessage);

                protoOutput.set_message(serializedMessage.data(), serializedMessage.size());
            }
            break;

        default:
            assert(input.action_type_case() != Proto::SigningInput::ActionTypeCase::ACTION_TYPE_NOT_SET);

    }

    return protoOutput;
}

