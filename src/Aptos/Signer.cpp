// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "MoveTypes.h"
#include "TransactionPayload.h"
#include "Hash.h"
#include "../PublicKey.h"

namespace TW::Aptos {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto protoOutput = Proto::SigningOutput();
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
        args.emplace_back(aSerializer.bytes);
        aSerializer.clear();
        aSerializer << tf.amount();
        args.emplace_back(aSerializer.bytes);
        TransactionPayload payload = EntryFunction(module, "transfer", {gTransferTag}, args);
        serializer << payload;
        break;
    }
    case Proto::SigningInput::TRANSACTION_PAYLOAD_NOT_SET:
        break;
    }
    serializer << input.max_gas_amount() << input.gas_unit_price() << input.expiration_timestamp_secs() << std::uint8_t(input.chain_id());
    auto msgToSign = TW::Hash::sha3_256(gAptosSalt.data(), gAptosSalt.size());
    append(msgToSign, serializer.bytes);
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto signature = privateKey.sign(msgToSign, TWCurveED25519);
    protoOutput.set_raw_txn(serializer.bytes.data(), serializer.bytes.size());
    auto pubKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    protoOutput.mutable_authenticator()->set_public_key(pubKeyData.data(), pubKeyData.size());
    protoOutput.mutable_authenticator()->set_signature(signature.data(), signature.size());

    serializer << BCS::uleb128{.value = 0} << pubKeyData << signature;
    protoOutput.set_signed_tx(serializer.bytes.data(), serializer.bytes.size());
    return protoOutput;
}

} // namespace TW::Aptos
