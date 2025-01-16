// Automatically generated rust module for 'timestamp.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use quick_protobuf::{MessageInfo, MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Timestamp {
    pub seconds: i64,
    pub nanos: i32,
}

impl<'a> MessageRead<'a> for Timestamp {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.seconds = r.read_int64(bytes)?,
                Ok(16) => msg.nanos = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for Timestamp {
    fn get_size(&self) -> usize {
        0
        + if self.seconds == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.seconds) as u64) }
        + if self.nanos == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.nanos) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.seconds != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.seconds))?; }
        if self.nanos != 0i32 { w.write_with_tag(16, |w| w.write_int32(*&self.nanos))?; }
        Ok(())
    }
}

