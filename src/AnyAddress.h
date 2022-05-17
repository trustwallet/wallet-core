// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrezorCrypto/cash_addr.h>

#include "Data.h"
#include "HexCoding.h"

#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Cardano/AddressV3.h"
#include "Cosmos/Address.h"
#include "Decred/Address.h"
#include "Ethereum/Address.h"
#include "Elrond/Address.h"
#include "Kusama/Address.h"
#include "NativeEvmos/Address.h"
#include "NEAR/Address.h"
#include "NEO/Address.h"
#include "Nano/Address.h"
#include "Polkadot/Address.h"
#include "Ronin/Address.h"
#include "Solana/Address.h"
#include "Zcash/TAddress.h"
#include "Zilliqa/Address.h"

#include <string>

namespace TW {

class AnyAddress {
  public:
    static auto dataFromString(const std::string& string, TWCoinType coin) -> Data {
        switch (coin) {
        case TWCoinTypeBinance:
        case TWCoinTypeCosmos:
        case TWCoinTypeKava:
        case TWCoinTypeTerra:
        case TWCoinTypeBandChain:
        case TWCoinTypeTHORChain:
        case TWCoinTypeBluzelle:
        case TWCoinTypeIoTeX:
        case TWCoinTypeCryptoOrg:
        case TWCoinTypeOsmosis:
        case TWCoinTypeHarmony: {
            Cosmos::Address addr;
            if (!Cosmos::Address::decode(string, addr)) {
                break;
            }
            return addr.getKeyHash();
        }

        case TWCoinTypeBitcoin:
        case TWCoinTypeDigiByte:
        case TWCoinTypeGroestlcoin:
        case TWCoinTypeLitecoin:
        case TWCoinTypeViacoin: {
            auto decoded = Bitcoin::SegwitAddress::decode(string);
            if (!std::get<2>(decoded)) {
                break;
            }
            return std::get<0>(decoded).witnessProgram;
        }

        case TWCoinTypeBitcoinCash: {
            Data data;
            auto addr = Bitcoin::BitcoinCashAddress(string);
            data.resize(Bitcoin::Address::size);
            size_t outlen = 0;
            cash_data_to_addr(data.data(), &outlen, addr.bytes.data(), 34);
            return {data.begin() + 1, data.end()};
        }

        case TWCoinTypeECash: {
            Data data;
            auto addr = Bitcoin::ECashAddress(string);
            data.resize(Bitcoin::Address::size);
            size_t outlen = 0;
            cash_data_to_addr(data.data(), &outlen, addr.bytes.data(), 34);
            return {data.begin() + 1, data.end()};
        }

        case TWCoinTypeDash:
        case TWCoinTypeDogecoin:
        case TWCoinTypeMonacoin:
        case TWCoinTypeQtum:
        case TWCoinTypeRavencoin:
        case TWCoinTypeFiro: {
            auto addr = Bitcoin::Address(string);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }

        case TWCoinTypeDecred: {
            auto addr = Decred::Address(string);
            return {addr.bytes.begin() + 2, addr.bytes.end()};
        }

        case TWCoinTypeZcash:
        case TWCoinTypeZelcash: {
            auto addr = Zcash::TAddress(string);
            return {addr.bytes.begin() + 2, addr.bytes.end()};
        }

        case TWCoinTypeCallisto:
        case TWCoinTypeEthereum:
        case TWCoinTypeEthereumClassic:
        case TWCoinTypeGoChain:
        case TWCoinTypePOANetwork:
        case TWCoinTypeThunderToken:
        case TWCoinTypeTomoChain:
        case TWCoinTypeVeChain:
        case TWCoinTypeTheta:
        case TWCoinTypeWanchain:
        case TWCoinTypeAion:
        case TWCoinTypeSmartChainLegacy:
        case TWCoinTypeSmartChain:
        case TWCoinTypePolygon:
        case TWCoinTypeOptimism:
        case TWCoinTypeArbitrum:
        case TWCoinTypeECOChain:
        case TWCoinTypeXDai:
        case TWCoinTypeAvalancheCChain:
        case TWCoinTypeFantom:
        case TWCoinTypeCelo:
        case TWCoinTypeCronosChain:
        case TWCoinTypeSmartBitcoinCash:
        case TWCoinTypeKuCoinCommunityChain:
        case TWCoinTypeBoba:
        case TWCoinTypeMetis:
        case TWCoinTypeAurora:
        case TWCoinTypeEvmos: {
            const auto addr = Ethereum::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }
        case TWCoinTypeRonin: {
            const auto addr = Ronin::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }
        case TWCoinTypeNano: {
            auto addr = Nano::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWCoinTypeZilliqa: {
            Zilliqa::Address addr;
            if (!Zilliqa::Address::decode(string, addr)) {
                break;
            }
            // data in Zilliqa is a checksummed string without 0x
            auto str = Zilliqa::checksum(addr.getKeyHash());
            return {str.begin(), str.end()};
        }

        case TWCoinTypeKusama: {
            auto addr = Kusama::Address(string);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }

        case TWCoinTypePolkadot: {
            auto addr = Polkadot::Address(string);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }

        case TWCoinTypeCardano: {
            auto addr = Cardano::AddressV3(string);
            return addr.data();
        }

        case TWCoinTypeNEO: {
            auto addr = NEO::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWCoinTypeElrond: {
            Elrond::Address addr;
            if (!Elrond::Address::decode(string, addr)) {
                break;
            }
            return addr.getKeyHash();
        }

        case TWCoinTypeNEAR: {
            auto addr = NEAR::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }
        case TWCoinTypeSolana: {
            return Solana::Address(string).vector();
        }
        case TWCoinTypeNativeEvmos: {
            NativeEvmos::Address addr;
            if (!NativeEvmos::Address::decode(string, addr)) {
                break;
            }
            return addr.getKeyHash();
        }
        default:
            break;
        }
        return {};
    }
};

} // namespace TW
