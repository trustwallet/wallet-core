// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "../Bitcoin/SegwitAddress.h"
#include "Signer.h"

namespace TW::Groestlcoin {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    return TW::Bitcoin::SegwitAddress::isValid(address, hrp) || Address::isValid(address, {p2pkh, p2sh});
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const {
    return deriveAddress(coin, TWDerivationDefault, publicKey, p2pkh, hrp);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, TWDerivation derivation, const PublicKey& publicKey,
                            TW::byte p2pkh, const char* hrp) const {
    switch (derivation) {
    case TWDerivationBitcoinLegacy:
        return Address(publicKey, p2pkh).string();
    default:
        return TW::Bitcoin::SegwitAddress(publicKey, hrp).string();
    }
}

TW::Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto decoded = Bitcoin::SegwitAddress::decode(address);
    if (!std::get<2>(decoded)) {
        // check if it is a legacy address
        if (Address::isValid(address)) {
            const auto addr = Address(address);
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

} // namespace TW::Groestlcoin