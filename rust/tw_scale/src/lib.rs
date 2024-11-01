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
        out.push(if *self { 0x01 } else { 0x00 } as u8);
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

// Implementations for Compact

impl ToScale for Compact<u8> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self.0 {
            0..=0b0011_1111 => out.push(self.0 << 2),
            _ => (((self.0 as u16) << 2) | 0b01).to_scale_into(out),
        }
    }
}

impl ToScale for Compact<u16> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self.0 {
            0..=0b0011_1111 => out.push((self.0 as u8) << 2),
            0..=0b0011_1111_1111_1111 => ((self.0 << 2) | 0b01).to_scale_into(out),
            _ => (((self.0 as u32) << 2) | 0b10).to_scale_into(out),
        }
    }
}

impl ToScale for Compact<u32> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self.0 {
            0..=0b0011_1111 => out.push((self.0 as u8) << 2),
            0..=0b0011_1111_1111_1111 => (((self.0 as u16) << 2) | 0b01).to_scale_into(out),
            0..=0b0011_1111_1111_1111_1111_1111_1111_1111 => {
                ((self.0 << 2) | 0b10).to_scale_into(out)
            },
            _ => {
                out.push(0b11);
                self.0.to_scale_into(out);
            },
        }
    }
}

impl ToScale for Compact<u64> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self.0 {
            0..=0b0011_1111 => out.push((self.0 as u8) << 2),
            0..=0b0011_1111_1111_1111 => (((self.0 as u16) << 2) | 0b01).to_scale_into(out),
            0..=0b0011_1111_1111_1111_1111_1111_1111_1111 => {
                (((self.0 as u32) << 2) | 0b10).to_scale_into(out)
            },
            _ => {
                let bytes_needed = 8 - self.0.leading_zeros() / 8;
                out.reserve(bytes_needed as usize);
                out.push(0b11 + ((bytes_needed - 4) << 2) as u8);
                let mut x = self.0;
                for _ in 0..bytes_needed {
                    out.push(x as u8);
                    x >>= 8;
                }
            },
        }
    }
}

impl ToScale for Compact<u128> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self.0 {
            0..=0b0011_1111 => out.push((self.0 as u8) << 2),
            0..=0b0011_1111_1111_1111 => (((self.0 as u16) << 2) | 0b01).to_scale_into(out),
            0..=0b0011_1111_1111_1111_1111_1111_1111_1111 => {
                (((self.0 as u32) << 2) | 0b10).to_scale_into(out)
            },
            _ => {
                let bytes_needed = 16 - self.0.leading_zeros() / 8;
                out.reserve(bytes_needed as usize);
                out.push(0b11 + ((bytes_needed - 4) << 2) as u8);
                let mut x = self.0;
                for _ in 0..bytes_needed {
                    out.push(x as u8);
                    x >>= 8;
                }
            },
        }
    }
}

impl ToScale for Compact<U256> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        // match syntax gets a bit cluttered without u256 literals, falling back to if's

        if self.0 <= 0b0011_1111u64.into() {
            return out.push(self.0.low_u8() << 2);
        }

        if self.0 <= 0b0011_1111_1111_1111u64.into() {
            let v = u16::try_from(self.0).expect("cannot happen as we just checked the value");
            return ((v << 2) | 0b01).to_scale_into(out);
        }

        if self.0 <= 0b0011_1111_1111_1111_1111_1111_1111_1111u64.into() {
            let v = u32::try_from(self.0).expect("cannot happen as we just checked the value");
            return ((v << 2) | 0b10).to_scale_into(out);
        }

        let bytes_needed = 32 - self.0.leading_zeros() / 8;
        out.reserve(bytes_needed as usize);
        out.push(0b11 + ((bytes_needed - 4) << 2) as u8);
        let mut x = self.0;
        for _ in 0..bytes_needed {
            out.push(x.low_u8());
            x >>= 8;
        }
    }
}

impl ToScale for Compact<usize> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        Compact(self.0 as u64).to_scale_into(out)
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

impl<T> ToScale for Vec<T>
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.as_slice().to_scale_into(out)
    }
}

pub struct FixedLength<'a, T>(pub &'a [T]);

impl<'a, T> ToScale for FixedLength<'a, T>
where
    T: ToScale,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        for ts in self.0.iter() {
            ts.to_scale_into(out);
        }
    }
}

pub struct Raw<'a>(pub &'a [u8]);

impl<'a> ToScale for Raw<'a> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend_from_slice(&self.0);
    }
}

pub struct RawIter<T>(pub T);

impl<T> ToScale for RawIter<T>
where
    T: IntoIterator<Item = u8> + Clone,
{
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend(self.0.clone());
    }
}

impl<T: ToScale> ToScale for &T {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        (*self).to_scale_into(out)
    }
}

#[cfg(test)]
mod tests {
    use super::{Compact, FixedLength, Raw, RawIter, ToScale};
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

    #[test]
    fn test_fixed_length() {
        let empty: [u8; 0] = [];
        assert_eq!(FixedLength(&empty).to_scale(), empty);
        assert_eq!(
            FixedLength(&[4u16, 8, 15, 16, 23, 42]).to_scale(),
            &[0x04, 0x00, 0x08, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x17, 0x00, 0x2a, 0x00],
        );
    }

    #[test]
    fn test_raw() {
        let empty: [u8; 0] = [];
        assert_eq!(Raw(empty.as_slice()).to_scale(), empty);
        assert_eq!(
            Raw([4u8, 8, 15, 16, 23, 42].as_slice()).to_scale(),
            &[0x04, 0x08, 0x0f, 0x10, 0x17, 0x2a],
        );
    }

    #[test]
    fn test_raw_iter() {
        let empty: [u8; 0] = [];
        assert_eq!(RawIter(empty.into_iter()).to_scale(), empty);
        assert_eq!(
            RawIter([4u8, 8, 15, 16, 23, 42].into_iter()).to_scale(),
            &[0x04, 0x08, 0x0f, 0x10, 0x17, 0x2a],
        );
    }

    #[test]
    fn test_tuple() {
        assert_eq!((Compact(3u32), false).to_scale(), &[0x0c, 0x00]);
        assert_eq!((1u8, 2u8, 3u8).to_scale(), &[0x01, 0x02, 0x03]);
    }
}
