// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "PrivateKey.h"
#include "Signer.h"
#include "HexCoding.h"
#include "Cosmos/Signer.h"
#include "Binance/Signer.h"
#include "Ethereum/Signer.h"
#include "Tezos/Signer.h"
#include "IoTeX/Signer.h"
#include "Wanchain/Signer.h"
#include "Waves/Signer.h"
#include "Nebulas/Signer.h"
#include "Tron/Signer.h"
#include "VeChain/Signer.h"

#include <string>
#include <google/protobuf/util/json_util.h>

#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace google::protobuf;

Any::Proto::SigningOutput Any::Signer::sign() const noexcept {
    const auto coinType = (TWCoinType) input.coin_type();
    const auto transaction = input.transaction();
    const auto privateKey = PrivateKey(parse_hex(input.private_key()));

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
        case TWCoinTypeTomoChain:
        case TWCoinTypeCallisto:
        case TWCoinTypeThunderToken:
        case TWCoinTypePOANetwork:
        case TWCoinTypeEthereumClassic:
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
        case TWCoinTypeIoTeX: {
            IoTeX::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_privatekey(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = IoTeX::Signer(std::move(message)).build();
                auto encoded = signerOutput.encoded();
                output.set_output(hex(encoded.begin(), encoded.end()));
            }
            break;
        }
        case TWCoinTypeWanchain: {
            Ethereum::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                auto ethTransaction = Ethereum::Transaction(
                        /* nonce: */ load(message.nonce()),
                        /* gasPrice: */ load(message.gas_price()),
                        /* gasLimit: */ load(message.gas_limit()),
                        /* to: */ Ethereum::Address(message.to_address()),
                        /* amount: */ load(message.amount()),
                        /* payload: */ Data(message.payload().begin(), message.payload().end())
                );
                auto signer = Wanchain::Signer(load(message.chain_id()));
                signer.sign(privateKey, ethTransaction);
                auto encoded = signer.encode(ethTransaction);
                output.set_output(hex(encoded.begin(), encoded.end()));
            }
            break;
        }
        case TWCoinTypeWaves: {
            Waves::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
                auto wavesTransaction = Waves::Transaction(
                        /* amount */ message.amount(),
                        /* asset */ message.asset(),
                        /* fee */ message.fee(),
                        /* fee_asset */ message.fee_asset(),
                        /* to */ Waves::Address(message.to()),
                        /* attachment */ Data(message.attachment().begin(), message.attachment().end()),
                        /* timestamp */ message.timestamp(),
                        /* pub_key */ publicKey.bytes
                );
                auto signature = Waves::Signer::sign(privateKey, wavesTransaction);
                auto jsonOutput = wavesTransaction.buildJson(signature).dump();
                output.set_output(jsonOutput);
            }
            break;
        }
        case TWCoinTypeNebulas: {
            Nebulas::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Nebulas::Signer(load(message.chain_id())).sign(message);
                auto signature = signerOutput.signature();
                output.set_output(hex(signature.begin(), signature.end()));
            }
            break;
        }
        case TWCoinTypeTron: {
            Tron::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = Tron::Signer::sign(message);
                auto signature = signerOutput.signature();
                output.set_output(hex(signature.begin(), signature.end()));
            }
            break;
        }
        case TWCoinTypeVeChain: {
            VeChain::Proto::SigningInput message;
            parse(transaction, &message, output);
            if (output.success()) {
                message.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
                auto signerOutput = VeChain::Signer::sign(message);
                auto encoded = signerOutput.encoded();
                output.set_output(hex(encoded.begin(), encoded.end()));
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

void Any::Signer::toJson(const google::protobuf::Message &message, std::string *json_string) const  noexcept {
    util::JsonPrintOptions options;
    options.preserve_proto_field_names = true;

    MessageToJsonString(message, json_string, options);
}