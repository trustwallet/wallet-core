use tw_proto::Utxo::Proto;

pub struct SigningInputBuilder {
    proto: Proto::SigningInput<'static>,
}

impl SigningInputBuilder {
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
                signing_method: Proto::mod_TxIn::OneOfsigning_method::None,
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
}

pub struct LegacyMethodBuilder<'a> {
    proto: Proto::TxIn<'a>,
}

impl<'a> LegacyMethodBuilder<'a> {
    pub fn spending_condition(mut self, script_pubkey: &'a [u8]) -> Self {
        self.proto.signing_method =
            Proto::mod_TxIn::OneOfsigning_method::legacy(Proto::mod_TxIn::Legacy {
                script_pubkey: script_pubkey.into(),
            });
        self
    }
    pub fn build(self) -> Result<Proto::TxIn<'a>, ()> {
        // TODO: Also check other fields.
        if self.proto.signing_method == Proto::mod_TxIn::OneOfsigning_method::None {
            return Err(());
        }

        Ok(self.proto)
    }
}
