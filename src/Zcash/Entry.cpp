// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Bitcoin/Address.h"
#include "Signer.h"
#include "TAddress.h"

namespace TW::Zcash {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    if (coin == TWCoinTypeKomodo) {
        auto* base58Prefix = std::get_if<Base58Prefix>(&addressPrefix);
        return base58Prefix ? Bitcoin::Address::isValid(address, {{base58Prefix->p2pkh}, {base58Prefix->p2sh}}) : false;
    }
    return TAddress::isValid(address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    byte p2pkh = getFromPrefixPkhOrDefault(addressPrefix, coin);
    if (coin == TWCoinTypeKomodo) {
        return Bitcoin::Address(publicKey, p2pkh).string();
    }
    return TAddress(publicKey, p2pkh).string();
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    if (coin == TWCoinTypeKomodo) {
        const auto addr = Bitcoin::Address(address);
        return {addr.bytes.begin() + 1, addr.bytes.end()};
    }

    const auto addr = TAddress(address);
    return {addr.bytes.begin() + 2, addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Bitcoin::Proto::SigningInput, Bitcoin::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) { 
            output = Signer::preImageHashes(input); 
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Bitcoin::Proto::SigningInput, Bitcoin::Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }

            if (signatures.size() != publicKeys.size()) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("signatures size and publickeys size not equal");
                return;
            }

            HashPubkeyList externalSignatures;
            auto n = signatures.size();
            for (auto i = 0ul; i < n; ++i) {
                externalSignatures.push_back(std::make_pair(signatures[i], publicKeys[i].bytes));
            }

            output = Signer::sign(input, externalSignatures);
        });
}

} // namespace TW::Zcash
