// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyProto.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWString.h>

// Note: the proto include files are not exposed with C++ includes
#include <proto/Any.pb.h> // AnySigner

#include <iostream>
#include <string>

using namespace std;

/// Helper for AnySigner input protobuf message building
string buildAnySignerInputMsg(uint32_t coinType, string transaction, string privKeyHex) {
    TW::Any::Proto::SigningInput si;
    si.set_coin_type(coinType);
    si.set_transaction(transaction);
    si.set_private_key(privKeyHex);
    string serialized;
    if (!si.SerializeToString(&serialized)) { return ""; }
    return serialized;
}

/// Helper for Signer output protobuf message parsing.
bool parseSignedTransactionOutput(TW_Any_Proto_SigningOutput signerOutput, string& out) {
    // copy to byte string
    string outStr;
    outStr.insert(outStr.end(), TWDataBytes(signerOutput), TWDataBytes(signerOutput) + TWDataSize(signerOutput));
    // parse protobuf message
    TW::Any::Proto::SigningOutput so;
    if (!so.ParseFromString(outStr)) {
        cout << "Error parsing SignerOutput protobuf message!" << endl;
        return false;
    }
    if (so.has_error()) {
        cout << "SignerOutput has error! " << so.error().description() << endl;
        return false;
    }
    // OK, get output
    out = so.output();
    return true;
}

int main() {
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
        cout << "Secret menmonic for new wallet: '";
        cout << TWStringUTF8Bytes(TWHDWalletMnemonic(walletNew)) << "'." << endl;

        // Alternative: Import wallet with existing recovery phrase (mnemonic)
        cout << "Importing an HD wallet from earlier ... ";
        auto secretMnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
        walletImp = TWHDWalletCreateWithMnemonic(TWStringCreateWithUTF8Bytes(secretMnemonic), TWStringCreateWithUTF8Bytes(""));
        cout << "done." << endl;
        cout << "Secret menmonic for imported wallet: '";
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
        auto customDerivationPath = "m/44'/60'/1'/0/0";
        TWPrivateKey* secretPrivateKeyCustom = TWHDWalletGetKey(walletImp, TWStringCreateWithUTF8Bytes(customDerivationPath));
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
        const string secretPrivKeyHex = TWStringUTF8Bytes(TWStringCreateWithHexData(TWPrivateKeyData(secretPrivateKeyDefault)));
        {
            cout << "preparing transaction (using AnySigner) ... ";
            string chainIdB64 = "AQ=="; // base64(parse_hex("01"))
            string gasPriceB64 = "1pOkAA=="; // base64(parse_hex("d693a4")) decimal 3600000000
            string gasLimitB64 = "Ugg="; // base64(parse_hex("5208")) decimal 21000
            string amountB64 = "A0i8paFgAA=="; // base64(parse_hex("0348bca5a160"))  924400000000000
            string transaction = "{"
                "\"chainId\":\"" + chainIdB64 +
                "\",\"gasPrice\":\"" + gasPriceB64 +
                "\",\"gasLimit\":\"" + gasLimitB64 +
                "\",\"toAddress\":\"" + dummyReceiverAddress +
                "\",\"amount\":\"" + amountB64 + "\"}";
            cout << "transaction: " << transaction << endl;

            string signerInput = buildAnySignerInputMsg((uint32_t)coinType, transaction, secretPrivKeyHex);
            cout << "signing transaction ... ";
            TW_Any_Proto_SigningOutput signerOutput = TWAnySignerSign(TWDataCreateWithBytes((const uint8_t*)signerInput.c_str(), signerInput.size()));
            cout << "done" << endl;
            // Extract signed output
            string signedTransaction;
            if (!parseSignedTransactionOutput(signerOutput, signedTransaction)) {
                cout << "Could not parse out signed transaction!" << endl;
            }
            else
            {
                cout << "Signed transaction data (to be broadcast to network):  (len " << signedTransaction.length() << ") '" << signedTransaction << "'" << endl;
                // see e.g. https://github.com/flightwallet/decode-eth-tx for checking binary output content
            }
            cout << endl;
        }
    }

    cout << "Bye!" << endl;
}
