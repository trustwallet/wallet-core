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
use crate::encoder::error::Error as EncoderError;
use crate::encoder::{deserialize, Decodable, Encodable};

const VERSION_LATEST: u8 = 1;
const FLAG_NOT_SIGNED: u8 = 0x02;

#[derive(Debug, Clone)]
pub enum PayloadType {
    Transfer = 1,
    Bond = 2,
    Sortition = 3,
    Unbond = 4,
    Withdraw = 5,
}

impl TryFrom<u8> for PayloadType {
    type Error = EncoderError;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            1 => Ok(PayloadType::Transfer),
            2 => Ok(PayloadType::Bond),
            3 => Ok(PayloadType::Sortition),
            4 => Ok(PayloadType::Unbond),
            5 => Ok(PayloadType::Withdraw),
            _ => Err(EncoderError::ParseFailed("Invalid PayloadType value")),
        }
    }
}

impl Encodable for PayloadType {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), EncoderError> {
        (self.clone() as u8).encode(w)
    }

    fn encoded_size(&self) -> usize {
        1
    }
}

impl Decodable for PayloadType {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, EncoderError> {
        PayloadType::try_from(u8::decode(r)?)
    }
}

pub trait Payload: Debug + Encodable {
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

impl Encodable for TransferPayload {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), EncoderError> {
        self.sender.encode(w)?;
        self.receiver.encode(w)?;
        self.amount.encode(w)?;

        Ok(())
    }

    fn encoded_size(&self) -> usize {
        self.sender.encoded_size() + self.receiver.encoded_size() + self.amount.encoded_size()
    }
}

impl Decodable for TransferPayload {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, EncoderError> {
        let sender = Address::decode(r)?;
        let receiver = Address::decode(r)?;
        let amount = Amount::decode(r)?;

        Ok(TransferPayload {
            sender,
            receiver,
            amount,
        })
    }
}

impl Payload for TransferPayload {
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
    flags: u8,
    version: u8,
    lock_time: u32,
    fee: Amount,
    memo: String,
    payload: Box<dyn Payload>,
    signature: Option<Signature>,
    public_key: Option<PublicKey>,
}

impl Transaction {
    pub fn new(
        flags: u8,
        version: u8,
        lock_time: u32,
        fee: Amount,
        memo: String,
        payload: Box<dyn Payload>,
    ) -> Self {
        Transaction {
            flags,
            version,
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
                        let sender = Address::from_str(&pld.sender)?;
                        let receiver = Address::from_str(&pld.receiver)?;

                        Box::new(TransferPayload {
                            sender,
                            receiver,
                            amount: Amount(pld.amount),
                        })
                    },
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::bond(_pld) => {
                        return SigningError::err(SigningErrorType::Error_not_supported)
                    },
                    Pactus::Proto::mod_TransactionMessage::OneOfpayload::None => {
                        return SigningError::err(SigningErrorType::Error_invalid_params)
                    },
                };

                let flags = FLAG_NOT_SIGNED;
                let version = VERSION_LATEST;
                Ok(Transaction::new(
                    flags,
                    version,
                    trx.lock_time,
                    Amount(trx.fee),
                    trx.memo.to_string(),
                    payload,
                ))
            },
        }
    }

    pub fn from_bytes(input: &[u8]) -> SigningResult<Self> {
        Ok(deserialize::<Transaction>(input)?)
    }

    pub fn sign(&mut self, private_key: &PrivateKey) -> SigningResult<Signature> {
        let sign_bytes = self.sign_bytes()?;
        let signature = private_key.sign(sign_bytes)?;

        self.set_signatory(private_key.public(), signature.clone());

        Ok(signature)
    }

    pub fn set_signatory(&mut self, public_key: PublicKey, signature: Signature) {
        // Unset "Not Signed" flag
        self.flags &= !FLAG_NOT_SIGNED;

        self.public_key = Some(public_key);
        self.signature = Some(signature);
    }

    pub fn id(&self) -> Vec<u8> {
        blake2_b(&self.sign_bytes().unwrap_or_default(), 32).unwrap_or_default()
    }

    pub fn to_bytes(&self) -> SigningResult<Vec<u8>> {
        let mut w = Vec::with_capacity(self.encoded_size());

        self.encode(&mut w)?;

        Ok(w.to_vec())
    }

    pub fn sign_bytes(&self) -> SigningResult<Vec<u8>> {
        let mut w = Vec::new();
        self.encode_with_no_signatory(&mut w)?;
        let mut sign_bytes = w.to_vec();
        sign_bytes.remove(0); // Remove flags

        Ok(sign_bytes)
    }

    fn encode_with_no_signatory(&self, w: &mut dyn std::io::Write) -> Result<(), EncoderError> {
        self.flags.encode(w)?;
        self.version.encode(w)?;
        self.lock_time.encode(w)?;
        self.fee.encode(w)?;
        self.memo.encode(w)?;
        self.payload.payload_type().encode(w)?;
        self.payload.encode(w)?;

        Ok(())
    }

    fn is_signed(&self) -> bool {
        self.flags & FLAG_NOT_SIGNED == 0
    }
}

