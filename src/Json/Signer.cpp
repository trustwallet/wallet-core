//
// Created by Leone Parise on 2019-05-24.
//

#include "Signer.h"
#include "Base64.h"
#include "../Cosmos/Signer.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Json;
using namespace google::protobuf::util;

Proto::SigningOutput Signer::sign(TWCoinType coinType, const std::string &transaction,
                                  const std::string &privateKey) const noexcept {
    auto output = Proto::SigningOutput();
    auto privateKeyData = TW::Base64::decode(privateKey);

    switch (coinType)
    {
        case TWCoinTypeCosmos: {
            TW::Cosmos::Proto::SigningInput input;
            parse(transaction, &input, output);
            if (!output.has_error()) {
                input.set_private_key(privateKeyData.data(), privateKeyData.size());
                auto signerOutput = TW::Cosmos::Signer(std::move(input)).build();
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

void Signer::parse(const std::string &transaction, google::protobuf::Message *message,
                   TW::Json::Proto::SigningOutput &output) const noexcept {
    JsonParseOptions options;
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