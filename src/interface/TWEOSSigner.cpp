// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEOSSigner.h>

#include "../EOS/Signer.h"
#include "../EOS/PackedTransaction.h"
#include "../proto/EOS.pb.h"
#include "../proto/Common.pb.h"

using namespace TW::EOS;

static TW_Proto_Result createErrorResult(const std::string& description) {

    auto result = TW::Proto::Result();
    result.set_success(false);
    result.set_error(description);
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWEOSSignerSign(TW_EOS_Proto_SigningInput input) {
    Proto::SigningInput in;
    bool success = in.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)));

    if (!success) {
        return createErrorResult("Error parsing the input.");
    }

    try {
        // create an asset object
        auto assetData = in.asset();

        if (assetData.decimals() > TW::Bravo::Asset::maxDecimals) {
            return createErrorResult("Max decimal places supported in an asset: " 
                                        + std::to_string(TW::Bravo::Asset::maxDecimals));
        }

        TW::Bravo::Asset asset{assetData.amount(), static_cast<uint8_t>(assetData.decimals()), assetData.symbol()};

        // create a transfer action
        TransferAction action {in.currency(), in.sender(), in.recipient(), asset, in.memo()};

        // create a Transaction and add the transfer action
        Transaction tx{ TW::Data(in.reference_block_id().begin(), in.reference_block_id().end()), 
                        in.reference_block_time() };
        tx.actions.push_back(action);

        // get key type
        TW::EOS::Type type = Type::Legacy;
        switch (in.private_key_type()) {
        case Proto::KeyType::LEGACY:
            type = Type::Legacy;
            break;

        case Proto::KeyType::MODERNK1:
            type = Type::ModernK1;
            break;

        case Proto::KeyType::MODERNR1:
            type = Type::ModernR1;
            break;
        default: break;
        }

        // sign the transaction with a Signer
        auto key = TW::PrivateKey(TW::Data(in.private_key().begin(), in.private_key().end()));
        auto chainId = TW::Data(in.chain_id().begin(), in.chain_id().end());
        Signer(chainId).sign(key, type, tx);

        // Pack the transaction and add the json encoding to Signing Output
        PackedTransaction ptx {tx, CompressionType::None};

        Proto::SigningOutput out;
        out.set_json_encoded(ptx.serialize().dump());

        auto result = TW::Proto::Result();
        result.set_success(true);
        result.add_objects()->PackFrom(out);
        auto serialized = result.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    } catch (const std::exception& e) {
        return createErrorResult(e.what());
    } catch (const std::logic_error& e) {
        return createErrorResult(e.what());
    } catch (const std::runtime_error& e) {
        return createErrorResult(e.what());
    }
}
