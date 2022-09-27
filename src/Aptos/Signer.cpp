// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "MoveTypes.h"
#include "TransactionPayload.h"
#include "../PublicKey.h"

namespace TW::Aptos {

Proto::SigningOutput Signer::sign([[maybe_unused]] const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    BCS::Serializer serializer;
    if (!input.sender().empty()) {
        serializer << Address(input.sender());
    }
    if (input.sequence_number() > 0) {
        serializer << input.sequence_number();
    }
    switch (input.transaction_payload_case()) {
    case Proto::SigningInput::kTransfer: {
        auto& tf = input.transfer();
        ModuleId module(gAddressOne, "coin");
        BCS::Serializer aSerializer;
        aSerializer << Address(tf.to());
        std::vector<Data> args;
        args.emplace_back(serializer.bytes);
        aSerializer.clear();
        aSerializer << tf.amount();
        args.emplace_back(serializer.bytes);
        TransactionPayload payload = EntryFunction(module, "transfer", {gTransferTag}, args);
        serializer << payload;
        break;
    }
    case Proto::SigningInput::TRANSACTION_PAYLOAD_NOT_SET:
        break;
    }
    serializer << input.max_gas_amount() << input.gas_unit_price() << input.expiration_timestamp_secs() << std::uint8_t(input.chain_id());
    // TODO: do the signing part?
    encoded = serializer.bytes;
    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

} // namespace TW::Aptos
