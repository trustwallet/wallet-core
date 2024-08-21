use crate::encode::{compact_integer::CompactInteger, stream::Stream, Encodable};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Amount(pub i64);

impl Encodable for Amount {
    fn encode(&self, stream: &mut Stream) {
        CompactInteger::from(self.0 as usize).encode(stream)
    }

    fn encoded_size(&self) -> usize {
        CompactInteger::from(self.0 as usize).encoded_size()
    }
}
