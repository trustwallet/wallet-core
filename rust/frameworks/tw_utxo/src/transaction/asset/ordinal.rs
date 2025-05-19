use crate::script::Script;
use tw_coin_entry::error::prelude::*;
use tw_hash::H264;

pub struct OrdinalsInscription {
    pub script: bitcoin::ScriptBuf,
    pub spend_info: bitcoin::taproot::TaprootSpendInfo,
}

impl OrdinalsInscription {
    /// Creates a new Ordinals Inscription ("commit stage").
    pub fn new(mime: &[u8], data: &[u8], recipient: &H264) -> SigningResult<OrdinalsInscription> {
        // Create the envelope, containing the inscription content.
        let (script, spend_info) = create_envelope(mime, data, recipient)?;

        Ok(OrdinalsInscription { script, spend_info })
    }

    pub fn taproot_program(&self) -> Script {
        Script::from(self.script.as_script().as_bytes().to_vec())
    }
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
fn create_envelope(
    mime: &[u8],
    data: &[u8],
    pubkey: &H264,
) -> SigningResult<(bitcoin::ScriptBuf, bitcoin::taproot::TaprootSpendInfo)> {
    use bitcoin::opcodes::all::*;
    use bitcoin::opcodes::*;

    // Create MIME buffer.
    let mut mime_buf = bitcoin::script::PushBytesBuf::new();
    mime_buf
        .extend_from_slice(mime)
        .tw_err(SigningErrorType::Error_invalid_params)
        .context("Given Ordinals mime is too long")?;

    // Create an Ordinals Inscription.
    let mut builder = bitcoin::ScriptBuf::builder()
        .push_opcode(OP_FALSE)
        .push_opcode(OP_IF)
        .push_slice(b"ord")
        // Separator.
        .push_opcode(OP_PUSHBYTES_1)
        // MIME types require this additional push. It seems that the original
        // creator inadvertently used `push_slice(&[1])`, which leads to
        // `<1><1>`, which denotes a length prefix followed by the value. On the
        // other hand, for the data, `push_slice(&[])` is used, producing `<0>`.
        // This denotes a length prefix followed by no data, as opposed to
        // '<1><0>', which would be a reasonable assumption. While this appears
        // inconsistent, it's the current requirement.
        .push_opcode(OP_PUSHBYTES_1)
        // MIME type identifying the data
        .push_slice(mime_buf.as_push_bytes())
        // Separator.
        .push_opcode(OP_PUSHBYTES_0);

    // Push the actual data in chunks.
    for chunk in data.chunks(520) {
        // Create data buffer.
        let mut data_buf = bitcoin::script::PushBytesBuf::new();
        data_buf
            .extend_from_slice(chunk)
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Ordinals payload is too long")?;

        // Push buffer
        builder = builder.push_slice(data_buf);
    }

    // Finalize scripts.
    let script = builder.push_opcode(OP_ENDIF).into_script();

    // The internal key.
    let xonly = bitcoin::secp256k1::XOnlyPublicKey::from(
        bitcoin::PublicKey::from_slice(pubkey.as_slice())
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Invalid public key")?
            .inner,
    );

    // Generate the necessary spending information. As mentioned in the
    // documentation of this function at the top, this serves two purposes;
    // setting the spending condition and actually claiming the spending
    // condition.
    let spend_info = bitcoin::taproot::TaprootBuilder::new()
        .add_leaf(0, script.clone())
        .expect("Ordinals Inscription spending info must always build")
        .finalize(&secp256k1::Secp256k1::new(), xonly)
        .expect("Ordinals Inscription spending info must always build");

    Ok((script, spend_info))
}

pub struct OrdinalNftInscription(OrdinalsInscription);

impl OrdinalNftInscription {
    // Constructs an [Ordinal inscription] with a given MIME type. Common MIME
    // types are:
    // * "application/json",
    // * "application/pdf",
    // * "image/gif",
    // * "image/jpeg",
    // * "image/png",
    // * "text/plain;charset=utf-8"
    // * ...
    //
    // [Ordinal inscription]: https://docs.ordinals.com/inscriptions.html
    pub fn new(mime_type: &[u8], data: &[u8], recipient: &H264) -> SigningResult<Self> {
        OrdinalsInscription::new(mime_type, data, recipient).map(OrdinalNftInscription)
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
