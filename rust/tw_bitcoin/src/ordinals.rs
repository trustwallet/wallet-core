use crate::{Error, Recipient, Result};
use bitcoin::opcodes::All as AnyOpcode;
use bitcoin::script::{PushBytesBuf, ScriptBuf};
use bitcoin::secp256k1::XOnlyPublicKey;
use bitcoin::taproot::{TaprootBuilder, TaprootSpendInfo};
use bitcoin::PublicKey;

fn get_push(size: u32) -> Result<(AnyOpcode, Option<Vec<u8>>)> {
    use bitcoin::opcodes::all::*;

    let ret = match size {
        // OP_PUSHBYTES[0|1|2|...|75]
        0..=75 => (bitcoin::opcodes::All::from(size as u8), None),
        76..=255 => (OP_PUSHDATA1, Some(size.to_le_bytes().to_vec())),
        256..=65535 => (OP_PUSHDATA2, Some(size.to_le_bytes().to_vec())),
        65536..=u32::MAX => (OP_PUSHDATA4, Some(size.to_le_bytes().to_vec())),
        _ => return Err(Error::Todo),
    };

    Ok(ret)
}

/// Creates an Ordinal Inscription as defined in
/// https://docs.ordinals.com/inscriptions.html. This function is used for two
/// purposes:
/// 
/// 1. It creates the spending condition for the given `internal_key`
///    (recipient). This associates the public key of the recipient with the
///    Merkle root of the Inscription on-chain, but it does not actually reveal
///    the script to anyone ("commit stage").
/// 2. The same function can then be used by the spender/claimer to actually
///    transfer the Inscripion by sending a transaction with the Inscription
///    script in the Witness ("reveal stage").
/// 
/// Do note that the `internal_key` can be different for each stage, but it
/// could also be the same entity. Stage one, the `internal_key` is the
/// recipient. Stage two, the `internal_key` is the claimer of the transaction
/// (where the Inscription script is available in the witness).
fn create_envelope(mime: &str, data: &str, internal_key: PublicKey) -> Result<TaprootSpendInfo> {
    use bitcoin::opcodes::all::*;
    use bitcoin::opcodes::*;

    // TODO: Check overflow
    let (op_push, size_buf) = get_push(mime.len() as u32)?;

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
    data_buf
        .extend_from_slice(data.as_bytes())
        .map_err(|_| Error::Todo)?;

	// Create an Ordinal Inscription.
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

    Ok(spend_info)
}
