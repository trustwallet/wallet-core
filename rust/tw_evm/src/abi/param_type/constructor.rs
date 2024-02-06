// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::NonZeroLen;
use crate::abi::param_type::ParamType;
use crate::abi::uint::UintBits;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};

pub trait TypeConstructor: Sized {
    fn address() -> Self;

    fn bytes() -> Self;

    fn fixed_bytes(len: usize) -> AbiResult<Self> {
        let checked_len = NonZeroLen::new(len)?;
        Ok(Self::fixed_bytes_checked(checked_len))
    }

    fn fixed_bytes_checked(len: NonZeroLen) -> Self;

    fn int(bits: usize) -> AbiResult<Self> {
        let checked_bits = UintBits::new(bits)?;
        Ok(Self::int_checked(checked_bits))
    }

    fn int_checked(bits: UintBits) -> Self;

    fn uint(bits: usize) -> AbiResult<Self> {
        let checked_bits = UintBits::new(bits)?;
        Ok(Self::uint_checked(checked_bits))
    }

    fn uint_checked(bits: UintBits) -> Self;

    fn bool() -> Self;

    fn string() -> Self;

    fn array(element_type: Self) -> Self;

    fn fixed_array(len: usize, element_type: Self) -> AbiResult<Self> {
        let checked_len = NonZeroLen::new(len)?;
        Ok(Self::fixed_array_checked(checked_len, element_type))
    }

    fn fixed_array_checked(len: NonZeroLen, element_type: Self) -> Self;

    fn empty_tuple() -> AbiResult<Self>;

    fn custom(s: &str) -> AbiResult<Self>;
}

impl TypeConstructor for ParamType {
    fn address() -> Self {
        ParamType::Address
    }

    fn bytes() -> Self {
        ParamType::Bytes
    }

    fn fixed_bytes_checked(len: NonZeroLen) -> Self {
        ParamType::FixedBytes { len }
    }

    fn int_checked(bits: UintBits) -> Self {
        ParamType::Int { bits }
    }

    fn uint_checked(bits: UintBits) -> Self {
        ParamType::Uint { bits }
    }

    fn bool() -> Self {
        ParamType::Bool
    }

    fn string() -> Self {
        ParamType::String
    }

    fn array(element_type: Self) -> Self {
        ParamType::Array {
            kind: Box::new(element_type),
        }
    }

    fn fixed_array_checked(len: NonZeroLen, element_type: Self) -> Self {
        ParamType::FixedArray {
            kind: Box::new(element_type),
            len,
        }
    }

    fn empty_tuple() -> AbiResult<Self> {
        Ok(ParamType::Tuple {
            params: Vec::default(),
        })
    }

    fn custom(_s: &str) -> AbiResult<Self> {
        Err(AbiError(AbiErrorKind::Error_invalid_param_type))
    }
}
