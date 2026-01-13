use crate::encoder::{error::Error, var_int::VarInt, Decodable, Encodable};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Amount(pub i64);

impl Encodable for Amount {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        let amount: usize = match self.0.try_into() {
            Ok(amount) => amount,
            Err(_) => {
                return Err(Error::IoError(std::io::Error::from(
                    std::io::ErrorKind::InvalidInput,
                )));
            },
        };
        VarInt::from(amount).encode(w)
    }

    fn encoded_size(&self) -> usize {
        VarInt::from(self.0 as usize).encoded_size()
    }
}

impl Decodable for Amount {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        Ok(Amount(*VarInt::decode(r)? as i64))
    }
}
