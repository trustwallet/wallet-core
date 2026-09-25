use tw_hash::Hash;
use tw_number::U256;

pub mod macros;

///
/// SCALE encoding implementation (see https://docs.substrate.io/reference/scale-codec)
///
pub trait ToScale {
    fn to_scale(&self) -> Vec<u8> {
        let mut data = Vec::new();
        self.to_scale_into(&mut data);
        data
    }

    fn to_scale_into(&self, out: &mut Vec<u8>);
}

impl ToScale for bool {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.push(*self as u8);
    }
}

macro_rules! fixed_impl {
    ($($t:ty),+) => {
        $(impl ToScale for $t {
            fn to_scale_into(&self, out: &mut Vec<u8>) {
                out.extend_from_slice(&self.to_le_bytes())
            }
        })+
    };
}

fixed_impl!(u8, u16, u32, u64, u128, usize, i8, i16, i32, i64, i128, isize);

#[derive(Debug, Copy, Clone, Ord, PartialOrd, Eq, PartialEq)]
pub struct Compact<T>(pub T);

// SCALE encoding for common Compact numbers.

impl ToScale for Compact<u8> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

impl ToScale for Compact<u16> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

impl ToScale for Compact<u32> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

impl ToScale for Compact<u64> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

impl ToScale for Compact<u128> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

impl ToScale for Compact<usize> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(U256::from(self.0)).to_scale_into(out)
    }
}

const COMPACT_1_BYTE_MAX: u32 = 0b0011_1111;
const COMPACT_2_BYTE_MAX: u32 = 0b0011_1111_1111_1111;
const COMPACT_4_BYTE_MAX: u32 = 0b0011_1111_1111_1111_1111_1111_1111_1111;

impl ToScale for Compact<U256> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match u32::try_from(self.0) {
            Ok(val) if val <= COMPACT_1_BYTE_MAX => out.push((val as u8) << 2),
            Ok(val) if val <= COMPACT_2_BYTE_MAX => (((val as u16) << 2) | 0b01).to_scale_into(out),
            Ok(val) if val <= COMPACT_4_BYTE_MAX => ((val << 2) | 0b10).to_scale_into(out),
            _ => {
                let bytes = self.0.to_little_endian_compact();
                let bytes_needed = bytes.len();
                out.reserve(bytes_needed);
                out.push(0b11 + ((bytes_needed - 4) << 2) as u8);
                out.extend_from_slice(&bytes[..]);
            },
        }
    }
}

impl<const N: usize> ToScale for Hash<N> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend_from_slice(self.as_slice());
    }
}

impl<T> ToScale for Option<T>
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        if let Some(t) = &self {
            out.push(1u8);
            t.to_scale_into(out);
        } else {
            out.push(0u8);
        }
    }
}

impl<T> ToScale for &[T]
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(self.len()).to_scale_into(out);
        for ts in self.iter() {
            ts.to_scale_into(out);
        }
    }
}

impl ToScale for String {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.as_bytes().to_scale_into(out)
    }
}

/// RawOwned is used to wrap data that is already encoded in SCALE format.
#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct RawOwned(pub Vec<u8>);

impl RawOwned {
    pub fn new<T: ToScale>(val: T) -> Self {
        Self(val.to_scale())
    }
}

impl ToScale for RawOwned {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend(&self.0);
    }
}

// Implement ToScale for BTreeSet collection.
impl<T> ToScale for std::collections::BTreeSet<T>
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(self.len()).to_scale_into(out);
        for ts in self.iter() {
            ts.to_scale_into(out);
        }
    }
}

// Implement ToScale for BTreeMap collection.
impl<K, V> ToScale for std::collections::BTreeMap<K, V>
where
    K: ToScale,
    V: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(self.len()).to_scale_into(out);
        for (k, v) in self.iter() {
            k.to_scale_into(out);
            v.to_scale_into(out);
        }
    }
}

// Implement ToScale for Vec collection.
impl<T> ToScale for Vec<T>
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.as_slice().to_scale_into(out)
    }
}

// Implement ToScale for references to types that implement ToScale.
impl<T: ToScale> ToScale for &T {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        (*self).to_scale_into(out)
    }
}

#[cfg(test)]
mod tests {
    use super::{Compact, ToScale};
    use tw_number::U256;

    #[test]
    fn test_fixed_width_integers() {
        assert_eq!(69i8.to_scale(), &[0x45]);
        assert_eq!(42u16.to_scale(), &[0x2a, 0x00]);
        assert_eq!(16777215u32.to_scale(), &[0xff, 0xff, 0xff, 0x00]);
    }

    #[test]
    fn test_bool() {
        assert_eq!(true.to_scale(), &[0x01]);
        assert_eq!(false.to_scale(), &[0x00]);
    }

