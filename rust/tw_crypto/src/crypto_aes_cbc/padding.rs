// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub const BLOCK_SIZE_AES: usize = 16;

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum PaddingMode {
    Zero = 0,
    PKCS7 = 1,
}

#[derive(Clone, Copy, Debug)]
pub struct InvalidPaddingMode;

impl TryFrom<u32> for PaddingMode {
    type Error = InvalidPaddingMode;

    fn try_from(value: u32) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(PaddingMode::Zero),
            1 => Ok(PaddingMode::PKCS7),
            _ => Err(InvalidPaddingMode),
        }
    }
}

impl PaddingMode {
    pub fn padding_size(&self, data_size: usize) -> usize {
        match self {
            PaddingMode::Zero => {
                if data_size % BLOCK_SIZE_AES == 0 {
                    0
                } else {
                    BLOCK_SIZE_AES - (data_size % BLOCK_SIZE_AES)
                }
            },
            PaddingMode::PKCS7 => {
                if data_size % BLOCK_SIZE_AES == 0 {
                    BLOCK_SIZE_AES
                } else {
                    BLOCK_SIZE_AES - (data_size % BLOCK_SIZE_AES)
                }
            },
        }
    }

    pub fn pad(&self, data: &[u8]) -> Vec<u8> {
        let padding_size = self.padding_size(data.len());
        let mut padded = data.to_vec();
        match self {
            PaddingMode::Zero => {
                padded.extend(std::iter::repeat(0).take(padding_size));
            },
            PaddingMode::PKCS7 => {
                padded.extend(std::iter::repeat(padding_size as u8).take(padding_size));
            },
        }
        padded
    }

    pub fn unpad(&self, data: &[u8]) -> Vec<u8> {
        if data.is_empty() {
            return data.to_vec();
        }
        match self {
            PaddingMode::PKCS7 => {
                let padding_len = data[data.len() - 1] as usize;
                if padding_len <= data.len() {
                    data[..data.len() - padding_len].to_vec()
                } else {
                    data.to_vec()
                }
            },
            PaddingMode::Zero => {
                // Find the position of the last non-zero byte and return data up to that point
                let last_non_zero = data.iter().rposition(|&b| b != 0).map_or(0, |pos| pos + 1);
                data[..last_non_zero].to_vec()
            },
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_padding_size() {
        let zero_mode = PaddingMode::Zero;
        let pkcs7_mode = PaddingMode::PKCS7;

        assert_eq!(zero_mode.padding_size(0), 0);
        assert_eq!(zero_mode.padding_size(1), 15);
        assert_eq!(zero_mode.padding_size(8), 8);
        assert_eq!(zero_mode.padding_size(15), 1);
        assert_eq!(zero_mode.padding_size(16), 0);
        assert_eq!(zero_mode.padding_size(17), 15);
        assert_eq!(zero_mode.padding_size(24), 8);
        assert_eq!(zero_mode.padding_size(31), 1);
        assert_eq!(zero_mode.padding_size(32), 0);

        assert_eq!(pkcs7_mode.padding_size(0), 16);
        assert_eq!(pkcs7_mode.padding_size(1), 15);
        assert_eq!(pkcs7_mode.padding_size(8), 8);
        assert_eq!(pkcs7_mode.padding_size(15), 1);
        assert_eq!(pkcs7_mode.padding_size(16), 16);
        assert_eq!(pkcs7_mode.padding_size(17), 15);
        assert_eq!(pkcs7_mode.padding_size(24), 8);
        assert_eq!(pkcs7_mode.padding_size(31), 1);
        assert_eq!(pkcs7_mode.padding_size(32), 16);
    }
}
