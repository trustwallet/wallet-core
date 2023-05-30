use bitcoin::hashes::Hash;
use bitcoin::{
    OutPoint as BTCOutPoint, ScriptBuf as BTCScriptBuf, Sequence as BTCSequence, Txid as BTCTxid,
    Witness as BTCWitness,
};
use tw_bitcoin::{RecipientHash160, TxInput};
use tw_encoding::hex;

#[test]
fn tx_input_p2pkh_from_slice() {
    // One byte VarInt, size of scriptPubKey (25 bytes, hex representation of little endian encoding).
    let var_int = "19";
    // 25 bytes, the scriptPubKey for claiming.
    let script_pubkey = "76a9143836741d8f4a925483cba7634aa3ed0ddd37c54e88ac";
    // Four bytes, using default value
    let sequence = "FFFFFFFF";
    // 32 bytes, hash (ID) of the input transaction.
    let txid = "f342ee6bd3fd94e528103d7a0cc95d4882b8284fed9c727e2a221f9bd34fe466";
    // Four bytes, vout value of zero.
    let vout = "00000000";
    // 20 bytes, RIPEMD160 hash of the recipient.
    let recipient_hash = "3836741d8f4a925483cba7634aa3ed0ddd37c54e";

    // The full RAW TX INPUT
    let raw_tx_input =
        hex::decode(&format!("{txid}{vout}{var_int}{script_pubkey}{sequence}")).unwrap();

    // Decode the `TxInput` from the slice. We expect a `P2PKH` variant.
    let tx_input = TxInput::from_slice(&raw_tx_input).unwrap();
    if let TxInput::P2PKH(p2pkh) = tx_input {
        // Expected `previous_output`
        assert_eq!(
            p2pkh.ctx.previous_output,
            BTCOutPoint {
                txid: BTCTxid::from_byte_array(hex::decode(txid).unwrap().try_into().unwrap()),
                vout: 0,
            }
        );
        // Expected `script_pubkey`
        assert_eq!(
            p2pkh.ctx.script_pubkey,
            BTCScriptBuf::from_hex(script_pubkey).unwrap()
        );
        // Expected sequence
        assert_eq!(p2pkh.ctx.sequence, BTCSequence::default());
        // Expected witness
        assert_eq!(p2pkh.ctx.witness, BTCWitness::default());
        // Expected recipient.
        assert_eq!(
            p2pkh.recipient,
            RecipientHash160::from_bytes(hex::decode(recipient_hash).unwrap().try_into().unwrap())
        );
    }

    //println!("{:?}", x);
}
