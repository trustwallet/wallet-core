use tw_proto::Utxo::Proto;

pub struct SigningInputBuilder<'a> {
    proto: Proto::SigningInput<'a>,
}

impl<'a> SigningInputBuilder<'a> {
    pub fn new() -> Self {
        SigningInputBuilder {
            proto: Proto::SigningInput {
                version: 2,
                inputs: vec![],
                outputs: vec![],
                lock_time: Proto::mod_SigningInput::OneOflock_time::None,
            },
        }
    }
    pub fn version(mut self, version: usize) -> Self {
        self.proto.version = version as i32;
        self
    }
    pub fn input<F>(mut self, f: F) -> Result<Self, ()>
    where
        F: FnOnce() -> Result<Proto::TxIn<'a>, ()>,
    {
        self.proto.inputs.push(f()?);
        Ok(self)
    }
    pub fn output<F>(mut self, f: F) -> Result<Self, ()>
    where
        F: FnOnce() -> Result<Proto::TxOut<'a>, ()>,
    {
        self.proto.outputs.push(f()?);
        Ok(self)
    }
    pub fn build(self) -> Result<Proto::SigningInput<'a>, ()> {
        Ok(self.proto)
    }
}

pub struct TxInBuilder<'a> {
    proto: Proto::TxIn<'a>,
}

impl<'a> TxInBuilder<'a> {
    pub fn new() -> Self {
        TxInBuilder {
            proto: Proto::TxIn {
                txid: Default::default(),
                vout: u32::MAX,
                sequence: u32::MAX,
                sighash: Proto::SighashType::All,
                sighash_method: Proto::mod_TxIn::OneOfsighash_method::None,
            },
        }
    }
    pub fn txid(mut self, txid: &'a [u8]) -> Self {
        self.proto.txid = txid.into();
        self
    }
    pub fn vout(mut self, vout: u32) -> Self {
        self.proto.vout = vout;
        self
    }
    pub fn sequence(mut self, sequence: u32) -> Self {
        self.proto.sequence = sequence;
        self
    }
    pub fn sighash(mut self, sighash: Proto::SighashType) -> Self {
        self.proto.sighash = sighash;
        self
    }
    pub fn legacy_method(self) -> LegacyMethodBuilder<'a> {
        LegacyMethodBuilder { proto: self.proto }
    }
    pub fn segwit_method(self) -> SegwitMethodBuilder<'a> {
        SegwitMethodBuilder {
            value: None,
            script_pubkey: None,
            proto: self.proto,
        }
    }
}

pub struct LegacyMethodBuilder<'a> {
    proto: Proto::TxIn<'a>,
}

impl<'a> LegacyMethodBuilder<'a> {
    /// The spending condition of the transaction input (_scriptPubkey_).
    pub fn spending_condition(mut self, script_pubkey: &'a [u8]) -> Self {
        self.proto.sighash_method =
            Proto::mod_TxIn::OneOfsighash_method::legacy(Proto::mod_TxIn::Legacy {
                script_pubkey: script_pubkey.into(),
            });
        self
    }
    pub fn build(self) -> Result<Proto::TxIn<'a>, ()> {
        // TODO: Also check other fields.
        if self.proto.sighash_method == Proto::mod_TxIn::OneOfsighash_method::None {
            return Err(());
        }

        Ok(self.proto)
    }
}

pub struct SegwitMethodBuilder<'a> {
    value: Option<u64>,
    script_pubkey: Option<&'a [u8]>,
    proto: Proto::TxIn<'a>,
}

impl<'a> SegwitMethodBuilder<'a> {
    pub fn value(mut self, value: u64) -> Self {
        self.value = Some(value);
        self
    }
    pub fn spending_condition(mut self, script_pubkey: &'a [u8]) -> Self {
        self.script_pubkey = Some(script_pubkey);
        self
    }
    pub fn build(mut self) -> Result<Proto::TxIn<'a>, ()> {
        self.proto.sighash_method =
            Proto::mod_TxIn::OneOfsighash_method::segwit(Proto::mod_TxIn::Segwit {
                value: self.value.ok_or(())?,
                script_pubkey: self.script_pubkey.ok_or(())?.into(),
            });

        Ok(self.proto)
    }
}

pub struct TxOutBuilder<'a> {
    proto: Proto::TxOut<'a>,
}

impl<'a> TxOutBuilder<'a> {
    pub fn new() -> Self {
        TxOutBuilder {
            proto: Proto::TxOut {
                value: u64::MAX,
                script_pubkey: Default::default(),
            },
        }
    }
    pub fn value(mut self, value: u64) -> Self {
        self.proto.value = value;
        self
    }
    pub fn spending_condition(mut self, script_pubkey: &'a [u8]) -> Self {
        self.proto.script_pubkey = script_pubkey.into();
        self
    }
    pub fn build(self) -> Result<Proto::TxOut<'a>, ()> {
        if self.proto.value == u64::MAX || self.proto.script_pubkey.is_empty() {
            return Err(());
        }

        Ok(self.proto)
    }
}
