use bitcoin::hashes::Hash;
use bitcoin::{
    OutPoint as BOutPoint, ScriptBuf as BScriptBuf, Sequence as BSequence, Txid as BTxid,
    Witness as BWitness,
};
use tw_bitcoin::{PubkeyHash, TxInput};
use tw_encoding::hex;

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
        /*
        assert_eq!(
            p2pkh.recipient,
            PubkeyHash::from_bytes(hex::decode(recipient_hash).unwrap().try_into().unwrap())
                .unwrap()
        );
         */
    }

    //println!("{:?}", x);
}
