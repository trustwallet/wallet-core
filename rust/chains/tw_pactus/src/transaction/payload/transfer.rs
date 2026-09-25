use crate::encoder::error::Error as EncoderError;
use crate::{
    encoder::{Decodable, Encodable},
    types::{Address, Amount},
};

use super::{Payload, PayloadType};

#[derive(Debug)]
pub struct TransferPayload {
    sender: Address,
    receiver: Address,
    amount: Amount,
}

impl TransferPayload {
    pub fn new(sender: Address, receiver: Address, amount: Amount) -> Self {
        TransferPayload {
            sender,
            receiver,
            amount,
        }
    }
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
