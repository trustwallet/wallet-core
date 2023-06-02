use tw_bitcoin::{
    bitcoin::{hashes::Hash, PublicKey, Txid},
    keypair_from_wif, PubkeyHash, TransactionBuilder, TxInputP2PKH, TxOutputP2PKH,
};
use tw_encoding::hex;

mod common;
use common::*;

#[test]
fn sign_input_p2pkh_output_p2pkh() {
    const EXPECTED_RAW_SIGNED: &str = "020000000115a009856a979cea16875cec396f5154e80d0a4c461d90516ceec6ce1da37aad000000006a47304402205b2b1633c6192c9eed16d624e43ee0c6fbc6f3bbeb9fd07f668afb37eaccff740220638dec2889d4c7129655234abe9b5647b193f30768d3719afbb065c55b2bd37c0121025d935858e4c56f24a8d634a94c678ad00b48fc86ec391c9d8215abec7e200e42ffffffff0100e1f505000000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let txid = Txid::hash(b"random");
    let vout = 0;
    // TODO: this can be done nicer
    let recipient = PubkeyHash::from(PublicKey::new(alice.public_key()));
    let satoshis = ONE_BTC * 10;

    let input = TxInputP2PKH::new(txid, vout, recipient, Some(satoshis));

    // Prepare outputs for Bob.
    let recipient = PubkeyHash::from(PublicKey::new(bob.public_key()));
    let satoshis = ONE_BTC;

    let output = TxOutputP2PKH::new(satoshis, &recipient);

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new()
        // TODO: Set return address, decrease miner fee.
        .miner_fee(ONE_BTC * 9)
        .add_input(input.into())
        .add_output(output.into())
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    assert_eq!(&hex, EXPECTED_RAW_SIGNED);

    // TODO
}
