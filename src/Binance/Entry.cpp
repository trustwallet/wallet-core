// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "../proto/TransactionCompiler.pb.h"
#include "Address.h"
#include "Coin.h"
#include "Signer.h"

namespace TW::Binance {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    if (std::holds_alternative<Bech32Prefix>(addressPrefix)) {
        if (const auto* hrp = std::get<Bech32Prefix>(addressPrefix); hrp) {
            return Address::isValid(address, hrp);
        }
    }
    return Address::isValid(coin, address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    const std::string hrp = getFromPrefixHrpOrDefault(addressPrefix, coin);
    return Address(publicKey, hrp).string();
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    const char* hrp = stringForHRP(TW::hrp(coin));
    Address addr(hrp);
    if (Address::decode(address, addr)) {
        return addr.getKeyHash();
    }
    return {};
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    auto input = Proto::SigningInput();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    const char* hrp = stringForHRP(TW::hrp(coin));
    auto serializedOut = Signer::sign(input, hrp).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

std::string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    const char* hrp = stringForHRP(TW::hrp(coin));
    return Signer::signJSON(json, key, hrp);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    const char* hrp = stringForHRP(TW::hrp(coin));

    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [hrp](const auto& input, auto& output) {
            Signer signer(input, hrp);

            auto preImageHash = signer.preImageHash();
            auto preImage = signer.signaturePreimage();
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    const char* hrp = stringForHRP(TW::hrp(coin));

    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [hrp](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            output = Signer(input, hrp).compile(signature, publicKey);
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

    const char* hrp = stringForHRP(TW::hrp(coinType));

    Data fromKeyhash;
    Data toKeyhash;

    Address fromAddress(hrp);
    if (!Address::decode(from, fromAddress)) {
        throw std::invalid_argument("Invalid from address");
    }
    fromKeyhash = fromAddress.getKeyHash();

    Address toAddress(hrp);
    if (!Address::decode(to, toAddress)) {
        throw std::invalid_argument("Invalid to address");
    }
    toKeyhash = toAddress.getKeyHash();

    {
        auto* sendOrderInputs = order.add_inputs();
        sendOrderInputs->set_address(fromKeyhash.data(), fromKeyhash.size());
        auto* inputCoin = sendOrderInputs->add_coins();
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

} // namespace TW::Binance
