use crate::encoder::error::Error;
use crate::encoder::{decode::decode_fix_slice, encode::encode_fix_slice};
use crate::encoder::{Decodable, Encodable};
use bech32::FromBase32;
use std::str::FromStr;
use tw_keypair::KeyPairError;

use super::network::{MAINNET_PUBLIC_KEY_HRP, TESTNET_PUBLIC_KEY_HRP};

pub const BLS_PUBLIC_KEY_SIZE: usize = 96;

#[derive(Debug)]
pub struct ValidatorPublicKey(pub [u8; BLS_PUBLIC_KEY_SIZE]);

impl Encodable for ValidatorPublicKey {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        encode_fix_slice(&self.0, w)
    }

    fn encoded_size(&self) -> usize {
        BLS_PUBLIC_KEY_SIZE
    }
}

impl Decodable for ValidatorPublicKey {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        Ok(ValidatorPublicKey(decode_fix_slice::<BLS_PUBLIC_KEY_SIZE>(
            r,
        )?))
    }
}

impl FromStr for ValidatorPublicKey {
    type Err = KeyPairError;

    fn from_str(s: &str) -> Result<Self, KeyPairError> {
        let (hrp, b32, _variant) = bech32::decode(s).map_err(|_| KeyPairError::InvalidPublicKey)?;
        if hrp != MAINNET_PUBLIC_KEY_HRP && hrp != TESTNET_PUBLIC_KEY_HRP {
            return Err(KeyPairError::InvalidPublicKey);
        }

        let b8 = Vec::<u8>::from_base32(&b32[1..]).map_err(|_| KeyPairError::InvalidPublicKey)?;
        let pub_data = b8.try_into().map_err(|_| KeyPairError::InvalidPublicKey)?;

        Ok(ValidatorPublicKey(pub_data))
    }
}

#[cfg(test)]
mod test {
    use std::str::FromStr;

    use tw_encoding::hex::DecodeHex;

    use crate::types::ValidatorPublicKey;

    #[test]
    fn test_public_key_string() {
        struct TestCase<'a> {
            name: &'a str,
            pub_key_str: &'a str,
            pub_key_data: &'a str,
        }

        // Define a list of test cases for encoding and decoding
        let test_cases = vec![
            TestCase {
                name: "invalid checksum",
                pub_key_str: "public1p4u8hfytl2pj6l9rj0t54gxcdmna4hq52ncqkkqjf3arha5mlk3x4mzpyjkhmdl20jae7f65aamjrvqcvf4sudcapz52ctcwc8r9wz3z2gwxs38880cgvfy49ta5ssyjut05myd4zgmjqstggmetyuyg7v5jhx470",
                pub_key_data: "",
            },
            TestCase {
                name: "invalid length: 95",
                pub_key_str: "public1p4u8hfytl2pj6l9rj0t54gxcdmna4hq52ncqkkqjf3arha5mlk3x4mzpyjkhmdl20jae7f65aamjrvqcvf4sudcapz52ctcwc8r9wz3z2gwxs38880cgvfy49ta5ssyjut05myd4zgmjqstggmetyuyg73y98kl",
                pub_key_data: "",
            },
            TestCase {
                name: "invalid HRP",
                pub_key_str: "xxx1p4u8hfytl2pj6l9rj0t54gxcdmna4hq52ncqkkqjf3arha5mlk3x4mzpyjkhmdl20jae7f65aamjrvqcvf4sudcapz52ctcwc8r9wz3z2gwxs38880cgvfy49ta5ssyjut05myd4zgmjqstggmetyuyg7v5evslaq",
                pub_key_data: "",
            },
            TestCase {
                name: "OK",
                pub_key_str: "public1p4u8hfytl2pj6l9rj0t54gxcdmna4hq52ncqkkqjf3arha5mlk3x4mzpyjkhmdl20jae7f65aamjrvqcvf4sudcapz52ctcwc8r9wz3z2gwxs38880cgvfy49ta5ssyjut05myd4zgmjqstggmetyuyg7v5jhx47a",
                pub_key_data: "af0f74917f5065af94727ae9541b0ddcfb5b828a9e016b02498f477ed37fb44d5d882495afb6fd4f9773e4ea9deee436030c4d61c6e3a1151585e1d838cae1444a438d089ce77e10c492a55f6908125c5be9b236a246e4082d08de564e111e65",
            },
            TestCase {
                name: "OK",
                pub_key_str: "tpublic1p4u8hfytl2pj6l9rj0t54gxcdmna4hq52ncqkkqjf3arha5mlk3x4mzpyjkhmdl20jae7f65aamjrvqcvf4sudcapz52ctcwc8r9wz3z2gwxs38880cgvfy49ta5ssyjut05myd4zgmjqstggmetyuyg7v5fmv7tx",
                pub_key_data: "af0f74917f5065af94727ae9541b0ddcfb5b828a9e016b02498f477ed37fb44d5d882495afb6fd4f9773e4ea9deee436030c4d61c6e3a1151585e1d838cae1444a438d089ce77e10c492a55f6908125c5be9b236a246e4082d08de564e111e65",
            },
       ];

        for case in test_cases {
            let pub_key_data = case.pub_key_data.decode_hex().unwrap().to_vec();
            let test_result = ValidatorPublicKey::from_str(case.pub_key_str);

            if pub_key_data.is_empty() {
                assert!(test_result.is_err());
            } else {
                assert!(test_result.is_ok());
                assert_eq!(
                    test_result.unwrap().0.to_vec(),
                    pub_key_data,
                    "test {} failed",
                    case.name
                );
            }
        }
    }
}
