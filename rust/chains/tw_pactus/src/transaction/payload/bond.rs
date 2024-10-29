use crate::encoder::error::Error as EncoderError;
use crate::{
    encoder::{Decodable, Encodable},
    types::{Address, Amount, ValidatorPublicKey},
};

use super::{Payload, PayloadType};

pub const BLS_PUBLIC_KEY_SIZE: usize = 96;

#[derive(Debug)]
pub struct BondPayload {
    sender: Address,
    receiver: Address,
    stake: Amount,
    public_key: Option<ValidatorPublicKey>,
}

impl BondPayload {
    pub fn new(sender: Address, receiver: Address, stake: Amount, public_key: Option<ValidatorPublicKey>) -> Self {
        BondPayload {
            sender,
            receiver,
            stake,
            public_key,
        }
    }
}

impl Encodable for BondPayload {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), EncoderError> {
        self.sender.encode(w)?;
        self.receiver.encode(w)?;

        match self.public_key {
            Some(ref public_key) => {
                BLS_PUBLIC_KEY_SIZE as u8.encode(w)?;
                public_key.encode(w)?;
            }
            None => {
                0u8.encode(w)?;
            }
        }

        self.stake.encode(w)?;
        Ok(())
    }

    fn encoded_size(&self) -> usize {
        self.sender.encoded_size() +
            self.receiver.encoded_size() +
            self.stake.encoded_size() +
            match self.public_key {
                Some(ref public_key) => {
                    1 + public_key.encoded_size()
                }
                None => {
                    1
                }
            }
    }
}

impl Decodable for BondPayload {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, EncoderError> {
        let sender = Address::decode(r)?;
        let receiver = Address::decode(r)?;
        let stake = Amount::decode(r)?;
        let mut public_key = None;

        let public_key_size: u8 = u8::decode(r)?;

        if public_key_size == BLS_PUBLIC_KEY_SIZE as u8 {
            public_key = Some(ValidatorPublicKey::decode(r)?);
        } else if public_key_size != 0 {
            return Err(EncoderError::ParseFailed("invalid public key size"));
        }

        Ok(BondPayload {
            sender,
            receiver,
            stake,
            public_key,
        })
    }
}

impl Payload for BondPayload {
    fn signer(&self) -> &Address {
        &self.sender
    }
    fn value(&self) -> Amount {
        self.stake.clone()
    }
    fn payload_type(&self) -> PayloadType {
        PayloadType::Bond
    }
}
