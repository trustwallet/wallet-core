use crate::ffi::tw_build_p2wpkh_script;
use crate::Recipient;
use bitcoin::PublicKey;
use tw_proto::Bitcoin::Proto::TransactionOutput;

mod brc20_transfer;
mod scripts;
mod transaction;

/// Convenience wrapper over `tw_build_p2wpkh_script` with Protobuf
/// deserialization support.
fn ffi_build_p2wpkh_script<'a, 'b>(
    satoshis: u64,
    // We use 'b to clarify that `recipient` is not tied to the return value.
    recipient: &'b Recipient<PublicKey>,
) -> TransactionOutput<'a> {
    let pubkey = recipient.public_key().to_bytes();

    let raw = unsafe {
        tw_build_p2wpkh_script(satoshis as i64, pubkey.as_ptr(), pubkey.len()).into_vec()
    };

    let des: TransactionOutput = tw_proto::deserialize(&raw).unwrap();

    // We convert the referenced data into owned data since `raw` goes out of
    // scope at the end of the function.
    TransactionOutput {
        value: des.value,
        script: des.script.into_owned().into(),
        spendingScript: des.spendingScript.into_owned().into(),
    }
}
