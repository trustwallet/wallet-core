use bitcoin::ScriptBuf;
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
                amount: u64::MAX,
                script_pubkey: Default::default(),
                sighash: Proto::SighashType::All,
                signing_method: Proto::SigningMethod::Legacy,
                leaf_hash: Default::default(),
                one_prevout: false,
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
    pub fn value(mut self, amount: u64) -> Self {
        self.proto.amount = amount;
        self
    }
    pub fn spending_condition(mut self, script_pubkey: &'a [u8]) -> Self {
        self.proto.script_pubkey = script_pubkey.into();
        self
    }
    pub fn sighash(mut self, sighash: Proto::SighashType) -> Self {
        self.proto.sighash = sighash;
        self
    }
    pub fn signing_method(mut self, method: Proto::SigningMethod) -> Self {
        self.proto.signing_method = method;
        self
    }
    pub fn build(self) -> Result<Proto::TxIn<'a>, ()> {
        Ok(self.proto)
    }
}

pub enum PrevoutVariant {
    One(Prevout),
    All(Vec<Prevout>),
}

pub struct Prevout {
    value: u64,
    script_pubkey: ScriptBuf,
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
