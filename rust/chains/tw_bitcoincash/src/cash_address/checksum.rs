// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::Data;

pub const CHECKSUM_LEN: usize = 8;
pub const PHANTOM_CHECKSUM: [u8; CHECKSUM_LEN] = [0; CHECKSUM_LEN];

// CalculateChecksum calculates a BCH checksum for a nibble-packed cashaddress
// that properly includes the network prefix.
pub fn calculate_checksum(prefix: &str, payload: &[u8]) -> u64 {
    // Convert the prefix string to a byte array with each element
    // being the corresponding character's right-most 5 bits.
    let mut raw_data: Vec<_> = prefix.as_bytes().iter().map(|x| x & 0b11111).collect();
    // Add a null termination byte after the prefix.
    raw_data.push(0);
    raw_data.extend(payload);
    poly_mod(&raw_data)
}

pub fn calc_and_append_checksum(prefix: &str, payload: &[u8]) -> Data {
    // The checksum sits in the last eight bytes.
    // Append the phantom checksum to calculate an actual value.
    let mut payload_with_checksum: Vec<_> =
        payload.iter().copied().chain(PHANTOM_CHECKSUM).collect();

    let checksum = calculate_checksum(prefix, &payload_with_checksum);

    let mut checksum_starts_at = payload_with_checksum.len() - CHECKSUM_LEN;
    // Rewrite the checksum.
    for i in 0..CHECKSUM_LEN {
        payload_with_checksum[checksum_starts_at] = ((checksum >> (5 * (7 - i))) & 0x1F) as u8;
        checksum_starts_at += 1;
    }

    payload_with_checksum
}

/// The poly_mod is a BCH-encoding checksum function per the CashAddr specification.
/// See https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md#checksum
pub fn poly_mod(raw_data: &[u8]) -> u64 {
    let mut c = 1u64;
    for d in raw_data {
        let c0 = c >> 35;
        c = ((c & 0x07ffffffff) << 5) ^ (*d as u64);

        if c0 & 0x01 != 0 {
            c ^= 0x98f2bc8e61;
        }
        if c0 & 0x02 != 0 {
            c ^= 0x79b76d99e2;
        }
        if c0 & 0x04 != 0 {
            c ^= 0xf33e5fb3c4;
        }
        if c0 & 0x08 != 0 {
            c ^= 0xae2eabe2a8;
        }
        if c0 & 0x10 != 0 {
            c ^= 0x1e4f43e470;
        }
    }

    c ^ 1
}
