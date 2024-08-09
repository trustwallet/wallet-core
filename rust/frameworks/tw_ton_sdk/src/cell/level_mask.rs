// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/level_mask.rs

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct LevelMask {
    mask: u32,
}

impl LevelMask {
    pub fn new(new_mask: u32) -> Self {
        Self { mask: new_mask }
    }

    pub fn mask(&self) -> u32 {
        self.mask
    }

    pub fn level(&self) -> u8 {
        32 - self.mask.leading_zeros() as u8
    }

    pub fn hash_index(&self) -> usize {
        self.mask.count_ones() as usize
    }

    pub fn hash_count(&self) -> usize {
        self.hash_index() + 1
    }

    pub fn apply(&self, level: u8) -> Self {
        LevelMask {
            mask: self.mask & ((1u32 << level) - 1),
        }
    }

    pub fn apply_or(&self, other: Self) -> Self {
        LevelMask {
            mask: self.mask | other.mask,
        }
    }

    pub fn shift_right(&self) -> Self {
        LevelMask {
            mask: self.mask >> 1,
        }
    }

    pub fn is_significant(&self, level: u8) -> bool {
        level == 0 || ((self.mask >> (level - 1)) % 2 != 0)
    }
}
