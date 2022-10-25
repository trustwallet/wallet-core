// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Bitcoin/Address.h"
#include "Signer.h"
#include "TAddress.h"

namespace TW::Zcash {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, [[maybe_unused]] const char* hrp) const {
    if (coin == TWCoinTypeKomodo) {
        return Bitcoin::Address::isValid(address, {{p2pkh}, {p2sh}});
    }
    return TAddress::isValid(address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, [[maybe_unused]] const char* hrp) const {
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

void Entry::plan([[maybe_unused]] [[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
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
