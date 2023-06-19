use crate::ffi::{tw_build_p2pkh_script, tw_build_p2tr_key_path_script, tw_build_p2wpkh_script};
use crate::Recipient;
use bitcoin::PublicKey;
use std::borrow::Cow;
use tw_proto::Bitcoin::Proto::{
    OutPoint, SigningInput, TransactionOutput, TransactionPlan, TransactionVariant,
    UnspentTransaction,
};

mod brc20_transfer;
mod scripts;
mod transaction;

/// Convenience function for reversing the Txid before it's being passed on to
/// the FFI.
fn reverse_txid(txid: &str) -> Vec<u8> {
    tw_encoding::hex::decode(txid)
        .unwrap()
        .into_iter()
        .rev()
        .collect()
}

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

/// Convenience wrapper over `tw_build_p2pkh_script` with Protobuf
/// deserialization support.
fn ffi_build_p2pkh_script<'a, 'b>(
    satoshis: u64,
    // We use 'b to clarify that `recipient` is not tied to the return value.
    recipient: &'b Recipient<PublicKey>,
) -> TransactionOutput<'a> {
    let pubkey = recipient.public_key().to_bytes();

    let raw =
        unsafe { tw_build_p2pkh_script(satoshis as i64, pubkey.as_ptr(), pubkey.len()).into_vec() };

    let des: TransactionOutput = tw_proto::deserialize(&raw).unwrap();

    // We convert the referenced data into owned data since `raw` goes out of
    // scope at the end of the function.
    TransactionOutput {
        value: des.value,
        script: des.script.into_owned().into(),
        spendingScript: des.spendingScript.into_owned().into(),
    }
}

/// Convenience wrapper over `tw_build_p2pkh_script` with Protobuf
/// deserialization support.
fn ffi_build_p2tr_key_path_script<'a, 'b>(
    satoshis: u64,
    // We use 'b to clarify that `recipient` is not tied to the return value.
    recipient: &'b Recipient<PublicKey>,
) -> TransactionOutput<'a> {
    let pubkey = recipient.public_key().to_bytes();

    let raw = unsafe {
        tw_build_p2tr_key_path_script(satoshis as i64, pubkey.as_ptr(), pubkey.len()).into_vec()
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

struct ProtoSigningInputBuilder<'a> {
    inner: SigningInput<'a>,
}

impl<'a> ProtoSigningInputBuilder<'a> {
    fn new() -> Self {
        let signing = SigningInput {
            plan: Some(TransactionPlan::default()),
            ..Default::default()
        };

        ProtoSigningInputBuilder { inner: signing }
    }
    fn private_key(mut self, privkey: &'a [u8]) -> Self {
        self.inner.private_key = vec![Cow::from(privkey)];
        self
    }
    fn input(mut self, tx: UnspentTransaction<'a>) -> Self {
        self.inner.utxo.push(tx);
        self
    }
    fn output(mut self, tx: UnspentTransaction<'a>) -> Self {
        self.inner.plan.as_mut().unwrap().utxos.push(tx);
        self
    }
    fn build(self) -> SigningInput<'a> {
        self.inner
    }
}

struct ProtoTransactionBuilder<'a> {
    inner: UnspentTransaction<'a>,
}

impl<'a> ProtoTransactionBuilder<'a> {
    fn new() -> Self {
        let unspent = UnspentTransaction {
            out_point: Some(OutPoint::default()),
            ..Default::default()
        };

        ProtoTransactionBuilder { inner: unspent }
    }
    fn txid(mut self, slice: &'a [u8]) -> Self {
        self.inner.out_point.as_mut().unwrap().hash = slice.into();
        self
    }
    fn vout(mut self, vout: u32) -> Self {
        self.inner.out_point.as_mut().unwrap().index = vout;
        self
    }
    fn variant(mut self, variant: TransactionVariant) -> Self {
        self.inner.variant = variant;
        self
    }
    fn satoshis(mut self, satoshis: u64) -> Self {
        self.inner.amount = satoshis as i64;
        self
    }
    fn script_pubkey(mut self, script: &'a [u8]) -> Self {
        self.inner.script = script.into();
        self
    }
    fn spending_script(mut self, script: &'a [u8]) -> Self {
        self.inner.spendingScript = script.into();
        self
    }
    fn build(self) -> UnspentTransaction<'a> {
        self.inner
    }
}
