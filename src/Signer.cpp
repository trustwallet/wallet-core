// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Coin.h"
#include "PrivateKey.h"
#include "Signer.h"
#include "Base64.h"
#include "Cosmos/Signer.h"
#include "Binance/Signer.h"
#include "Ethereum/Signer.h"
#include "Ethereum/Address.h"
#include "proto/Ethereum.pb.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace google::protobuf;

Proto::SigningOutput Signer::sign() const noexcept {
    const auto coinType = (TWCoinType) input.network();
    const auto transaction = input.transaction();
    const auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto output = Proto::SigningOutput();

    switch (coinType)
    {
        case TWCoinTypeCosmos: {
            Cosmos::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (!output.has_error()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Cosmos::Signer(std::move(message)).build();
                output.set_json(signerOutput.json());
            }
            break;
        }
        case TWCoinTypeBinance: {
            Binance::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (!output.has_error()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Binance::Signer(std::move(message)).build();
                output.set_encoded(Base64::encode(signerOutput));
            }
            break;
        }
        case TWCoinTypeEthereum: {
            Ethereum::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (!output.has_error()) {
                auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
                auto ethTransaction = Ethereum::Transaction(
                        /* nonce: */ load(message.nonce()),
                        /* gasPrice: */ load(message.gas_price()),
                        /* gasLimit: */ load(message.gas_limit()),
                        /* to: */ Ethereum::Address(message.to_address()),
                        /* amount: */ load(message.amount()),
                        /* payload: */ Data(message.payload().begin(), message.payload().end())
                );

                auto signer = Ethereum::Signer(load(message.chain_id()));
                signer.sign(key, ethTransaction);

                auto protoOutput = Ethereum::Proto::SigningOutput();

                auto encoded = Ethereum::RLP::encode(ethTransaction);
                protoOutput.set_encoded(encoded.data(), encoded.size());

                auto v = store(ethTransaction.v);
                protoOutput.set_v(v.data(), v.size());

                auto r = store(ethTransaction.r);
                protoOutput.set_r(r.data(), r.size());

                auto s = store(ethTransaction.s);
                protoOutput.set_s(s.data(), s.size());
                auto serialized = protoOutput.SerializeAsString();

                output.set_encoded(Base64::encode(create(serialized)));
            }
        }
        default:
            auto error = new Proto::SigningOutput_Error();
            error->set_code(SignerErrorCodeNotSupported);
            error->set_description("Network not supported");
            output.set_allocated_error(error);
    }

    return output;
}

void Signer::parse(const std::string& transaction, Message* message,
                   Proto::SigningOutput& output) const noexcept {
    util::JsonParseOptions options;
    options.case_insensitive_enum_parsing = true;
    options.ignore_unknown_fields = false;

    auto result = JsonStringToMessage(transaction, message, options);

    if (!result.ok()) {
        auto error = new Proto::SigningOutput_Error();
        error->set_code(SignerErrorCodeInvalidJson);
        error->set_description(result.error_message());
        output.set_allocated_error(error);
    }
}