use tw_bitcoin::{keypair_from_wif, TransactionBuilder, TxOutputP2TKeyPath, TxInput, TxInputP2PKH};
use tw_encoding::hex;

#[test]
fn sign_tx_output_p2tr_key_spend() {
    // One byte VarInt, size of scriptPubKey (25 bytes, hex representation of little endian encoding).
    let var_int = "19";
    // 25 bytes, the scriptPubKey for claiming.
    let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
    // Four bytes, using default value
    let sequence = "FFFFFFFF";
    // 32 bytes, reversed, little-endian encoded hash (ID) of the input transaction.
    let txid = "66e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f3";
    // Four bytes, vout value of zero.
    let vout = "00000000";
    // 20 bytes, recipient address (Base58Check of the RIPEMD160 hash of the public key).
    let recipient_address = "n3XdSggHJo1tipK4hr2wg6UvddPV2Y3kDV";

    // The full RAW TX INPUT
    let raw_tx_input =
        hex::decode(&format!("{txid}{vout}{var_int}{script_pubkey}{sequence}")).unwrap();

    // The encoded transaction *without* signature.
    static EXPECTED_NON_SIG: &str = "020000000166e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f30000000000ffffffff01c0aff629010000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";
    // The encoded transaction *with* signature.
    static EXPECTED_SIG: &str = "020000000166e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f3000000006a473044022050d3a1f393e8a27a277d0769f39f772ed35f5ddcac2d3f21e7186d45da72cd4d0220261b9daec08ef9ac34d91ccaac03b0843e21b5a40b7631b78dbf93bfa1678d530121025d935858e4c56f24a8d634a94c678ad00b48fc86ec391c9d8215abec7e200e42ffffffff01c0aff629010000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";

    // Decode the `TxInput` from the slice. We expect a `P2PKH` variant.
    let input = TxInput::from_slice(&raw_tx_input, None).unwrap();

    let my_private_key = "cUGCA4LGsXbHDpurvWM63Snk4Q1FVySw2wESD7a35mkQyEyXneRv";
    let keypair = keypair_from_wif(my_private_key).unwrap();

    let bob_pk = "cUGCA4LGsXbHDpurvWM63Snk4Q1FVySw2wESD7a35mkQyEyXneRv";
    let bob_keypair = keypair_from_wif(bob_pk).unwrap();
	let pubkey = bob_keypair.public_key();

	let output = TxOutputP2TKeyPath::new(4_999_000_000, pubkey);

    let builder = TransactionBuilder::new()
        .add_input(input)
        .add_output(output.into())
        .prepare_for_signing();

    let serialized = builder.sign_inputs(keypair).unwrap().serialize().unwrap();
    let hex_serialized = hex::encode(&serialized, false);
	println!("{hex_serialized}");
}
