use tw_bitcoin::{
    bitcoin::{hashes::Hash, PublicKey, Txid},
    keypair_from_wif, TransactionBuilder, TxInputP2TRKeySpend, TxOutputP2TKeyPath,
};
use tw_encoding::hex;

mod common;
use common::*;

#[test]
fn sign_input_p2tr_output_p2tr() {
    // TODO: This does not really work here given that the Schnorr signatures
    // uses random auxiliary data as input. Therefore, we should test individual
    // components.
    const EXPECTED_RAW_SIGNED: &str = "0200000000010115a009856a979cea16875cec396f5154e80d0a4c461d90516ceec6ce1da37aad0000000000ffffffff0100e1f505000000002251201b09bbb94afb9b8098a1e661c04d9079fb9464f26928dce48128bbe489e606820141a35d48563b9fe13fb4694a73e8a1f66e1b356d983b2ae53fbb4342b218311a9d23bfc346df4188ff296a23e1e4db6637fa172c2168627ea78f44af9589d9ca980100000000";

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let txid = Txid::hash(b"random");
    let vout = 0;
    // TODO: this can be done nicer
    let recipient = PublicKey::new(alice.public_key());
    let satoshis = ONE_BTC * 10;

    let input = TxInputP2TRKeySpend::new(txid, vout, recipient, satoshis);

    // Prepare outputs for Bob
    let recipient = PublicKey::new(bob.public_key());
    let satoshis = ONE_BTC;

    let output = TxOutputP2TKeyPath::new(satoshis, recipient);

    let signed_transaction = TransactionBuilder::new()
        // TODO: Set return address, decrease miner fee.
        .miner_fee(ONE_BTC * 9)
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let _hex = hex::encode(signed_transaction, false);
    //assert_eq!(&hex, EXPECTED_RAW_SIGNED);
}
