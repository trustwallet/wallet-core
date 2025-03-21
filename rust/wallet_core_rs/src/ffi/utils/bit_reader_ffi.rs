// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use bitreader::{BitReader, BitReaderError};
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_result::{CUInt8Result, ErrorCode};
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::Data;
use tw_misc::try_or_else;

#[derive(Debug, Eq, PartialEq)]
#[repr(C)]
pub enum CBitReaderCode {
    Ok = 0,
    /// Requested more bits than there are left in the byte slice at the current position.
    NotEnoughData = 1,
    /// Requested more bits than the returned variable can hold, for example more than 8 bits when
    /// reading into a u8.
    TooManyBitsForType = 2,
    InvalidInput = 3,
}

impl From<BitReaderError> for CBitReaderCode {
    fn from(value: BitReaderError) -> Self {
        match value {
            BitReaderError::NotEnoughData { .. } => CBitReaderCode::NotEnoughData,
            BitReaderError::TooManyBitsForType { .. } => CBitReaderCode::TooManyBitsForType,
        }
    }
}

impl From<CBitReaderCode> for ErrorCode {
    fn from(error: CBitReaderCode) -> Self {
        error as ErrorCode
    }
}

/// BitReader reads data from a big-endian byte slice at the granularity of a single bit.
#[derive(Debug)]
pub struct TWBitReader {
    buffer: Data,
    bit_position: u64,
    bit_len: u64,
}

impl TWBitReader {
    pub fn with_relative_bit_len(buffer: Data, bit_len: u64) -> TWBitReader {
        TWBitReader {
            buffer,
            bit_position: 0,
            bit_len,
        }
    }

    /// Read at most 8 bits into a u8.
    pub fn read_u8(&mut self, bit_count: u8) -> Result<u8, CBitReaderCode> {
        let mut reader = self.make_reader()?;
        let res = reader.read_u8(bit_count)?;
        // Update the bit position in case of success read.
        self.bit_position += bit_count as u64;
        Ok(res)
    }

    // Reads an entire slice of `byte_count` bytes. If there aren't enough bits remaining
    // after the internal cursor's current position, returns none.
    pub fn read_u8_slice(&mut self, byte_count: usize) -> Result<Data, CBitReaderCode> {
        let mut reader = self.make_reader()?;

        let mut res = vec![0_u8; byte_count];
        reader.read_u8_slice(&mut res)?;

        // Update the bit position in case of success read.
        self.bit_position += byte_count as u64 * 8;
        Ok(res)
    }

    pub fn is_finished(&self) -> bool {
        self.bit_len == self.bit_position
    }

    fn make_reader(&self) -> Result<BitReader<'_>, CBitReaderCode> {
        let mut reader = BitReader::new(&self.buffer).relative_reader_atmost(self.bit_len);
        reader.skip(self.bit_position)?;
        Ok(reader)
    }
}

impl RawPtrTrait for TWBitReader {}

/// Constructs a new `TWBitReader` from a big-endian byte slice
/// that will not allow reading more than `bit_len` bits. It must be deleted at the end.
///
/// \param data big-endian byte slice to be read.
/// \param bit_len length this reader is allowed to read from the slice.
/// \return nullable pointer to a `TWBitReader` instance.
#[no_mangle]
pub unsafe extern "C" fn tw_bit_reader_create(
    data: *const TWData,
    bit_len: u64,
) -> *mut TWBitReader {
    let data = try_or_else!(TWData::from_ptr_as_ref(data), std::ptr::null_mut);
    TWBitReader::with_relative_bit_len(data.to_vec(), bit_len).into_ptr()
}

/// Deletes a `TWBitReader` and frees the memory.
/// \param reader a `TWBitReader` pointer.
#[no_mangle]
pub unsafe extern "C" fn tw_bit_reader_delete(reader: *mut TWBitReader) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWBitReader::from_ptr(reader);
}

/// Read at most 8 bits into a u8.
///
/// \param reader a `TWBitReader` pointer.
/// \param bit_count number of bits to read. Expected from 1 to 8.
/// \return u8 or error.
#[no_mangle]
pub unsafe extern "C" fn tw_bit_reader_read_u8(
    reader: *mut TWBitReader,
    bit_count: u8,
) -> CUInt8Result {
    let tw_reader = try_or_else!(
        TWBitReader::from_ptr_as_mut(reader),
        || CUInt8Result::error(CBitReaderCode::InvalidInput)
    );
    tw_reader.read_u8(bit_count).into()
}

/// Reads an entire slice of `byteCount` bytes. If there aren't enough bits remaining
/// after the internal cursor's current position, returns null.
///
/// \param reader a `TWBitReader` pointer.
/// \param byte_count number of bytes to read.
/// \return byte array or error.
#[no_mangle]
pub unsafe extern "C" fn tw_bit_reader_read_u8_slice(
    reader: *mut TWBitReader,
    byte_count: usize,
) -> CByteArrayResult {
    let tw_reader = try_or_else!(TWBitReader::from_ptr_as_mut(reader), || {
        CByteArrayResult::error(CBitReaderCode::InvalidInput)
    });
    tw_reader
        .read_u8_slice(byte_count)
        .map(CByteArray::from)
        .into()
}

/// Checks whether all bits were read.
///
/// \param reader a `TWBitReader` pointer.
/// \return whether all bits were read.
#[no_mangle]
pub unsafe extern "C" fn tw_bit_reader_finished(reader: *const TWBitReader) -> bool {
    try_or_else!(TWBitReader::from_ptr_as_ref(reader), || true).is_finished()
}
