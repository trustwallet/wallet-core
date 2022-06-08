// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "../proto/TransactionCompiler.pb.h"
#include "Address.h"
#include "Coin.h"
#include "Signer.h"

using namespace TW::Binance;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    Address addr;
    if (!Address::decode(address, addr)) {
        return Data();
    }
    return addr.getKeyHash();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const { 
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            Signer signer(input);

            auto preImageHash = signer.preImageHash();
            auto preImage = signer.signaturePreimage();
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }
            if (signatures.size() > 1 || publicKeys.size() > 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }
            output = Signer(input).compile(signatures[0], publicKeys[0]);
        });
}

Data Entry::buildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId) const {
    auto input = Proto::SigningInput();
    input.set_chain_id(chainId);
    input.set_account_number(0);
    input.set_sequence(0);
    input.set_source(0);
    input.set_memo(memo);
    // do not set private_key!
    input.set_private_key("");

    auto& order = *input.mutable_send_order();

    Address fromAddress;
    if (!Address::decode(from, fromAddress)) {
        throw std::invalid_argument("Invalid from address");
    }
    const auto fromKeyhash = fromAddress.getKeyHash();
    Address toAddress;
    if (!Address::decode(to, toAddress)) {
        throw std::invalid_argument("Invalid to address");
    }
    const auto toKeyhash = toAddress.getKeyHash();

    {
        auto* input = order.add_inputs();
        input->set_address(fromKeyhash.data(), fromKeyhash.size());
        auto* inputCoin = input->add_coins();
        inputCoin->set_denom(asset);
        inputCoin->set_amount(static_cast<uint64_t>(amount));
    }
    {
        auto* output = order.add_outputs();
        output->set_address(toKeyhash.data(), toKeyhash.size());
        auto* outputCoin = output->add_coins();
        outputCoin->set_denom(asset);
        outputCoin->set_amount(static_cast<uint64_t>(amount));
    }

    const auto txInputData = data(input.SerializeAsString());
    return txInputData;
}