impl Encodable for Transaction {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), EncoderError> {
        self.encode_with_no_signatory(w)?;

        if let Some(sig) = &self.signature {
            sig.encode(w)?;
        }

        if let Some(pub_key) = &self.public_key {
            pub_key.encode(w)?;
        }

        Ok(())
    }

    fn encoded_size(&self) -> usize {
        let mut len = self.flags.encoded_size()
            + self.version.encoded_size()
            + self.lock_time.encoded_size()
            + self.payload.payload_type().encoded_size()
            + self.fee.encoded_size()
            + self.memo.encoded_size()
            + self.payload.encoded_size();

        if let Some(sig) = &self.signature {
            len += sig.encoded_size();
        }

        if let Some(pub_key) = &self.public_key {
            len += pub_key.encoded_size();
        }

        len
    }
}

impl Decodable for Transaction {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, EncoderError> {
        let flags = u8::decode(r)?;
        let version = u8::decode(r)?;
        let lock_time = u32::decode(r)?;
        let fee = Amount::decode(r)?;
        let memo = String::decode(r)?;
        let payload_type = PayloadType::decode(r)?;
        let payload = match payload_type {
            PayloadType::Transfer => Box::new(TransferPayload::decode(r)?),
            _ => return Err(EncoderError::ParseFailed("Unsupported payload")),
        };

        let mut trx = Transaction::new(flags, version, lock_time, fee, memo, payload);

        if !trx.is_signed() {
            return Ok(trx);
        }

        let signature = Signature::decode(r)?;
        let public_key = PublicKey::decode(r)?;

        trx.signature = Some(signature);
        trx.public_key = Some(public_key);

        Ok(trx)
    }
}

#[cfg(test)]
mod tests {
    use std::str::FromStr;

    use tw_encoding::hex::DecodeHex;

    use super::*;

    #[test]
    fn test_payload_type_encoding() {
        let mut stream = Vec::new();

        let payload = PayloadType::Unbond;
        payload.encode(&mut stream).unwrap();
        assert_eq!(stream.to_vec(), &[4]);
    }

    const TRANSACTION_NOT_SIGNED: &str = concat!(
        "02",                                         // Flags
        "01",                                         // Version
        "01020300",                                   // LockTime
        "e807",                                       // Fee
        "0474657374",                                 // Memo
        "01",                                         // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b", // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21", // Receiver
        "a09c01"                                      // Amount
    );

    const TRANSACTION_SIGNED: &str = concat!(
        "00",                                                               // Flags
        "01",                                                               // Version
        "01020300",                                                         // LockTime
        "e807",                                                             // Fee
        "0474657374",                                                       // Memo
        "01",                                                               // PayloadType
        "037098338e0b6808119dfd4457ab806b9c2059b89b",                       // Sender
        "037a14ae24533816e7faaa6ed28fcdde8e55a7df21",                       // Receiver
        "a09c01",                                                           // Amount
        "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47ff", // Signature
        "f027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09",
        "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa" // PublicKey
    );

    const TX_ID: &str = "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311";

    #[test]
    fn test_sign_signature() {
        let expected_data = TRANSACTION_SIGNED.decode_hex().unwrap();
        let expected_id = TX_ID.decode_hex().unwrap();

        let sender = Address::from_str("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr").unwrap();
        let receiver = Address::from_str("pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra").unwrap();
        let payload = Box::new(TransferPayload {
            sender,
            receiver,
            amount: Amount(20000),
        });
        let mut trx = Transaction::new(
            FLAG_NOT_SIGNED,
            VERSION_LATEST,
            0x00030201,
            Amount(1000),
            "test".to_string(),
            payload,
        );

        let private_key_data = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6"
            .decode_hex()
            .unwrap();
        let private_key = PrivateKey::try_from(private_key_data.as_slice()).unwrap();
        trx.sign(&private_key).unwrap();

        assert_eq!(expected_data, trx.to_bytes().unwrap());
        assert_eq!(expected_id, trx.id());
    }

    #[test]
    fn test_encoding_not_signed() {
        let data = TRANSACTION_NOT_SIGNED.decode_hex().unwrap();
        let trx = Transaction::from_bytes(&data).unwrap();
        let expected_id = TX_ID.decode_hex().unwrap();

        let encoded_data = trx.to_bytes().unwrap();

        assert_eq!(encoded_data, data);
        assert_eq!(expected_id, trx.id());
        assert_eq!(trx.encoded_size(), data.len());
        assert!(!trx.is_signed());
    }

    #[test]
    fn test_encoding_signed() {
        let data = TRANSACTION_SIGNED.decode_hex().unwrap();
        let trx = Transaction::from_bytes(&data).unwrap();
        let expected_id = TX_ID.decode_hex().unwrap();

        let encoded_data = trx.to_bytes().unwrap();

        assert_eq!(encoded_data, data);
        assert_eq!(expected_id, trx.id());
        assert_eq!(trx.encoded_size(), data.len());
        assert!(trx.is_signed());
    }
}