    #[test]
    fn test_compact_integers() {
        assert_eq!(Compact(0u8).to_scale(), &[0x00]);
        assert_eq!(Compact(1u8).to_scale(), &[0x04]);
        assert_eq!(Compact(18u8).to_scale(), &[0x48]);
        assert_eq!(Compact(42u8).to_scale(), &[0xa8]);
        assert_eq!(Compact(63u8).to_scale(), &[0xfc]);
        assert_eq!(Compact(64u8).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(69u8).to_scale(), &[0x15, 0x01]);

        assert_eq!(Compact(0u16).to_scale(), &[0x00]);
        assert_eq!(Compact(1u16).to_scale(), &[0x04]);
        assert_eq!(Compact(18u16).to_scale(), &[0x48]);
        assert_eq!(Compact(42u16).to_scale(), &[0xa8]);
        assert_eq!(Compact(63u16).to_scale(), &[0xfc]);
        assert_eq!(Compact(64u16).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(69u16).to_scale(), &[0x15, 0x01]);
        assert_eq!(Compact(12345u16).to_scale(), &[0xe5, 0xc0]);
        assert_eq!(Compact(16383u16).to_scale(), &[0xfd, 0xff]);
        assert_eq!(Compact(16384u16).to_scale(), &[0x02, 0x00, 0x01, 0x00]);
        assert_eq!(Compact(65535u16).to_scale(), &[0xfe, 0xff, 0x03, 0x00]);

        assert_eq!(Compact(0u32).to_scale(), &[0x00]);
        assert_eq!(Compact(1u32).to_scale(), &[0x04]);
        assert_eq!(Compact(18u32).to_scale(), &[0x48]);
        assert_eq!(Compact(42u32).to_scale(), &[0xa8]);
        assert_eq!(Compact(63u32).to_scale(), &[0xfc]);
        assert_eq!(Compact(64u32).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(69u32).to_scale(), &[0x15, 0x01]);
        assert_eq!(Compact(12345u32).to_scale(), &[0xe5, 0xc0]);
        assert_eq!(Compact(16383u32).to_scale(), &[0xfd, 0xff]);
        assert_eq!(Compact(16384u32).to_scale(), &[0x02, 0x00, 0x01, 0x00]);
        assert_eq!(Compact(65535u32).to_scale(), &[0xfe, 0xff, 0x03, 0x00]);
        assert_eq!(Compact(1073741823u32).to_scale(), &[0xfe, 0xff, 0xff, 0xff]);
        assert_eq!(
            Compact(1073741824u32).to_scale(),
            &[0x03, 0x00, 0x00, 0x00, 0x40]
        );
        assert_eq!(
            Compact(4294967295u32).to_scale(),
            &[0x03, 0xff, 0xff, 0xff, 0xff]
        );

        assert_eq!(Compact(0u64).to_scale(), &[0x00]);
        assert_eq!(Compact(1u64).to_scale(), &[0x04]);
        assert_eq!(Compact(18u64).to_scale(), &[0x48]);
        assert_eq!(Compact(42u64).to_scale(), &[0xa8]);
        assert_eq!(Compact(63u64).to_scale(), &[0xfc]);
        assert_eq!(Compact(64u64).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(69u64).to_scale(), &[0x15, 0x01]);
        assert_eq!(Compact(12345u64).to_scale(), &[0xe5, 0xc0]);
        assert_eq!(Compact(16383u64).to_scale(), &[0xfd, 0xff]);
        assert_eq!(Compact(16384u64).to_scale(), &[0x02, 0x00, 0x01, 0x00]);
        assert_eq!(Compact(65535u64).to_scale(), &[0xfe, 0xff, 0x03, 0x00]);
        assert_eq!(Compact(1073741823u64).to_scale(), &[0xfe, 0xff, 0xff, 0xff]);
        assert_eq!(
            Compact(1073741824u64).to_scale(),
            &[0x03, 0x00, 0x00, 0x00, 0x40]
        );
        assert_eq!(
            Compact(4294967295u64).to_scale(),
            &[0x03, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(4294967296u64).to_scale(),
            &[0x07, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(100000000000000u64).to_scale(),
            &[0x0b, 0x00, 0x40, 0x7a, 0x10, 0xf3, 0x5a]
        );
        assert_eq!(
            Compact(1099511627776u64).to_scale(),
            &[0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(281474976710656u64).to_scale(),
            &[0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(72057594037927935u64).to_scale(),
            &[0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(72057594037927936u64).to_scale(),
            &[0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(18446744073709551615u64).to_scale(),
            &[0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );

        assert_eq!(Compact(0u128).to_scale(), &[0x00]);
        assert_eq!(Compact(1u128).to_scale(), &[0x04]);
        assert_eq!(Compact(18u128).to_scale(), &[0x48]);
        assert_eq!(Compact(42u128).to_scale(), &[0xa8]);
        assert_eq!(Compact(63u128).to_scale(), &[0xfc]);
        assert_eq!(Compact(64u128).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(69u128).to_scale(), &[0x15, 0x01]);
        assert_eq!(Compact(12345u128).to_scale(), &[0xe5, 0xc0]);
        assert_eq!(Compact(16383u128).to_scale(), &[0xfd, 0xff]);
        assert_eq!(Compact(16384u128).to_scale(), &[0x02, 0x00, 0x01, 0x00]);
        assert_eq!(Compact(65535u128).to_scale(), &[0xfe, 0xff, 0x03, 0x00]);
        assert_eq!(
            Compact(1073741823u128).to_scale(),
            &[0xfe, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(1073741824u128).to_scale(),
            &[0x03, 0x00, 0x00, 0x00, 0x40]
        );
        assert_eq!(
            Compact(4294967295u128).to_scale(),
            &[0x03, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(4294967296u128).to_scale(),
            &[0x07, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(100000000000000u128).to_scale(),
            &[0x0b, 0x00, 0x40, 0x7a, 0x10, 0xf3, 0x5a]
        );
        assert_eq!(
            Compact(1099511627776u128).to_scale(),
            &[0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(281474976710656u128).to_scale(),
            &[0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(72057594037927935u128).to_scale(),
            &[0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(72057594037927936u128).to_scale(),
            &[0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(18446744073709551615u128).to_scale(),
            &[0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );

        assert_eq!(Compact(U256::from(0u64)).to_scale(), &[0x00]);
        assert_eq!(Compact(U256::from(1u64)).to_scale(), &[0x04]);
        assert_eq!(Compact(U256::from(18u64)).to_scale(), &[0x48]);
        assert_eq!(Compact(U256::from(42u64)).to_scale(), &[0xa8]);
        assert_eq!(Compact(U256::from(63u64)).to_scale(), &[0xfc]);
        assert_eq!(Compact(U256::from(64u64)).to_scale(), &[0x01, 0x01]);
        assert_eq!(Compact(U256::from(69u64)).to_scale(), &[0x15, 0x01]);
        assert_eq!(Compact(U256::from(12345u64)).to_scale(), &[0xe5, 0xc0]);
        assert_eq!(Compact(U256::from(16383u64)).to_scale(), &[0xfd, 0xff]);
        assert_eq!(
            Compact(U256::from(16384u64)).to_scale(),
            &[0x02, 0x00, 0x01, 0x00]
        );
        assert_eq!(
            Compact(U256::from(65535u64)).to_scale(),
            &[0xfe, 0xff, 0x03, 0x00]
        );
        assert_eq!(
            Compact(U256::from(1073741823u64)).to_scale(),
            &[0xfe, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(U256::from(1073741824u64)).to_scale(),
            &[0x03, 0x00, 0x00, 0x00, 0x40]
        );
        assert_eq!(
            Compact(U256::from(4294967295u64)).to_scale(),
            &[0x03, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(U256::from(4294967296u64)).to_scale(),
            &[0x07, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(U256::from(100000000000000u64)).to_scale(),
            &[0x0b, 0x00, 0x40, 0x7a, 0x10, 0xf3, 0x5a]
        );
        assert_eq!(
            Compact(U256::from(1099511627776u64)).to_scale(),
            &[0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(U256::from(281474976710656u64)).to_scale(),
            &[0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(U256::from(72057594037927935u64)).to_scale(),
            &[0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );
        assert_eq!(
            Compact(U256::from(72057594037927936u64)).to_scale(),
            &[0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01]
        );
        assert_eq!(
            Compact(U256::from(18446744073709551615u64)).to_scale(),
            &[0x13, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff]
        );
    }

    #[test]
    fn test_option() {
        assert_eq!(Some(1u8).to_scale(), &[0x01, 0x01]);
        assert_eq!(None::<u8>.to_scale(), &[0x00]);
        assert_eq!(Some(Compact(1u64)).to_scale(), &[0x01, 0x04]);
    }

    #[test]
    fn test_slice() {
        let empty: [u8; 0] = [];
        assert_eq!(empty.as_slice().to_scale(), &[0x00]);
        assert_eq!(
            [4u16, 8, 15, 16, 23, 42].as_slice().to_scale(),
            &[0x18, 0x04, 0x00, 0x08, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x17, 0x00, 0x2a, 0x00],
        );
    }

    // Test SCALE encoding of String
    #[test]
    fn test_string() {
        assert_eq!("".to_string().to_scale(), &[0x00]);
        assert_eq!(
            "hello".to_string().to_scale(),
            &[0x14, 0x68, 0x65, 0x6c, 0x6c, 0x6f]
        );
        assert_eq!(
            "hello world".to_string().to_scale(),
            &[0x2c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64]
        );
    }

    // Test SCALE encoding of BTreeSet
    #[test]
    fn test_btree_set() {
        use std::collections::BTreeSet;
        let mut set = BTreeSet::new();
        set.insert(10u8);
        set.insert(30u8);
        set.insert(20u8);
        // The values are encoded in sorted order.
        assert_eq!(set.to_scale(), &[0x0c, 10, 20, 30]);
    }

    // Test SCALE encoding of BTreeMap
    #[test]
    fn test_btree_map() {
        use std::collections::BTreeMap;
        let mut map = BTreeMap::new();
        map.insert(30u8, 300u16);
        map.insert(10u8, 100u16);
        map.insert(20u8, 200u16);
        // The keys/value pairs are encoded in sorted order (by key order).
        assert_eq!(
            map.to_scale(),
            &[0x0c, 10, 0x64, 0x00, 20, 0xc8, 0x00, 30, 0x2c, 0x01]
        );
    }
}
