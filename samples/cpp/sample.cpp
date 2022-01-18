// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWString.h>

#include <iostream>
#include <string>

using namespace std;

int main() {
    try {
        {
            cout << endl;
            cout << "  Wallet Core Demo, C++" << endl;
            cout << endl;
            cout << "  *** DISCLAIMER ***" << endl;
            cout << "  THIS IS A SAMPLE APPLICATION WITH DEMONSTRATION PURPOSES ONLY." << endl;
            cout << "  DO NOT USE WITH REAL SECRETS, REAL ADDRESSESS, OR REAL TRANSACTIONS.  USE IT AT YOUR OWN RISK." << endl;
            cout << "  *** DISCLAIMER ***" << endl;
            cout << endl;
        }

        TWHDWallet* walletImp = nullptr;
        {
            // Create a new multi-coin HD wallet, with new recovery phrase (mnemonic)
            cout << "Creating a new HD wallet ... ";
            TWHDWallet* walletNew = TWHDWalletCreate(128, TWStringCreateWithUTF8Bytes(""));
            cout << "done." << endl;
            cout << "Secret mnemonic for new wallet: '";
            cout << TWStringUTF8Bytes(TWHDWalletMnemonic(walletNew)) << "'." << endl;
            TWHDWalletDelete(walletNew);

            // Alternative: Import wallet with existing recovery phrase (mnemonic)
            cout << "Importing an HD wallet from earlier ... ";
            auto secretMnemonic = TWStringCreateWithUTF8Bytes("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
            walletImp = TWHDWalletCreateWithMnemonic(secretMnemonic, TWStringCreateWithUTF8Bytes(""));
            TWStringDelete(secretMnemonic);
            cout << "done." << endl;
            cout << "Secret mnemonic for imported wallet: '";
            cout << TWStringUTF8Bytes(TWHDWalletMnemonic(walletImp)) << "'." << endl;
            cout << endl;
        }

        {
            // coin type: we use Ethereum
            const TWCoinType coinType = TWCoinType::TWCoinTypeEthereum; // TWCoinTypeBitcoin, TWCoinTypeEthereum
            cout << "Working with coin: " <<
                TWStringUTF8Bytes(TWCoinTypeConfigurationGetName(coinType)) << " " <<
                TWStringUTF8Bytes(TWCoinTypeConfigurationGetSymbol(coinType)) << endl;

            // Derive default address.
            cout << "Obtaining default address ... ";
            string address = TWStringUTF8Bytes(TWHDWalletGetAddressForCoin(walletImp, coinType));
            cout << " done." << endl;
            cout << "Default address:          '" << address << "'" << endl;

            // Alternative: Derive address using default derivation path.
            // Done in 2 steps: derive private key, then address from private key.
            // Note that private key is passed around between the two calls by the wallet -- be always cautious when handling secrets, avoid the risk of leaking secrets.
            cout << "Default derivation path:  " << TWStringUTF8Bytes(TWCoinTypeDerivationPath(coinType)) << endl;
            TWPrivateKey* secretPrivateKeyDefault = TWHDWalletGetKeyForCoin(walletImp, coinType);
            string addressDefault = TWStringUTF8Bytes(TWCoinTypeDeriveAddress(coinType, secretPrivateKeyDefault));
            cout << "Address from default key: '" << addressDefault << "'" << endl;

            // Alternative: Derive address using custom derivation path.  Done in 2 steps: derive private key, then address.
            auto customDerivationPath = TWStringCreateWithUTF8Bytes("m/44'/60'/1'/0/0");
            TWPrivateKey* secretPrivateKeyCustom = TWHDWalletGetKey(walletImp, coinType, customDerivationPath);
            TWStringDelete(customDerivationPath);
            string addressCustom = TWStringUTF8Bytes(TWCoinTypeDeriveAddress(coinType, secretPrivateKeyCustom));
            cout << "Custom-derived address:   '" << addressCustom << "'" << endl;
            cout << endl;

            cout << "RECEIVE funds: Perform send from somewehere else to this address:   " << address << " ." << endl;
            cout << endl;

            // Steps for sending:
            // 1. put together a send message (contains sender and receiver address, amount, gas price, etc.)
            // 2. sign this message
            // 3. broadcast this message to the P2P network -- not done in this sample
            // Note that Signer input and output are represented as protobuf binary messages, for which support is missing in C++.
            // Therefore some direct serialization/parsing is done in helper methods.
            cout << "SEND funds:" << endl;
            const string dummyReceiverAddress = "0xC37054b3b48C3317082E7ba872d7753D13da4986";
            auto secretPrivKey = TWPrivateKeyData(secretPrivateKeyDefault);

            cout << "preparing transaction (using AnySigner) ... ";
            string chainIdB64 = "AQ==";        // base64(parse_hex("01"))
            string gasPriceB64 = "1pOkAA==";   // base64(parse_hex("d693a4")) decimal 3600000000
            string gasLimitB64 = "Ugg=";       // base64(parse_hex("5208")) decimal 21000
            string amountB64 = "A0i8paFgAA=="; // base64(parse_hex("0348bca5a160"))  924400000000000
            string transaction = "{"
                "\"chainId\":\"" + chainIdB64 +
                "\",\"gasPrice\":\"" + gasPriceB64 +
                "\",\"gasLimit\":\"" + gasLimitB64 +
                "\",\"toAddress\":\"" + dummyReceiverAddress +
                "\",\"transaction\":{\"transfer\":{\"amount\":\"" + amountB64 +
                "\"}}}";            
            cout << "transaction: " << transaction << endl;

            cout << "signing transaction ... ";

            auto json = TWStringCreateWithUTF8Bytes(transaction.c_str());
            auto result = TWAnySignerSignJSON(json, secretPrivKey, TWCoinTypeEthereum);
            auto signedTransaction = string(TWStringUTF8Bytes(result));
            cout << "done" << endl;
            cout << "Signed transaction data (to be broadcast to network):  (len " << signedTransaction.length() << ") '" << signedTransaction << "'" << endl;
            // see e.g. https://github.com/flightwallet/decode-eth-tx for checking binary output content
            cout << endl;
            TWStringDelete(json);
            TWStringDelete(result);
        }
        cout << "Bye!" << endl;
        TWHDWalletDelete(walletImp);
    } catch (const exception& ex) {
        cout << "EXCEPTION: " << ex.what() << endl;
        throw ex;
    }
}
