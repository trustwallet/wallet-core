use once_cell::sync::Lazy;

const C32_ALPHABET: &[u8; 32] = b"0123456789ABCDEFGHJKMNPQRSTVWXYZ";

static C32_BYTE_MAP: Lazy<[Option<u8>; 128]> = Lazy::new(|| {
    let mut table: [Option<u8>; 128] = [None; 128];

    let alphabet: [char; 32] = C32_ALPHABET
        .iter()
        .map(|byte| *byte as char)
        .collect::<Vec<_>>()
        .try_into()
        .unwrap();

    alphabet.iter().enumerate().for_each(|(i, x)| {
        table[*x as usize] = Some(i as u8);
    });

    alphabet
        .iter()
        .map(|c| c.to_ascii_lowercase())
        .enumerate()
        .for_each(|(i, x)| {
            table[x as usize] = Some(i as u8);
        });

    [('O', '0'), ('L', '1'), ('I', '1')]
        .into_iter()
        .for_each(|special_pair| {
            let i = alphabet
                .iter()
                .enumerate()
                .find(|(_, a)| **a == special_pair.1)
                .unwrap()
                .0;

            table[special_pair.0 as usize] = Some(i as u8);
            table[special_pair.0.to_ascii_lowercase() as usize] = Some(i as u8);
        });

    table
});

fn encode_overhead(len: usize) -> usize {
    (len * 8 + 4) / 5
}

fn decode_underhead(len: usize) -> usize {
    len / (8f64 / 5f64).ceil() as usize
}

#[derive(Clone, Debug, Eq, PartialEq)]
/// C32 error type
pub enum C32Error {
    /// Invalid C32 string.
    InvalidC32,
    /// Invalid character.
    InvalidChar(char),
    /// Conversion error, from utf8.
    FromUtf8Error(std::string::FromUtf8Error),
    /// Integer conversion error.
    IntConversionError(std::num::TryFromIntError),
}

impl From<std::string::FromUtf8Error> for C32Error {
    fn from(e: std::string::FromUtf8Error) -> Self {
        Self::FromUtf8Error(e)
    }
}

impl From<std::num::TryFromIntError> for C32Error {
    fn from(e: std::num::TryFromIntError) -> Self {
        Self::IntConversionError(e)
    }
}

/// C32 encode the given data
pub fn encode(data: impl AsRef<[u8]>) -> String {
    let data = data.as_ref();

    let mut encoded = Vec::with_capacity(encode_overhead(data.len()));
    let mut buffer = 0u32;
    let mut bits = 0;

    for byte in data.iter().rev() {
        buffer |= (*byte as u32) << bits;
        bits += 8;

        while bits >= 5 {
            encoded.push(C32_ALPHABET[(buffer & 0x1F) as usize]);
            buffer >>= 5;
            bits -= 5;
        }
    }

    if bits > 0 {
        encoded.push(C32_ALPHABET[(buffer & 0x1F) as usize]);
    }

    while let Some(i) = encoded.pop() {
        if i != C32_ALPHABET[0] {
            encoded.push(i);
            break;
        }
    }

    for i in data {
        if *i == 0 {
            encoded.push(C32_ALPHABET[0]);
        } else {
            break;
        }
    }

    encoded.reverse();

    String::from_utf8(encoded).unwrap()
}

/// C32 decode the given data
pub fn decode(input: impl AsRef<str>) -> Result<Vec<u8>, C32Error> {
    let input = input.as_ref().as_bytes();

    if !input.is_ascii() {
        return Err(C32Error::InvalidC32);
    }

    let mut decoded = Vec::with_capacity(decode_underhead(input.len()));
    let mut carry = 0u16;
    let mut carry_bits = 0;

    for byte in input.iter().rev() {
        let Some(bits) = C32_BYTE_MAP.get(*byte as usize).unwrap() else {
            return Err(C32Error::InvalidChar(*byte as char));
        };

        carry |= (u16::from(*bits)) << carry_bits;
        carry_bits += 5;

        if carry_bits >= 8 {
            decoded.push((carry & 0xFF) as u8);
            carry >>= 8;
            carry_bits -= 8;
        }
    }

    if carry_bits > 0 {
        decoded.push(u8::try_from(carry)?);
    }

    while let Some(i) = decoded.pop() {
        if i != 0 {
            decoded.push(i);
            break;
        }
    }

    for byte in input.iter() {
        if *byte == b'0' {
            decoded.push(0);
        } else {
            break;
        }
    }

    decoded.reverse();

    Ok(decoded)
}
