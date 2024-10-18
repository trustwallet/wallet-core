mod transfer;

pub use transfer::TransferPayload;

use std::fmt::Debug;

use crate::encoder::error::Error as EncoderError;
use crate::{
    encoder::{Decodable, Encodable},
    types::{Address, Amount},
};

#[derive(Debug, Clone, Copy)]
#[repr(u8)]
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
        (*self as u8).encode(w)
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
