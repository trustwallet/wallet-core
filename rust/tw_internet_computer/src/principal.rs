/// Taken from candid crate and modified to rely upon built-in crc32 and SHA224 functionality.
use std::fmt::Write;

use tw_hash::{crc32::crc32, sha2::sha224};
use tw_keypair::ecdsa::secp256k1::PublicKey;

/// An error happened while encoding, decoding or serializing a [`Principal`].
#[derive(Clone, Debug, Eq, PartialEq)]
pub enum PrincipalError {
    BytesTooLong,
    InvalidBase32,
    TextTooShort,
    TextTooLong,
    CheckSequenceNotMatch,
    AbnormalGrouped(Principal),
}

/// Generic ID on Internet Computer.
///
/// Principals are generic identifiers for canisters, users
/// and possibly other concepts in the future.
/// As far as most uses of the IC are concerned they are
/// opaque binary blobs with a length between 0 and 29 bytes,
/// and there is intentionally no mechanism to tell canister ids and user ids apart.
///
/// Note a principal is not necessarily tied with a public key-pair,
/// yet we need at least a key-pair of a related principal to sign
/// requests.
///
/// A Principal can be serialized to a byte array ([`Vec<u8>`]) or a text
/// representation, but the inner structure of the byte representation
/// is kept private.
///
/// Example of using a Principal object:
/// ```
/// # use candid::Principal;
/// let text = "aaaaa-aa";  // The management canister ID.
/// let principal = Principal::from_text(text).expect("Could not decode the principal.");
/// assert_eq!(principal.as_slice(), &[] as &[u8]);
/// assert_eq!(principal.to_text(), text);
/// ```
///
/// Similarly, serialization using serde has two versions:
/// serilizing to a byte bufer for non-human readable serializer, and a string version for human
/// readable serializers.
///
/// ```
/// # use candid::Principal;
/// use serde::{Deserialize, Serialize};
/// use std::str::FromStr;
///
/// #[derive(Serialize)]
/// struct Data {
///     id: Principal,
/// }
///
/// let id = Principal::from_str("2chl6-4hpzw-vqaaa-aaaaa-c").unwrap();
///
/// // JSON is human readable, so this will serialize to a textual
/// // representation of the Principal.
/// assert_eq!(
///     serde_json::to_string(&Data { id: id.clone() }).unwrap(),
///     r#"{"id":"2chl6-4hpzw-vqaaa-aaaaa-c"}"#
/// );
///
/// // CBOR is not human readable, so will serialize to bytes.
/// assert_eq!(
///     serde_cbor::to_vec(&Data { id: id.clone() }).unwrap(),
///     &[161, 98, 105, 100, 73, 239, 205, 171, 0, 0, 0, 0, 0, 1],
/// );
/// ```
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Principal {
    /// Length.
    len: u8,

    /// The content buffer. When returning slices this should always be sized according to
    /// `len`.
    bytes: [u8; Self::MAX_LENGTH_IN_BYTES],
}

impl Principal {
    const MAX_LENGTH_IN_BYTES: usize = 29;
    const CRC_LENGTH_IN_BYTES: usize = 4;

    const SELF_AUTHENTICATING_TAG: u8 = 2;
    const ANONYMOUS_TAG: u8 = 4;

    /// Construct a [`Principal`] of the IC management canister
    pub const fn management_canister() -> Self {
        Self {
            len: 0,
            bytes: [0; Self::MAX_LENGTH_IN_BYTES],
        }
    }

    /// Construct a self-authenticating ID from public key
    pub fn self_authenticating<P: AsRef<[u8]>>(public_key: P) -> Self {
        let public_key = public_key.as_ref();
        let hash = sha224(public_key);
        let mut bytes = [0; Self::MAX_LENGTH_IN_BYTES];
        bytes[..Self::MAX_LENGTH_IN_BYTES - 1].copy_from_slice(hash.as_slice());
        bytes[Self::MAX_LENGTH_IN_BYTES - 1] = Self::SELF_AUTHENTICATING_TAG;

        Self {
            len: Self::MAX_LENGTH_IN_BYTES as u8,
            bytes,
        }
    }

    /// Construct an anonymous ID.
    pub const fn anonymous() -> Self {
        let mut bytes = [0; Self::MAX_LENGTH_IN_BYTES];
        bytes[0] = Self::ANONYMOUS_TAG;
        Self { len: 1, bytes }
    }

    /// Construct a [`Principal`] from a slice of bytes.
    ///
    /// # Panics
    ///
    /// Panics if the slice is longer than 29 bytes.
    pub const fn from_slice(slice: &[u8]) -> Self {
        match Self::try_from_slice(slice) {
            Ok(v) => v,
            _ => panic!("slice length exceeds capacity"),
        }
    }

