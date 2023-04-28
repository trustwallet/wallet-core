// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "CashAddress.h"
#include "SegwitAddress.h"
#include "Signer.h"

namespace TW::Bitcoin {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    auto* base58Prefix = std::get_if<Base58Prefix>(&addressPrefix);
    auto* hrp = std::get_if<Bech32Prefix>(&addressPrefix);
    bool isValidBase58 = base58Prefix ? Address::isValid(address, {{base58Prefix->p2pkh}, {base58Prefix->p2sh}}) : false;
    bool isValidHrp = hrp ? SegwitAddress::isValid(address, *hrp) : false;

    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeDigiByte:
    case TWCoinTypeLitecoin:
    case TWCoinTypeMonacoin:
    case TWCoinTypeQtum:
    case TWCoinTypeViacoin:
    case TWCoinTypeBitcoinGold:
    case TWCoinTypeSyscoin:
    case TWCoinTypeStratis:
        return isValidBase58 || isValidHrp;
    case TWCoinTypeBitcoinCash:
        return base58Prefix ? isValidBase58 : BitcoinCashAddress::isValid(address);
    case TWCoinTypeECash:
        return base58Prefix ? isValidBase58 : ECashAddress::isValid(address);
    case TWCoinTypeDash:
    case TWCoinTypeDogecoin:
    case TWCoinTypePivx:
    case TWCoinTypeRavencoin:
    case TWCoinTypeFiro:
    default:
        return isValidBase58;
    }
}

std::string Entry::normalizeAddress(TWCoinType coin, const std::string& address) const {
    switch (coin) {
    case TWCoinTypeBitcoinCash:
        // normalized with bitcoincash: prefix
        if (BitcoinCashAddress::isValid(address)) {
            return BitcoinCashAddress(address).string();
        }
        return address;

    case TWCoinTypeECash:
        // normalized with ecash: prefix
        if (ECashAddress::isValid(address)) {
            return ECashAddress(address).string();
        }
        return address;

    default:
        // no change
        return address;
    }
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    byte p2pkh = getFromPrefixPkhOrDefault(addressPrefix, coin);
    const char* hrp = getFromPrefixHrpOrDefault(addressPrefix, coin);

    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
    case TWCoinTypeDigiByte:
    case TWCoinTypeViacoin:
    case TWCoinTypeBitcoinGold:
    case TWCoinTypeSyscoin:
    case TWCoinTypeStratis:
        switch (derivation) {
        case TWDerivationBitcoinLegacy:
        case TWDerivationLitecoinLegacy:
            return Address(publicKey, p2pkh).string();

        case TWDerivationBitcoinTestnet:
            return SegwitAddress::createTestnetFromPublicKey(publicKey).string();

        case TWDerivationBitcoinSegwit:
        case TWDerivationDefault:
        default:
            return SegwitAddress(publicKey, hrp).string();
        }

    case TWCoinTypeBitcoinCash:
        return BitcoinCashAddress(publicKey).string();

    case TWCoinTypeECash:
        return ECashAddress(publicKey).string();

    case TWCoinTypeDash:
    case TWCoinTypeDogecoin:
    case TWCoinTypeMonacoin:
    case TWCoinTypePivx:
    case TWCoinTypeQtum:
    case TWCoinTypeRavencoin:
    case TWCoinTypeFiro:
    default:
        return Address(publicKey, p2pkh).string();
    }
}

template <typename CashAddress>
inline Data cashAddressToData(const CashAddress&& addr) {
    return subData(addr.getData(), 1);
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    switch (coin) {
    case TWCoinTypeBitcoinCash:
        return cashAddressToData(BitcoinCashAddress(address));

    case TWCoinTypeECash:
        return cashAddressToData(ECashAddress(address));

    default: {
        const auto decoded = SegwitAddress::decode(address);
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
    }
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, Proto::PreSigningOutput>(
        txInputData, [](auto&& input, auto&& output) { output = Signer::preImageHashes(input); });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto txCompilerFunctor = [&signatures, &publicKeys](auto&& input, auto&& output) noexcept {
        if (signatures.empty() || publicKeys.empty()) {
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
        auto insertFunctor = [](auto&& signature, auto&& pubkey) noexcept {
            return std::make_pair(signature, pubkey.bytes);
        };
        transform(begin(signatures), end(signatures), begin(publicKeys),
                  back_inserter(externalSignatures), insertFunctor);
        output = Signer::sign(input, externalSignatures);
    };

    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(txInputData,
                                                                            txCompilerFunctor);
}

} // namespace TW::Bitcoin
