use std::fmt::Debug;
use std::str::FromStr;

use tw_coin_entry::error::prelude::{SigningError, SigningErrorType, SigningResult};
use tw_hash::blake2::blake2_b;
use tw_keypair::ed25519::sha512::{PrivateKey, PublicKey};
use tw_keypair::ed25519::Signature;
use tw_keypair::traits::SigningKeyTrait;
use tw_proto::Pactus;

use crate::address::Address;
use crate::amount::Amount;
use crate::encode::stream::Stream;
use crate::encode::Encodable;

const VERSION_LATEST: u8 = 1;

#[derive(Debug, Clone)]
pub enum PayloadType {
    Transfer = 1,
    Bond = 2,
    Sortition = 3,
    Unbond = 4,
    Withdraw = 5,
}

impl Encodable for PayloadType {
    fn encode(&self, stream: &mut Stream) {
        (self.clone() as u8).encode(stream)
    }

    fn encoded_size(&self) -> usize {
        1
    }
}

pub trait Payload: Debug {
    fn encode(&self, stream: &mut Stream);
    fn encoded_size(&self) -> usize;
    fn signer(&self) -> &Address;
    fn value(&self) -> Amount;
    fn payload_type(&self) -> PayloadType;
}

#[derive(Debug)]
pub struct TransferPayload {
    sender: Address,
    receiver: Address,
    amount: Amount,
}

impl Payload for TransferPayload {
    fn encode(&self, stream: &mut Stream) {
        self.sender.encode(stream);
        self.receiver.encode(stream);
        self.amount.encode(stream);
    }

    fn encoded_size(&self) -> usize {
        self.sender.encoded_size() + self.receiver.encoded_size() + self.amount.encoded_size()
    }

    fn signer(&self) -> &Address {
        &self.sender
    }
    fn value(&self) -> Amount {
        self.amount.clone()
    }
    fn payload_type(&self) -> PayloadType {
        PayloadType::Transfer
    }
}

#[derive(Debug)]
pub struct Transaction {
    pub flags: u8,
    pub version: u8,
    pub lock_time: u32,
    pub fee: Amount,
    pub memo: String,
    pub payload: Box<dyn Payload>,
    pub signature: Option<Signature>,
    pub public_key: Option<PublicKey>,
}

impl Transaction {
    pub fn new(lock_time: u32, fee: Amount, memo: String, payload: Box<dyn Payload>) -> Self {
        Transaction {
            flags: 0,
            version: VERSION_LATEST,
            lock_time,
            fee,
            memo,
            payload,
            public_key: None,
            signature: None,
        }
    }

    pub fn from_proto(input: &Pactus::Proto::SigningInput) -> SigningResult<Self> {
        match &input.transaction {
            None => SigningError::err(SigningErrorType::Error_invalid_params),
            Some(trx) => {
                let payload = match &trx.payload {
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::transfer(pld) => {
                        let private_key = PrivateKey::try_from(input.private_key.as_ref())?;
                        let sender = Address::from_public_key(&private_key.public())?;
                        let receiver = Address::from_str(&pld.receiver)?;

                        Box::new(TransferPayload {
                            sender,
                            receiver,
                            amount: Amount(pld.amount),
                        })
                    }
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::bond(_pld) => {
                        return SigningError::err(SigningErrorType::Error_not_supported)
                    }
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::None => {
                        return SigningError::err(SigningErrorType::Error_invalid_params)
                    }
                };

                Ok(Transaction {
                    flags: 0,
                    version: VERSION_LATEST,
                    lock_time: trx.lock_time,
                    fee: Amount(trx.fee),
                    memo: trx.memo.to_string(),
                    payload,
                    public_key: None,
                    signature: None,
                })
            }
        }
    }

    pub fn sign(&mut self, private_key: &PrivateKey) -> SigningResult<()> {
        let sign_bytes = self.sign_bytes();
        let signature = private_key.sign(sign_bytes)?;

        self.set_signatory(private_key.public(), signature);

        Ok(())
    }

    pub fn set_signatory(&mut self, public_key: PublicKey, signature: Signature) {
        self.public_key = Some(public_key);
        self.signature = Some(signature);
    }

    pub fn id(&self) -> Vec<u8> {
        blake2_b(&self.sign_bytes(), 32).unwrap_or_default()
    }

    pub fn to_bytes(&self) -> Vec<u8> {
        let mut stream = Stream::new();

        self.flags.encode(&mut stream);
        stream.append_raw_slice(&self.sign_bytes());

        if let Some(signature) = &self.signature {
            signature.encode(&mut stream)
        }

        if let Some(public_key) = &self.public_key {
            public_key.encode(&mut stream)
        }

        stream.out()
    }

    fn sign_bytes(&self) -> Vec<u8> {
        let mut stream = Stream::new();

        self.version.encode(&mut stream);
        self.lock_time.encode(&mut stream);
        self.fee.encode(&mut stream);
        self.memo.encode(&mut stream);
        self.payload.payload_type().encode(&mut stream);
        self.payload.encode(&mut stream);

        stream.out()
    }
}

#[cfg(test)]
mod tests {
    use std::str::FromStr;

    use tw_encoding::hex::DecodeHex;

    use super::*;

    #[test]
    fn test_payload_type_encoding() {
        let mut stream = Stream::new();

        let payload = PayloadType::Unbond;
        payload.encode(&mut stream);
        assert_eq!(stream.out(), &[4]);
    }

    #[test]
    fn test_encoding() {
        let data_hex = "00".to_string()+ // Flags
            "01"+ // Version
            "01020304"+ // LockTime
            "01"+ // Fee
            "00" +// Memo
            "01" +// PayloadType
            "013333333333333333333333333333333333333333"+ // Sender
            "012222222222222222222222222222222222222222"+ // Receiver
            "01"; // Amount

        let expected_data = data_hex.as_str().decode_hex().unwrap();
        let expected_id = "1a8cedbb2ffce29df63210f112afb1c0295b27e2162323bfc774068f0573388e"
            .decode_hex()
            .unwrap();

        let sender = Address::from_str("pc1pxvenxvenxvenxvenxvenxvenxvenxven250tr7").unwrap();
        let receiver = Address::from_str("pc1pyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zc2ckkv").unwrap();
        let payload = Box::new(TransferPayload {
            sender,
            receiver,
            amount: Amount(1),
        });
        let trx = Transaction::new(0x04030201, Amount(1), "".to_string(), payload);

        assert_eq!(expected_data, trx.to_bytes());
        assert_eq!(expected_id, trx.id());
    }
}
