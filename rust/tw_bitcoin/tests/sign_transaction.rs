use bitcoin::hashes::Hash;
use bitcoin::{
    OutPoint as BTCOutPoint, ScriptBuf as BTCScriptBuf, Sequence as BTCSequence, Txid as BTCTxid,
    Witness as BTCWitness,
};
use tw_bitcoin::{RecipientHash160, TransactionBuilder, TxInput, TxOutput, TxOutputP2PKH};
use tw_encoding::hex;

#[test]
fn sig_transaction() {
    // One byte VarInt, size of scriptPubKey (25 bytes, hex representation of little endian encoding).
    let var_int = "19";
    // 25 bytes, the scriptPubKey for claiming.
    let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
    // Four bytes, using default value
    let sequence = "FFFFFFFF";
    // 32 bytes, hash (ID) of the input transaction.
    // TODO: Find prettier way to do this.
    let mut txid =
        hex::decode("f342ee6bd3fd94e528103d7a0cc95d4882b8284fed9c727e2a221f9bd34fe466").unwrap();
    txid.reverse();
    let txid = hex::encode(&txid, false);
    // Four bytes, vout value of zero.
    let vout = "00000000";
    // 20 bytes, recipient address (Base58Check of the RIPEMD160 hash of the public key).
    let recipient_address = "n3XdSggHJo1tipK4hr2wg6UvddPV2Y3kDV";

    // The full RAW TX INPUT
    let raw_tx_input =
        hex::decode(&format!("{txid}{vout}{var_int}{script_pubkey}{sequence}")).unwrap();

	// The encoded transaction *without* signature.
    static EXPECTED_NON_SIG: &str = "020000000166e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f30000000000ffffffff0100f2052a010000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";
	// The encoded transaction *with* signature.
    static EXPECTED_SIG: &str = "020000000166e44fd39b1f222a7e729ced4f28b882485dc90c7a3d1028e594fdd36bee42f3000000006a4730440220697525358a5887731de155539f544d7592e9e18f1ab817d13fd503d69905e5bd0220318a12e9476990062dc7b115241a2e6c758ce4210460fc0dbcdd8cdc016479020121025d935858e4c56f24a8d634a94c678ad00b48fc86ec391c9d8215abec7e200e42ffffffff0100f2052a010000001976a914f173727012cef132acff9630b622a5d62508b9cc88ac00000000";

    // Decode the `TxInput` from the slice. We expect a `P2PKH` variant.
    let input = TxInput::from_slice(&raw_tx_input).unwrap();
    let output = TxOutputP2PKH::new(
        5_000_000_000,
        RecipientHash160::from_address_str(recipient_address).unwrap(),
    );

    let serialized = TransactionBuilder::new()
        .add_input(input)
        .add_output(output.into())
        .prepare_for_signing()
        .serialize()
        .unwrap();

    let hex_serialized = hex::encode(&serialized, false);

    assert_eq!(hex_serialized, EXPECTED_NON_SIG);
}