    /// Construct a [`Principal`] from a slice of bytes.
    pub const fn try_from_slice(slice: &[u8]) -> Result<Self, PrincipalError> {
        const MAX_LENGTH_IN_BYTES: usize = Principal::MAX_LENGTH_IN_BYTES;
        match slice.len() {
            len @ 0..=MAX_LENGTH_IN_BYTES => {
                let mut bytes = [0; MAX_LENGTH_IN_BYTES];
                let mut i = 0;
                while i < len {
                    bytes[i] = slice[i];
                    i += 1;
                }
                Ok(Self {
                    len: len as u8,
                    bytes,
                })
            },
            _ => Err(PrincipalError::BytesTooLong),
        }
    }

    /// Parse a [`Principal`] from text representation.
    pub fn from_text<S: AsRef<str>>(text: S) -> Result<Self, PrincipalError> {
        // Strategy: Parse very liberally, then pretty-print and compare output
        // This is both simpler and yields better error messages

        let mut s = text.as_ref().to_string();
        s.make_ascii_uppercase();
        s.retain(|c| c != '-');

        let bytes = tw_encoding::base32::decode(&s, None, false)
            .map_err(|_| PrincipalError::InvalidBase32)?;

        if bytes.len() < Self::CRC_LENGTH_IN_BYTES {
            return Err(PrincipalError::TextTooShort);
        }

        let crc_bytes = &bytes[..Self::CRC_LENGTH_IN_BYTES];
        let data_bytes = &bytes[Self::CRC_LENGTH_IN_BYTES..];
        if data_bytes.len() > Self::MAX_LENGTH_IN_BYTES {
            return Err(PrincipalError::TextTooLong);
        }

        if crc32(data_bytes).to_be_bytes() != crc_bytes {
            return Err(PrincipalError::CheckSequenceNotMatch);
        }

        // Already checked data_bytes.len() <= MAX_LENGTH_IN_BYTES
        // safe to unwrap here
        let result = Self::try_from_slice(data_bytes).unwrap();
        let expected = format!("{result}");

        // In the Spec:
        // The textual representation is conventionally printed with lower case letters,
        // but parsed case-insensitively.
        if text.as_ref().to_ascii_lowercase() != expected {
            return Err(PrincipalError::AbnormalGrouped(result));
        }
        Ok(result)
    }

    /// Convert [`Principal`] to text representation.
    pub fn to_text(&self) -> String {
        format!("{self}")
    }

    /// Return the [`Principal`]'s underlying slice of bytes.
    #[inline]
    pub fn as_slice(&self) -> &[u8] {
        &self.bytes[..self.len as usize]
    }
}

impl std::str::FromStr for Principal {
    type Err = PrincipalError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Principal::from_text(s)
    }
}

impl From<&PublicKey> for Principal {
    /// Takes a Secp256k1 public key, DER-encodes the public key,
    /// and creates a principal from the encoding.
    fn from(public_key: &PublicKey) -> Self {
        Self::self_authenticating(public_key.der_encoded())
    }
}

impl std::fmt::Display for Principal {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let blob: &[u8] = self.as_slice();

        // calc checksum
        let checksum = crc32(blob);

        // combine blobs
        let mut bytes = vec![];
        bytes.extend_from_slice(&checksum.to_be_bytes());
        bytes.extend_from_slice(blob);

        // base32
        let mut s =
            tw_encoding::base32::encode(&bytes, None, false).map_err(|_| std::fmt::Error)?;
        s.make_ascii_lowercase();

        // write out string with dashes
        let mut s = s.as_str();
        while s.len() > 5 {
            f.write_str(&s[..5])?;
            f.write_char('-')?;
            s = &s[5..];
        }
        f.write_str(s)
    }
}

impl TryFrom<&str> for Principal {
    type Error = PrincipalError;

    fn try_from(s: &str) -> Result<Self, Self::Error> {
        Principal::from_text(s)
    }
}

impl TryFrom<Vec<u8>> for Principal {
    type Error = PrincipalError;

    fn try_from(bytes: Vec<u8>) -> Result<Self, Self::Error> {
        Self::try_from(bytes.as_slice())
    }
}

impl TryFrom<&Vec<u8>> for Principal {
    type Error = PrincipalError;

    fn try_from(bytes: &Vec<u8>) -> Result<Self, Self::Error> {
        Self::try_from(bytes.as_slice())
    }
}

impl TryFrom<&[u8]> for Principal {
    type Error = PrincipalError;

    fn try_from(bytes: &[u8]) -> Result<Self, Self::Error> {
        Self::try_from_slice(bytes)
    }
}

impl AsRef<[u8]> for Principal {
    fn as_ref(&self) -> &[u8] {
        self.as_slice()
    }
}

// Serialization
impl serde::Serialize for Principal {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        if serializer.is_human_readable() {
            self.to_text().serialize(serializer)
        } else {
            serializer.serialize_bytes(self.as_slice())
        }
    }
}
