// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrezorCrypto/cash_addr.h>
#include <TrustWalletCore/TWBlockchain.h>

#include "Coin.h"
#include "Data.h"
#include "HexCoding.h"

#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Cardano/AddressV3.h"
#include "Cosmos/Address.h"
#include "Decred/Address.h"
#include "Elrond/Address.h"
#include "Ethereum/Address.h"
#include "Kusama/Address.h"
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
        const auto blockchain = TW::blockchain(coin);
        switch (blockchain) {
        case TWBlockchainBinance:
        case TWBlockchainCosmos:
        case TWBlockchainThorchain:
        case TWBlockchainIoTeX:
        case TWBlockchainHarmony: {
            Cosmos::Address addr;
            if (!Cosmos::Address::decode(string, addr)) {
                break;
            }
            return addr.getKeyHash();
        }

        case TWBlockchainBitcoin:
        case TWBlockchainGroestlcoin:
            switch (coin) {
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
                Data data(Bitcoin::Address::size);
                auto addr = Bitcoin::BitcoinCashAddress(string);
                size_t outlen = 0;
                cash_data_to_addr(data.data(), &outlen, addr.bytes.data(), 34);
                return {data.begin() + 1, data.end()};
            }

            case TWCoinTypeECash: {
                Data data(Bitcoin::Address::size);
                auto addr = Bitcoin::ECashAddress(string);
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
            default:
                break;
            }

        case TWBlockchainDecred: {
            auto addr = Decred::Address(string);
            return {addr.bytes.begin() + 2, addr.bytes.end()};
        }

        case TWBlockchainZcash: {
            auto addr = Zcash::TAddress(string);
            return {addr.bytes.begin() + 2, addr.bytes.end()};
        }

        case TWBlockchainEthereum:
        case TWBlockchainVechain:
        case TWBlockchainTheta:
        case TWBlockchainAion: {
            const auto addr = Ethereum::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWBlockchainRonin: {
            const auto addr = Ronin::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }
        case TWBlockchainNano: {
            auto addr = Nano::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWBlockchainZilliqa: {
            Zilliqa::Address addr;
            if (!Zilliqa::Address::decode(string, addr)) {
                break;
            }
            // data in Zilliqa is a checksummed string without 0x
            auto str = Zilliqa::checksum(addr.getKeyHash());
            return {str.begin(), str.end()};
        }

        case TWBlockchainKusama: {
            auto addr = Kusama::Address(string);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }

        case TWBlockchainPolkadot: {
            auto addr = Polkadot::Address(string);
            return {addr.bytes.begin() + 1, addr.bytes.end()};
        }

        case TWBlockchainCardano: {
            auto addr = Cardano::AddressV3(string);
            return addr.data();
        }

        case TWBlockchainNEO: {
            auto addr = NEO::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWBlockchainElrondNetwork: {
            Elrond::Address addr;
            if (!Elrond::Address::decode(string, addr)) {
                break;
            }
            return addr.getKeyHash();
        }

        case TWBlockchainNEAR: {
            auto addr = NEAR::Address(string);
            return {addr.bytes.begin(), addr.bytes.end()};
        }

        case TWBlockchainSolana: {
            return Solana::Address(string).vector();
        }

        default:
            break;
        }
        return {};
    }
};

} // namespace TW
