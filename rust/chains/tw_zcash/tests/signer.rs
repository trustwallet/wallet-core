// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_encoding::hex::ToHex;
use tw_keypair::ecdsa::secp256k1;
use tw_utxo::encode::Encodable;
use tw_utxo::modules::keys_manager::KeysManager;
use tw_utxo::modules::tx_signer::TxSigner;
use tw_utxo::sighash::{SighashBase, SighashType};
use tw_utxo::transaction::standard_transaction::builder::{OutputBuilder, UtxoBuilder};
use tw_zcash::modules::transaction_builder::ZcashTransactionBuilder;
use tw_zcash::t_address::TAddress;

#[test]
fn test_zcash_signer() {
    const PRIVATE_KEY: &str = "a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559";
    const SAPPLING_BRANCH_ID: &str = "bb09b876";

    // Step 1 - Build an unsigned transaction.

    let private_key = secp256k1::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    // Public key - 03b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6
    let public_key = private_key.public();

    let mut tx_builder = ZcashTransactionBuilder::new();
    // BranchId is required.
    tx_builder.branch_id(SAPPLING_BRANCH_ID.into());

    // Add a P2PKH UTXO.
    let (utxo, utxo_args) = UtxoBuilder::new()
        .prev_txid("53685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a".into())
        .prev_index(0)
        .sequence(u32::MAX)
        .amount(494_000)
        .sighash_type(SighashType::from_base(SighashBase::All))
        .p2pkh(&public_key)
        .unwrap();
    tx_builder.push_input(utxo, utxo_args);

    // Add a P2PKH Output.
    let recipient_addr = TAddress::from_str("t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS").unwrap();
    let output = OutputBuilder::new(488_000).p2pkh_from_hash(&recipient_addr.payload());
    tx_builder.push_output(output);

    let unsigned_tx = tx_builder.build().unwrap();

    // Step 2 - Sign the transaction.

    let mut keys_manager = KeysManager::default();
    keys_manager.add_ecdsa_private(private_key);

    let signed_tx = TxSigner::sign_tx(unsigned_tx, &keys_manager).unwrap();
    assert_eq!(
        signed_tx.encode_out().to_hex(),
        "0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000"
    );
}
