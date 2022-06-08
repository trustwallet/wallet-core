// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "CashAddress.h"
#include "SegwitAddress.h"
#include "Signer.h"

#include <TrezorCrypto/cash_addr.h>

using namespace TW::Bitcoin;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, byte p2pkh, byte p2sh, const char* hrp) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
        case TWCoinTypeDigiByte:
        case TWCoinTypeLitecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeViacoin:
        case TWCoinTypeBitcoinGold:
            return SegwitAddress::isValid(address, hrp)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeBitcoinCash:
            return BitcoinCashAddress::isValid(address)
                || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeECash:
            return ECashAddress::isValid(address)
                   || Address::isValid(address, {{p2pkh}, {p2sh}});

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeRavencoin:
        case TWCoinTypeFiro:
        default:
            return Address::isValid(address, {{p2pkh}, {p2sh}});
    }
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    switch (coin) {
        case TWCoinTypeBitcoinCash:
            // normalized with bitcoincash: prefix
            if (BitcoinCashAddress::isValid(address)) {
                return BitcoinCashAddress(address).string();
            } else {
                return std::string(address);
            }

        case TWCoinTypeECash:
            // normalized with ecash: prefix
            if (ECashAddress::isValid(address)) {
                return ECashAddress(address).string();
            } else {
                return std::string(address);
            }

        default:
            // no change
            return address;
    }
}

string Entry::deriveAddress(TWCoinType coin, TWDerivation derivation, const PublicKey& publicKey, byte p2pkh, const char* hrp) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
        case TWCoinTypeLitecoin:
            switch (derivation) {
                case TWDerivationBitcoinLegacy:
                case TWDerivationLitecoinLegacy:
                    return Address(publicKey, p2pkh).string();

                case TWDerivationBitcoinSegwit:
                case TWDerivationDefault:
                default:
                    return SegwitAddress(publicKey, hrp).string();
            }

        case TWCoinTypeDigiByte:
        case TWCoinTypeViacoin:
        case TWCoinTypeBitcoinGold:
            return SegwitAddress(publicKey, hrp).string();

        case TWCoinTypeBitcoinCash:
            return BitcoinCashAddress(publicKey).string();

        case TWCoinTypeECash:
            return ECashAddress(publicKey).string();

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeRavencoin:
        case TWCoinTypeFiro:
        default:
            return Address(publicKey, p2pkh).string();
    }
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    switch (coin) {
        case TWCoinTypeBitcoin:
        case TWCoinTypeDigiByte:
        case TWCoinTypeGroestlcoin:
        case TWCoinTypeLitecoin:
        case TWCoinTypeViacoin:
            {
                const auto decoded = SegwitAddress::decode(address);
                if (!std::get<2>(decoded)) {
                    return Data();
                }
                return std::get<0>(decoded).witnessProgram;
            }

        case TWCoinTypeBitcoinCash:
            {
                const auto addr = BitcoinCashAddress(address);
                Data data(Address::size);
                size_t outlen = 0;
                cash_data_to_addr(data.data(), &outlen, addr.bytes.data(), 34);
                return {data.begin() + 1, data.end()};
            }

        case TWCoinTypeECash:
            {
                const auto addr = ECashAddress(address);
                Data data(Address::size);
                size_t outlen = 0;
                cash_data_to_addr(data.data(), &outlen, addr.bytes.data(), 34);
                return {data.begin() + 1, data.end()};
            }

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeRavencoin:
        case TWCoinTypeFiro:
            {
                const auto addr = Address(address);
                return {addr.bytes.begin() + 1, addr.bytes.end()};
            }

        default:
            return Data();
    }
}

void Entry::sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            output = Signer::preImageHashes(input);
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

            if (signatures.size() != publicKeys.size()) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("signatures size and publickeys size not equal");
                return;
            }

            HashPubkeyList externalSignatures;
            auto n = signatures.size();
            for (auto i = 0; i < n; ++i) {
                externalSignatures.push_back(std::make_pair(signatures[i], publicKeys[i].bytes));
            }

            output = Signer::sign(input, externalSignatures);
        });
}
