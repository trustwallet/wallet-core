// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Blockchain enum type
TW_EXPORT_ENUM(uint32_t)
enum TWBlockchain {
    TWBlockchainBitcoin = 0,
    TWBlockchainEthereum = 1,
    TWBlockchainVechain = 3,
    TWBlockchainTron = 4,
    TWBlockchainIcon = 5,
    TWBlockchainBinance = 6,
    TWBlockchainRipple = 7,
    TWBlockchainTezos = 8,
    TWBlockchainNimiq = 9,
    TWBlockchainStellar = 10,
    TWBlockchainAion = 11,
    TWBlockchainCosmos = 12,
    TWBlockchainTheta = 13,
    TWBlockchainOntology = 14,
    TWBlockchainZilliqa = 15,
    TWBlockchainIoTeX = 16,
    TWBlockchainEOS = 17,
    TWBlockchainNano = 18,
    TWBlockchainNULS = 19,
    TWBlockchainWaves = 20,
    TWBlockchainAeternity = 21,
    TWBlockchainNebulas = 22,
    TWBlockchainFIO = 23,
    TWBlockchainSolana = 24,
    TWBlockchainHarmony = 25,
    TWBlockchainNEAR = 26,
    TWBlockchainAlgorand = 27,
    TWBlockchainIOST = 28,
    TWBlockchainPolkadot = 29,
    TWBlockchainCardano = 30,
    TWBlockchainNEO = 31,
    TWBlockchainFilecoin = 32,
    TWBlockchainMultiversX = 33,
    TWBlockchainOasisNetwork = 34,
    TWBlockchainDecred = 35, // Bitcoin
    TWBlockchainZcash = 36, // Bitcoin
    TWBlockchainGroestlcoin = 37, // Bitcoin
    TWBlockchainThorchain = 38, // Cosmos
    TWBlockchainRonin = 39, // Ethereum
    TWBlockchainKusama = 40, // Polkadot
    TWBlockchainZen = 41, // Bitcoin
    TWBlockchainBitcoinDiamond = 42, // Bitcoin
    TWBlockchainVerge = 43, // Bitcoin
    TWBlockchainNervos = 44,
    TWBlockchainEverscale = 45,
    TWBlockchainAptos = 46, // Aptos
    TWBlockchainNebl = 47, // Bitcoin
    TWBlockchainHedera = 48, // Hedera
    TWBlockchainTheOpenNetwork = 49,
    TWBlockchainSui = 50,
    TWBlockchainGreenfield = 51,
    TWBlockchainInternetComputer = 52,
    TWBlockchainNativeEvmos = 53, // Cosmos
    TWBlockchainNativeInjective = 54, // Cosmos
};

TW_EXTERN_C_END
