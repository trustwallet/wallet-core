use super::TaprootProgram;
use crate::{Error, Result};
use bitcoin::script::{PushBytesBuf, ScriptBuf};
use bitcoin::secp256k1::XOnlyPublicKey;
use bitcoin::taproot::{TaprootBuilder, TaprootSpendInfo};
use bitcoin::{PublicKey, Script};
use tw_proto::BitcoinV2::Proto;

pub struct OrdinalsInscription {
    envelope: TaprootProgram,
}

impl OrdinalsInscription {
    /// Creates a new Ordinals Inscription ("commit stage").
    pub fn new(mime: &[u8], data: &[u8], recipient: PublicKey) -> Result<OrdinalsInscription> {
        // Create the envelope, containing the inscription content.
        let envelope = create_envelope(mime, data, recipient)?;

        Ok(OrdinalsInscription { envelope })
    }
    pub fn taproot_program(&self) -> &Script {
        self.envelope.script.as_script()
    }
    pub fn spend_info(&self) -> &TaprootSpendInfo {
        &self.envelope.spend_info
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
fn create_envelope(mime: &[u8], data: &[u8], internal_key: PublicKey) -> Result<TaprootProgram> {
    use bitcoin::opcodes::all::*;
    use bitcoin::opcodes::*;

    // Create MIME buffer.
    let mut mime_buf = PushBytesBuf::new();
    mime_buf
        .extend_from_slice(mime)
        .map_err(|_| Error::from(Proto::Error::Error_ordinal_mime_type_too_large))?;

    // Create an Ordinals Inscription.
    let mut builder = ScriptBuf::builder()
        .push_opcode(OP_FALSE)
        .push_opcode(OP_IF)
        .push_slice(b"ord")
        // Separator.
        .push_opcode(OP_PUSHBYTES_1)
        // MIME types require this addtional push. It seems that the original
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
        let mut data_buf = PushBytesBuf::new();
        data_buf
            .extend_from_slice(chunk)
            .map_err(|_| Error::from(Proto::Error::Error_ordinal_payload_too_large))?;

        // Push buffer
        builder = builder.push_slice(data_buf);
    }

    // Finalize scripts.
    let script = builder.push_opcode(OP_ENDIF).into_script();

    // Generate the necessary spending information. As mentioned in the
    // documentation of this function at the top, this serves two purposes;
    // setting the spending condition and actually claiming the spending
    // condition.
    let spend_info = TaprootBuilder::new()
        .add_leaf(0, script.clone())
        .expect("Ordinals Inscription spending info must always build")
        .finalize(
            &secp256k1::Secp256k1::new(),
            XOnlyPublicKey::from(internal_key.inner),
        )
        .expect("Ordinals Inscription spending info must always build");

    Ok(TaprootProgram { script, spend_info })
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
    pub fn new(mime_type: &[u8], data: &[u8], recipient: PublicKey) -> Result<Self> {
        OrdinalsInscription::new(mime_type, data, recipient).map(OrdinalNftInscription)
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
