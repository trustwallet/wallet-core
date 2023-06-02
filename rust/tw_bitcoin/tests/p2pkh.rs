use std::str::FromStr;

use bitcoin::hashes::Hash;
use bitcoin::{
    OutPoint as BOutPoint, ScriptBuf as BScriptBuf, Sequence as BSequence, Txid as BTxid,
    Witness as BWitness, TxOut, PublicKey
};
use tw_bitcoin::{PubkeyHash, TxInput, TxInputP2PKH, keypair_from_wif, pubkey_hash_from_address, TransactionBuilder, TxOutputP2PKH};
use tw_encoding::hex;

const ONE_BTC: u64 = 100_000_000;
const ALICE_WIF: &str = "cUGCA4LGsXbHDpurvWM63Snk4Q1FVySw2wESD7a35mkQyEyXneRv";
const BOB_WIF: &str = "cV3Fp99spihU4t28JwEggbywmKXEY9KKX9PwtRqHaZnzCrvnF4yr";

#[test]
fn sign_input_p2pkh_output_p2pkh() {
    const EXPECTED_RAW_SIGNED: &str = "020000000115a009856a979cea16875cec396f5154e80d0a4c461d90516ceec6ce1da37aad000000006a47304402205b2b1633c6192c9eed16d624e43ee0c6fbc6f3bbeb9fd07f668afb37eaccff740220638dec2889d4c7129655234abe9b5647b193f30768d3719afbb065c55b2bd37c0121025d935858e4c56f24a8d634a94c678ad00b48fc86ec391c9d8215abec7e200e42ffffffff0100e1f505000000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Prepare inputs for Alice.
    let txid = BTxid::hash(b"random");
    let vout = 0;
    // TODO: this can be done nicer
    let recipient = PubkeyHash::from(PublicKey::new(alice.public_key()));
    let satoshis = ONE_BTC * 10;

    let input = TxInputP2PKH::new(txid, vout, recipient, satoshis);

    // Prepare outputs for Bob.
    let recipient = PubkeyHash::from(PublicKey::new(bob.public_key()));
    let satoshis = ONE_BTC;

    let output = TxOutputP2PKH::new(satoshis, &recipient);

    // Alice signs the transaction.
    let signed_transaction = TransactionBuilder::new()
        .add_input(input.into())
        .add_output(output.into())
        .prepare_for_signing()
        .sign_inputs(alice)
        .unwrap()
        .serialize()
        .unwrap();

    let hex = hex::encode(signed_transaction, false);
    assert_eq!(&hex, EXPECTED_RAW_SIGNED);
}

#[test]
fn tx_input_p2pkh_from_slice() {
    // One byte VarInt, size of scriptPubKey (25 bytes, hex representation of little endian encoding).
    let var_int = "19";
    // 25 bytes, the scriptPubKey for claiming.
    let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
    // Four bytes, using default value
    let sequence = "FFFFFFFF";
    // 32 bytes, reversed, little-endian encoded hash (ID) of the input transaction.
    // TODO: Check ordianess in asserts at the end.
    let _txid_original = "f342ee6bd3fd94e528103d7a0cc95d4882b8284fed9c727e2a221f9bd34fe466";
    let txid = "66e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f3";
    // Four bytes, vout value of zero.
    let vout = "00000000";
    // 20 bytes, RIPEMD160 hash of the recipient.
    let recipient_hash = "3836741d8f4a925483cba7634aa3ed0ddd37c54e";

    // The full RAW TX INPUT
    let raw_tx_input =
        hex::decode(&format!("{txid}{vout}{var_int}{script_pubkey}{sequence}")).unwrap();

    // Decode the `TxInput` from the slice. We expect a `P2PKH` variant.
    let tx_input = TxInput::from_slice(&raw_tx_input, None).unwrap();
    if let TxInput::P2PKH(p2pkh) = tx_input {
        // Expected `previous_output`
        assert_eq!(
            p2pkh.ctx.previous_output,
            BOutPoint {
                txid: BTxid::from_byte_array(hex::decode(txid).unwrap().try_into().unwrap()),
                vout: 0,
            }
        );
        // Expected `script_pubkey`
        assert_eq!(
            p2pkh.ctx.script_pubkey,
            BScriptBuf::from_hex(script_pubkey).unwrap()
        );
        // Expected sequence
        assert_eq!(p2pkh.ctx.sequence, BSequence::default());
        // Expected witness
        assert_eq!(p2pkh.ctx.witness, BWitness::default());
        // Expected recipient.
        assert_eq!(
            p2pkh.recipient,
            //pubkey_hash_from_string(recipient_hash).unwrap()
            PubkeyHash::from_str(recipient_hash).unwrap(),
        );
    }

    //println!("{:?}", x);
}
