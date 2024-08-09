// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Bitcoin/Address.h"
#include "Bitcoin/SegwitAddress.h"
#include "Signer.h"

using namespace std;

namespace TW::Verge {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const string& address, const PrefixVariant& addressPrefix) const {
    auto* base58Prefix = std::get_if<Base58Prefix>(&addressPrefix);
    auto* hrp = std::get_if<Bech32Prefix>(&addressPrefix);
    bool isValidBase58 = base58Prefix ? Bitcoin::Address::isValid(address) : false;
    bool isValidHrp = hrp ? Bitcoin::SegwitAddress::isValid(address, *hrp) : false;
    return isValidBase58 || isValidHrp;
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    byte p2pkh = getFromPrefixPkhOrDefault(addressPrefix, coin);
    const char* hrp = getFromPrefixHrpOrDefault(addressPrefix, coin);

    switch (derivation) {
    case TWDerivationBitcoinLegacy:
    case TWDerivationDefault:
        return Bitcoin::Address(publicKey, p2pkh).string();
    default:
        return TW::Bitcoin::SegwitAddress(publicKey, hrp).string();
    }
}

TW::Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto decoded = Bitcoin::SegwitAddress::decode(address);
    if (!std::get<2>(decoded)) {
        // check if it is a legacy address
        if (Bitcoin::Address::isValid(address)) {
            const auto addr = Bitcoin::Address(address);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }
        return {Data()};
    }
    return std::get<0>(decoded).witnessProgram;
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

} // namespace TW::Verge
