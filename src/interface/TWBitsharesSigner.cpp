// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitsharesSigner.h>

#include "../Bravo/Signer.h"
#include "../Bitshares/Operation.h"
#include "../proto/Bitshares.pb.h"
#include "../proto/Common.pb.h"

using namespace TW::Bitshares;

static TW_Proto_Result createErrorResult(const std::string& description) {

    auto result = TW::Proto::Result();
    result.set_success(false);
    result.set_error(description);
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWBitsharesSignerSign(TW_Bitshares_Proto_SigningInput input) {
    Proto::SigningInput in;
    bool success = in.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)));

    if (!success) {
        return createErrorResult("Error parsing the input.");
    }

    try {
        auto privateKey = TW::PrivateKey(TW::Data(in.private_key().begin(), in.private_key().end()));

        // create a memo;
        Memo *memo = nullptr;
        const std::string& memoContents = in.memo();
        if (memoContents.size()) {
            auto publicKey = TW::PublicKey(TW::Data(in.recipient_public_key().begin(), in.recipient_public_key().end()), TWPublicKeyTypeSECP256k1);
            memo = new Memo(privateKey, publicKey, memoContents);
        }

        // create a transfer operation
        Asset asset {in.asset().amount(), in.asset().asset_id()};
        Asset fee {in.fee().amount(), in.fee().asset_id()};
        auto op = new TransferOperation(in.sender_id(), in.recipient_id(), asset, fee, memo);

        // create a Transaction and add the transfer operation
        TW::Bravo::Transaction tx{ TW::Data(in.reference_block_id().begin(), in.reference_block_id().end()), 
                                in.reference_block_time() };
        tx.addOperation(op);

        // sign the transaction with a Signer
        auto chainId = TW::Data(in.chain_id().begin(), in.chain_id().end());
        TW::Bravo::Signer(chainId).sign(privateKey, tx);

        // add transaction's json encoding to Signing Output and return that
        Proto::SigningOutput out;
        out.set_json_encoded(tx.serialize().dump());

        auto result = TW::Proto::Result();
        result.set_success(true);
        result.add_objects()->PackFrom(out);
        auto serialized = result.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    } catch (const std::exception& e) {
        return createErrorResult(e.what());
    }
}