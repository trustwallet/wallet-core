use std::io;

use crate::encode::{var_int::VarInt, Encodable};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Amount(pub i64);

impl Encodable for Amount {
    fn encode<W: std::io::Write + ?Sized>(&self, w: &mut W) -> Result<usize, io::Error> {
        VarInt::from(self.0 as usize).encode(w)
    }

    fn encoded_size(&self) -> usize {
        VarInt::from(self.0 as usize).encoded_size()
    }
}
