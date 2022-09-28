// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Hash.h"
#include "MoveTypes.h"
#include "TransactionBuilder.h"
#include "TransactionPayload.h"

namespace TW::Aptos {

TransactionPayload transferPayload(const Proto::SigningInput& input) {
    auto& tf = input.transfer();
    ModuleId module(gAddressOne, "coin");
    BCS::Serializer aSerializer;
    aSerializer << Address(tf.to());
    std::vector<Data> args;
    args.emplace_back(aSerializer.bytes);
    aSerializer.clear();
    aSerializer << tf.amount();
    args.emplace_back(aSerializer.bytes);
    TransactionPayload payload = EntryFunction(module, "transfer", {gTransferTag}, args);
    return payload;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto protoOutput = Proto::SigningOutput();
    auto payloadFunctor = [&input]() {
        switch (input.transaction_payload_case()) {
        case Proto::SigningInput::kTransfer: {
            return transferPayload(input);
        }
        case Proto::SigningInput::TRANSACTION_PAYLOAD_NOT_SET:
            throw std::runtime_error("Transaction payload should be set");
        }
    };
    TransactionBuilder::builder()
        .sender(Address(input.sender()))
        .sequenceNumber(input.sequence_number())
        .payload(payloadFunctor())
        .maxGasAmount(input.max_gas_amount())
        .gasUnitPrice(input.gas_unit_price())
        .expirationTimestampSecs(input.expiration_timestamp_secs())
        .chainId(input.chain_id())
        .sign(input, protoOutput);
    return protoOutput;
}

} // namespace TW::Aptos
