// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class AvalancheTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalanche)
        let addressFromString = AnyAddress(string: "X-avax1apmh7wxg3js48fhacfv5y9md9065jxuft30vup", coin: .avalanche)!

        XCTAssertEqual(pubkey.data.hexString, "0x02b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        // TODO: Create implementation

        /*
        const auto privateKeyBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
        const std::vector<uint8_t> privateKeyBytesNoChecksum(privateKeyBytes.begin(), privateKeyBytes.begin() + 32); // we just want the first 32 bytes
        const auto privateKey = PrivateKey(privateKeyBytesNoChecksum); 
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        
        auto blockchainID = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
        Data blockchainIDBytes(blockchainID.begin(), blockchainID.begin() + 32); // we just want the first 32 bytes, no checksum
        uint32_t netID = 12345;
        auto assetID = "0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db";
        auto assetIDBytes = parse_hex(assetID);
        auto txIDBytes = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"); 
        Data memo = {0xde, 0xad, 0xbe, 0xef};
        auto amount = 1000;   
        auto locktime = 0;
        auto threshold = 1;

        Proto::SigningInput input;

        // keep in mind the expected formats in Signer!
        // for example, addresses are expected as secp pubkey, not avalanche-formatted address
        input.add_private_keys();
        input.set_private_keys(0, privateKeyBytesNoChecksum.data(), privateKeyBytesNoChecksum.size());
        auto &inputTx = *input.mutable_input_tx();
        auto &baseTx = *inputTx.mutable_base_tx();
        baseTx.set_typeid_(0);
        baseTx.set_network_id(netID);
        baseTx.set_blockchain_id(blockchainIDBytes.data(), blockchainIDBytes.size());
        baseTx.set_memo(memo.data(), memo.size());
        
        auto coreInputOne = new Proto::SECP256K1TransferInput();
        coreInputOne->set_amount(123456789);
        coreInputOne->add_address_indices(3);
        coreInputOne->add_address_indices(7);
        auto wrappedInputOne = new Proto::TransactionInput();
        wrappedInputOne->set_allocated_secp_transfer_input(coreInputOne);
        auto inputOne = baseTx.add_inputs();
        inputOne->set_utxo_index(5);
        inputOne->set_tx_id(txIDBytes.data(), txIDBytes.size());
        inputOne->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
        for (auto i = 0; i < 8; ++i) {
            inputOne->add_spendable_addresses();
            inputOne->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
        }
        inputOne->set_allocated_input(wrappedInputOne);
        auto coreInputTwo = new Proto::SECP256K1TransferInput();
        coreInputTwo->set_amount(123456789);
        coreInputTwo->add_address_indices(3);
        coreInputTwo->add_address_indices(7);
        auto wrappedInputTwo = new Proto::TransactionInput();
        wrappedInputTwo->set_allocated_secp_transfer_input(coreInputTwo);
        auto inputTwo = baseTx.add_inputs();
        inputTwo->set_utxo_index(5);
        inputTwo->set_tx_id(txIDBytes.data(), txIDBytes.size());
        inputTwo->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
        for (auto i = 0; i < 8; ++i) {
            inputTwo->add_spendable_addresses();
            inputTwo->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
        }
        inputTwo->set_allocated_input(wrappedInputTwo);

        auto coreOutputOne = new Proto::SECP256K1TransferOutput();
        coreOutputOne->set_amount(12345);
        coreOutputOne->set_locktime(54321);
        coreOutputOne->set_threshold(threshold);
        coreOutputOne->add_addresses();
        coreOutputOne->set_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());
        auto wrappedOutputOne = new Proto::TransactionOutput();
        wrappedOutputOne->set_allocated_secp_transfer_output(coreOutputOne);
        auto outputOne = baseTx.add_outputs();
        outputOne->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
        outputOne->set_allocated_output(wrappedOutputOne);

        auto coreOutputTwo = new Proto::SECP256K1TransferOutput();
        coreOutputTwo->set_amount(amount);
        coreOutputTwo->set_locktime(locktime);
        coreOutputTwo->set_threshold(threshold);
        coreOutputTwo->add_addresses();
        coreOutputTwo->set_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());    
        auto wrappedOutputTwo = new Proto::TransactionOutput();
        wrappedOutputTwo->set_allocated_secp_transfer_output(coreOutputTwo);
        auto outputTwo = baseTx.add_outputs();
        outputTwo->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
        outputTwo->set_allocated_output(wrappedOutputTwo);
        */

        let output: AvlancheSigningOutput = AnySigner.sign(input: input, coin: .avalanche)

        XCTAssertEqual(output.encoded.hexString, "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00")

    }
}
