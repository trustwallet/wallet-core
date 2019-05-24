//
// Created by Leone Parise on 2019-05-24.
//

#include "Signer.h"
#include "../Cosmos/Signer.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Json;
using namespace google::protobuf::util;

enum StringSignErrorCode {
    StringSignErrorCodeNotSupported = 1,
    StringSignErrorCodeJsonParseError = 2,
};

Proto::SigningOutput Signer::sign(TWCoinType coinType, const std::string &transaction,
                                  const std::string &privateKey) const noexcept {
    auto protoOutput = Proto::SigningOutput();

    switch (coinType)
    {
        case TWCoinTypeCosmos: {
            TW::Cosmos::Proto::SigningInput input;
            parse(transaction, &input, protoOutput);
            if (!protoOutput.has_error()) {
                input.set_private_key(privateKey);
                auto output = TW::Cosmos::Signer(std::move(input)).build();
                protoOutput.set_json(output.json());
            }
            break;
        }
        default:
            auto error = Proto::SigningOutput_Error();
            error.set_code(StringSignErrorCodeNotSupported);
            error.set_description("Network not supported");
            protoOutput.set_allocated_error(&error);
    }
}

void Signer::parse(const std::string &transaction, google::protobuf::Message *message,
                   TW::Json::Proto::SigningOutput &output) const noexcept {
    JsonParseOptions options;
    options.case_insensitive_enum_parsing = true;
    options.ignore_unknown_fields = true;

    auto result = JsonStringToMessage(transaction, message, options);

    if (!result.ok()) {
        auto error = Proto::SigningOutput_Error();
        error.set_code(StringSignErrorCodeJsonParseError);
        error.set_description(result.error_message());
        output.set_allocated_error(&error);
    }
}