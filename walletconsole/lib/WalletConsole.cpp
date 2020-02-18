// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "WalletConsole.h"
#include "CommandExecutor.h"
#include "Keys.h"
#include "Coins.h"
#include "Util.h"
#include "Address.h"
#include "TonCoin.h"

#include "Base64.h"
//#include "Base58.h"
#include "HexCoding.h"
#include "Data.h"

#include "FIO/TransactionBuilder.h" // TODO

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;
using namespace TW;
using namespace TW::FIO; // TODO


void WalletConsole::init() {
    _outst << endl;
    _outst << "Wallet-core Console                          (c) TrustWallet" << endl;
    _outst << "Type 'help' for list of commands." << endl;
    _outst << endl;

///*
    ChainParams chainParams{
        //parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77"),
        parse_hex("afeadd92017e28abf0cb79324bd356ec3c8ff143dd43318b0954020fcdbb51f8"),
        123020, 
        2523882092
    };
    // 5JxV2dBsUpoV2mgkv5NArmdK1D73N7FVdUNEJ1fqCnpDQEACecQ FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE
    PrivateKey privKey = PrivateKey(parse_hex("964f2916371ac2e4297517f54eead43ffb7754fbfbd5abc880f36c2c063e2f49"));
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    FIO::Address addr(pubKey);
    uint64_t maxFee = 5000000000;
    PrivateKey privKey2 = PrivateKey(parse_hex("81b637d8fcd2c6da6359e6963113a1170de795e4b725b84d1e0b4cfd9ec58ce9"));
    PublicKey pubKey2 = privKey2.getPublicKey(TWPublicKeyTypeSECP256k1);
    FIO::Address addr2(pubKey2);

    ///*
    string regAddTx = TransactionBuilder::createRegisterFioAddress(addr, privKey, "adam@fiotestnet",
        chainParams, maxFee, "rewards@wallet", 0);
    _outst << regAddTx << endl << endl;

    string addPubTx = TransactionBuilder::createAddPubAddress(addr, privKey, "adam@fiotestnet", {
        {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"},
        {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"},
        {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}}, chainParams, 0, "rewards@wallet", 0);
    _outst << addPubTx << endl << endl;
    //_outst << hex(Base58::bitcoin.decode("5KEB6pSUyzDLVEZf2PVBSbv7gp3ddzLQwhhrbjNd59Lm8B3nUZY")) << endl;
    //_outst << Base58::bitcoin.encodeCheck(parse_hex("80964f2916371ac2e4297517f54eead43ffb7754fbfbd5abc880f36c2c063e2f49")) << endl;
    //_outst << Base58::bitcoin.encodeCheck(parse_hex("80ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035")) << endl;
    //*/

    /*
    const Data iv = parse_hex("000102030405060708090a0b0c0d0e0f"); // use fixed iv for testability
    string newFundTx = TransactionBuilder::createNewFundsRequest(addr, privKey, 
        "mario@trust", addr2.string(), "alice@fiotestnet",
        "5", "BNB", "Memo", "Hash", "https://trustwallet.com",
        chainParams, maxFee, "rewards@wallet", 0, iv);
    _outst << newFundTx << endl << endl;
    //*/

    _executor.init();
}

void WalletConsole::loop() {
    while (true) {
        _outst << "> ";
        string line;
        getline(_inst, line);
        Util::trimLeft(line);
        if (line.length() == 0) { continue; }
        if (isExit(line)) {
            break;
        }

        _executor.executeLine(line);
    }
    _outst << "Bye!" << endl << endl;
}

bool WalletConsole::isExit(const string& line) {
    if (line.length() < 4) return false;
    string first4 = line.substr(0, 4);
    if (first4 == "quit" || first4 == "exit") { return true; }
    return false;
}

} // namespace TW::WalletConsole
