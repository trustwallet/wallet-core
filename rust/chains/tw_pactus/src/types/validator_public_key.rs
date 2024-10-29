use std::str::FromStr;
use tw_coin_entry::error::prelude::TWError;
use tw_hash::H160;
use crate::encoder::{Decodable, Encodable};
use crate::encoder::error::Error;
use crate::encoder::{encode::encode_fix_slice, decode::decode_fix_slice};
use crate::types::Address;
use crate::types::address::AddressType;

pub const BLS_PUBLIC_KEY_SIZE: usize = 96;

#[derive(Debug)]
pub struct ValidatorPublicKey(pub [u8; BLS_PUBLIC_KEY_SIZE]);

impl Encodable for ValidatorPublicKey {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        encode_fix_slice(&self.0, w)
    }

    fn encoded_size(&self) -> usize {
        BLS_PUBLIC_KEY_SIZE
    }
}

impl Decodable for ValidatorPublicKey {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        Ok(ValidatorPublicKey(decode_fix_slice::<BLS_PUBLIC_KEY_SIZE>(r)?))
    }
}


impl FromStr for ValidatorPublicKey {
    type Err = TWError<String>;

    fn from_str(s: &str) -> Result<Self, TWError<String>> {
       todo!("implement me!!")
    }
}