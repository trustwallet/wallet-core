use crate::{Error, Recipient, Result, TaprootProgram, TaprootScript, TxInputP2TRScriptPath};
use bitcoin::opcodes::All as AnyOpcode;
use bitcoin::script::{PushBytesBuf, ScriptBuf};
use bitcoin::secp256k1::XOnlyPublicKey;
use bitcoin::taproot::{TaprootBuilder, TaprootSpendInfo};
use bitcoin::{PublicKey, Witness};
use serde_json::json;

/// Convenience function for retrieving the size prefix for a PUSH operation in
/// a Script/Witness. For example, the size of `5` only returns
/// `OP_PUSHBYTES_5`, while the size of value `300` returns `OP_PUSHDATA2 + LE(300)`
fn get_op_push(size: u32) -> Result<(AnyOpcode, Option<Vec<u8>>)> {
    use bitcoin::opcodes::all::*;

    let ret = match size {
        // OP_PUSHBYTES[0|1|2|...|75]
        0..=75 => (bitcoin::opcodes::All::from(size as u8), None),
        // OP_PUSHDATA[1|2|4]
        76..=255 => (OP_PUSHDATA1, Some(size.to_le_bytes().to_vec())),
        256..=65535 => (OP_PUSHDATA2, Some(size.to_le_bytes().to_vec())),
        65536..=u32::MAX => (OP_PUSHDATA4, Some(size.to_le_bytes().to_vec())),
    };

    Ok(ret)
}

pub struct OrdinalsInscription {
    envelope: TaprootProgram,
    recipient: Recipient<TaprootScript>,
}

/// Creates a new Ordinals Inscription ("commit stage").
pub fn new_ordinals_inscription(
    mime: &[u8],
    data: &[u8],
    recipient: Recipient<PublicKey>,
) -> Result<OrdinalsInscription> {
    let envelope = create_envelope(mime, data, recipient.public_key())?;
    // TODO: In which cases is this `false`?
    let merkle_root = envelope.spend_info.merkle_root().unwrap();

    Ok(OrdinalsInscription {
        envelope,
        recipient: Recipient::<TaprootScript>::from_pubkey_recipient(recipient, merkle_root),
    })
}

/// Creates an [Ordinals Inscription](https://docs.ordinals.com/inscriptions.html).
/// This function is used for two purposes:
///
/// 1. It creates the spending condition for the given `internal_key`. This
///    associates the public key of the recipient with the Merkle root of the
///    Inscription on-chain, but it does not actually reveal the script to
///    anyone ("commit stage").
/// 2. The same function can then be used by the spender/claimer to actually
///    transfer the Inscripion by sending a transaction with the Inscription
///    script in the Witness ("reveal stage").
///
/// Do note that the `internal_key` can be different for each stage, but it
/// could also be the same entity. Stage one, the `internal_key` is the
/// recipient. Stage two, the `internal_key` is the claimer of the transaction
/// (where the Inscription script is available in the Witness).
fn create_envelope(mime: &[u8], data: &[u8], internal_key: PublicKey) -> Result<TaprootProgram> {
    use bitcoin::opcodes::all::*;
    use bitcoin::opcodes::*;

    // TODO: Check overflow
    let (op_push, size_buf) = get_op_push(mime.len() as u32)?;

    // Prepare content-type buffer.
    let mut mime_buf = PushBytesBuf::new();

    // For any sizes below 75, we use encode as `OP_PUSHBYTES[0|1|2|...|75]`.
    // Fany any sized above 75, we use encode as `OP_PUSHDATA[1|2|3|4] <SIZE_BUF>`.
    mime_buf.push(op_push.to_u8()).unwrap();
    if let Some(size_buf) = size_buf {
        mime_buf.extend_from_slice(size_buf.as_slice()).unwrap();
    }

    // Prepare data buffer.
    let mut data_buf = PushBytesBuf::new();
    data_buf.extend_from_slice(data).map_err(|_| Error::Todo)?;

    // Create an Ordinals Inscription.
    let script = ScriptBuf::builder()
        .push_opcode(OP_FALSE)
        .push_opcode(OP_IF)
        // Push three bytes of "ord"
        .push_opcode(OP_PUSHBYTES_3)
        .push_slice(b"ord")
        // OP_TRUE = OP_1
        .push_opcode(OP_TRUE)
        .push_slice(mime_buf)
        .push_opcode(OP_0)
        .push_slice(data_buf)
        .push_opcode(OP_ENDIF)
        .into_script();

    // Generate the necessary spending information. As mentioned in the
    // documentation of this function at the top, this serves two purposes;
    // setting the spending condition and actually claiming the spending
    // condition.
    let spend_info = TaprootBuilder::new()
        .add_leaf(1, script)
        .map_err(|_| Error::Todo)?
        .finalize(
            &secp256k1::Secp256k1::new(),
            XOnlyPublicKey::from(internal_key.inner),
        )
        .map_err(|_| Error::Todo)?;

    Ok(TaprootProgram { script, spend_info })
}

#[cfg(test)]
mod tests {
    use super::*;
    use bitcoin::opcodes::all::*;

    #[test]
    fn test_get_op_push() {
        fn eq(size: u32, expected: AnyOpcode, has_size_le: bool) {
            dbg!(size, expected, has_size_le);

            let (op, size_le) = get_op_push(size).unwrap();
            assert_eq!(op, expected);
            if has_size_le {
                assert_eq!(size_le.unwrap().as_slice(), size.to_le_bytes());
            }
        }

        // Up to 75, only the OP_PUSHBYTES_* opcode is returned.
        eq(0, OP_PUSHBYTES_0, false);
        eq(1, OP_PUSHBYTES_1, false);
        eq(2, OP_PUSHBYTES_2, false);
        eq(75, OP_PUSHBYTES_75, false);

        // From 76 to 255, it returns a OP_PUSHDATA_1 + SIZE (LE).
        eq(76, OP_PUSHDATA1, true);
        eq(255, OP_PUSHDATA1, true);

        // From 256 to 65535, it returns a OP_PUSHDATA_2 + SIZE (LE).
        eq(256, OP_PUSHDATA2, true);
        eq(65535, OP_PUSHDATA2, true);

        // From 65536 to 2^32-1, it returns a OP_PUSHDATA_4 + SIZE (LE).
        eq(65536, OP_PUSHDATA4, true);
        eq(u32::MAX, OP_PUSHDATA4, true);
    }
}
