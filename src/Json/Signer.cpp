// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Coin.h"
#include "PrivateKey.h"
#include "Signer.h"
#include "../Cosmos/Signer.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Json;
using namespace google::protobuf;

Proto::SigningOutput Signer::sign() const noexcept {
    const auto coinType = (TWCoinType) input.network();
    const auto transaction = input.transaction();
    const auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto output = Proto::SigningOutput();

    switch (coinType)
    {
        case TWCoinTypeCosmos: {
            TW::Cosmos::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (!output.has_error()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = TW::Cosmos::Signer(std::move(message)).build();
                output.set_json(signerOutput.json());
            }
            break;
        }
        default:
            auto error = new Proto::SigningOutput_Error();
            error->set_code(JsonSignErrorCodeNotSupported);
            error->set_description("Network not supported");
            output.set_allocated_error(error);
    }

    return output;
}

void Signer::parse(const std::string& transaction, Message* message,
                   Proto::SigningOutput& output) const noexcept {
    util::JsonParseOptions options;
    options.case_insensitive_enum_parsing = true;
    options.ignore_unknown_fields = true;

    auto result = JsonStringToMessage(transaction, message, options);

    if (!result.ok()) {
        auto error = new Proto::SigningOutput_Error();
        error->set_code(JsonSignErrorCodeJsonParseError);
        error->set_description(result.error_message());
        output.set_allocated_error(error);
    }
}