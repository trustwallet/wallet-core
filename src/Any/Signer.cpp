// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Coin.h"
#include "PrivateKey.h"
#include "Signer.h"
#include "HexCoding.h"
#include "Cosmos/Signer.h"
#include "Binance/Signer.h"
#include "Ethereum/Signer.h"
#include "Tezos/Signer.h"

#include <string>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace google::protobuf;

Any::Proto::SigningOutput Any::Signer::sign() const noexcept {
    const auto coinType = (TWCoinType) input.coin_type();
    const auto transaction = input.transaction();
    const auto privateKeyData = parse_hex(input.private_key());
    const auto privateKey = PrivateKey(privateKeyData);

    auto output = Any::Proto::SigningOutput();

    switch (coinType)
    {
        case TWCoinTypeCosmos: {
            Cosmos::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Cosmos::Signer(std::move(message)).build();
                output.set_output(signerOutput.json());
            }
            break;
        }
        case TWCoinTypeBinance: {
            Binance::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Binance::Signer(std::move(message)).build();
                output.set_output(hex(signerOutput.begin(), signerOutput.end()));
            }
            break;
        }
        case TWCoinTypeEthereum: {
            Ethereum::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Ethereum::Signer(load(message.chain_id())).sign(message);
                auto encoded = signerOutput.encoded();
                output.set_output(hex(encoded.begin(), encoded.end()));
            }
            break;
        }
        case TWCoinTypeTezos: {
            Tezos::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto operations = message.operation_list().operations();
                auto operation_list = Tezos::OperationList(message.operation_list().branch());
                operation_list.operation_list.assign(operations.begin(), operations.end());
                auto signerOutput = Tezos::Signer().signOperationList(privateKey, operation_list);
                output.set_output(hex(signerOutput.begin(), signerOutput.end()));
            }
            break;
        }
        default:
            auto error = new Proto::SigningOutput_Error();
            error->set_code(SignerErrorCodeNotSupported);
            error->set_description("Network not supported");
            output.set_allocated_error(error);
    }

    return output;
}

void Any::Signer::parse(const std::string& transaction, Message* message,
                            Any::Proto::SigningOutput& output) const noexcept {
    util::JsonParseOptions options;
    options.case_insensitive_enum_parsing = true;
    options.ignore_unknown_fields = false;

    auto result = JsonStringToMessage(transaction, message, options);

    if (result.ok()) {
        output.set_success(true);
        return;
    }

    auto error = new Any::Proto::SigningOutput_Error();
    error->set_code(SignerErrorCodeInvalidJson);
    error->set_description(result.error_message());
    output.set_allocated_error(error);
}
