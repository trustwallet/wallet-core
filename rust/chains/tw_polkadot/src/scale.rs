use tw_number::U256;

///
/// SCALE encoding implementation (see https://docs.substrate.io/reference/scale-codec)
/// TODO: this is a substrate-specific encoding, but consider moving to tw_encoding crate
///

pub trait ToScale {
    fn to_scale(&self) -> Vec<u8>;
}

impl ToScale for bool {
    fn to_scale(&self) -> Vec<u8> {
        (if *self { 0x01 } else { 0x00 } as u8).to_scale()
    }
}

macro_rules! fixed_impl {
    ($($t:ty),+) => {
        $(impl ToScale for $t {
            fn to_scale(&self) -> Vec<u8> {
                self.to_le_bytes().to_vec()
            }
        })+
    };
}

fixed_impl!(u8, u16, u32, u64, usize, i8, i16, i32, i64, isize);

#[derive(Debug, Copy, Clone, Ord, PartialOrd, Eq, PartialEq)]
pub struct Compact<T>(pub T);

// Implementations for Compact

impl ToScale for Compact<u8> {
    fn to_scale(&self) -> Vec<u8> {
        match self.0 {
            0..=0b0011_1111 => vec![self.0 << 2],
            _ => (((self.0 as u16) << 2) | 0b01).to_scale(),
        }
    }
}

impl ToScale for Compact<u16> {
    fn to_scale(&self) -> Vec<u8> {
        match self.0 {
            0..=0b0011_1111 => vec![(self.0 as u8) << 2],
            0..=0b0011_1111_1111_1111 => ((self.0 << 2) | 0b01).to_scale(),
            _ => (((self.0 as u32) << 2) | 0b10).to_scale(),
        }
    }
}

impl ToScale for Compact<u32> {
    fn to_scale(&self) -> Vec<u8> {
        match self.0 {
            0..=0b0011_1111 => vec![(self.0 as u8) << 2],
            0..=0b0011_1111_1111_1111 => (((self.0 as u16) << 2) | 0b01).to_scale(),
            0..=0b0011_1111_1111_1111_1111_1111_1111_1111 => ((self.0 << 2) | 0b10).to_scale(),
            _ => {
                let mut v = vec![0b11];
                v.extend(self.0.to_scale());
                v
            },
        }
    }
}

impl ToScale for Compact<u64> {
    fn to_scale(&self) -> Vec<u8> {
        match self.0 {
            0..=0b0011_1111 => vec![(self.0 as u8) << 2],
            0..=0b0011_1111_1111_1111 => (((self.0 as u16) << 2) | 0b01).to_scale(),
            0..=0b0011_1111_1111_1111_1111_1111_1111_1111 => {
                (((self.0 as u32) << 2) | 0b10).to_scale()
            },
            _ => {
                let bytes_needed = 8 - self.0.leading_zeros() / 8;
                let mut v = Vec::with_capacity(bytes_needed as usize);
                v.push(0b11 + ((bytes_needed - 4) << 2) as u8);
                let mut x = self.0;
                for _ in 0..bytes_needed {
                    v.push(x as u8);
                    x >>= 8;
                }
                v
            },
        }
    }
}

// TODO: implement U128 support (we don't have it yet in tw_number)

impl ToScale for Compact<U256> {
    fn to_scale(&self) -> Vec<u8> {
        // match syntax gets a bit cluttered without u256 literals, falling back to if's

        if self.0 <= 0b0011_1111u64.into() {
            return vec![self.0.low_u8() << 2];
        }

        if self.0 <= 0b0011_1111_1111_1111u64.into() {
            let v = u16::try_from(self.0).expect("cannot happen as we just checked the value");
            return ((v << 2) | 0b01).to_scale();
        }

        if self.0 <= 0b0011_1111_1111_1111_1111_1111_1111_1111u64.into() {
            let v = u32::try_from(self.0).expect("cannot happen as we just checked the value");
            return ((v << 2) | 0b10).to_scale();
        }

        let bytes_needed = 32 - self.0.leading_zeros() / 8;
        let mut v = Vec::with_capacity(bytes_needed as usize);
        v.push(0b11 + ((bytes_needed - 4) << 2) as u8);
        let mut x = self.0;
        for _ in 0..bytes_needed {
            v.push(x.low_u8());
            x >>= 8;
        }

        v
    }
}

impl ToScale for Compact<usize> {
    fn to_scale(&self) -> Vec<u8> {
        Compact(self.0 as u64).to_scale()
    }
}

impl<T> ToScale for &[T]
where
    T: ToScale,
{
    fn to_scale(&self) -> Vec<u8> {
        let mut data = Compact(self.len()).to_scale();
        for ts in self.iter() {
            data.extend(ts.to_scale());
        }
        data
    }
}

impl<T> ToScale for Vec<T>
where
    T: ToScale,
{
    fn to_scale(&self) -> Vec<u8> {
        self.as_slice().to_scale()
    }
}

pub struct Raw(pub Vec<u8>);

impl ToScale for Raw {
    fn to_scale(&self) -> Vec<u8> {
        self.0.clone()
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
    fn test_slice() {
        let empty: [u8; 0] = [];
        assert_eq!(empty.as_slice().to_scale(), &[0x00]);
        assert_eq!(
            [4u16, 8, 15, 16, 23, 42].as_slice().to_scale(),
            &[0x18, 0x04, 0x00, 0x08, 0x00, 0x0f, 0x00, 0x10, 0x00, 0x17, 0x00, 0x2a, 0x00],
        );
    }
}
