// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::NonZeroLen;
use crate::abi::param_type::constructor::TypeConstructor;
use crate::abi::uint::UintBits;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;

const MAX_RECURSION_DEPTH: usize = 20;

pub struct Reader;

impl Reader {
    /// Doesn't accept tuple types with specified parameters, e.g `(uint32, address)`.
    pub fn parse_type<T: TypeConstructor>(s: &str) -> AbiResult<T> {
        Self::parse_type_step(s, 0)
    }

    /// Doesn't accept tuple types with specified parameters, e.g `(uint32, address)`.
    pub fn parse_type_step<T: TypeConstructor>(s: &str, current_depth: usize) -> AbiResult<T> {
        if current_depth > MAX_RECURSION_DEPTH {
            return AbiError::err(AbiErrorKind::Error_invalid_param_type)
                .with_context(|| format!("Max recursion depth exceeded: {MAX_RECURSION_DEPTH}"));
        }

        // Array
        if let Some(remaining) = s.strip_suffix(']') {
            let Some((element_type_str, len_str)) = remaining.rsplit_once('[') else {
                return AbiError::err(AbiErrorKind::Error_invalid_param_type)
                    .with_context(|| format!("Invalid array type: {s}"));
            };

            let element_type = Reader::parse_type_step::<T>(element_type_str, current_depth + 1)
                .with_context(|| format!("Error parsing inner array type: {element_type_str}"))?;
            if let Some(len) = parse_len(len_str)
                .with_context(|| format!("Error parsing fixed_array length: {len_str}"))?
            {
                return Ok(T::fixed_array_checked(len, element_type));
            }
            return Ok(T::array(element_type));
        }

        let all_alphanumeric = s.chars().all(|ch| ch.is_ascii_alphanumeric());
        if s.is_empty() || !all_alphanumeric {
            return AbiError::err(AbiErrorKind::Error_invalid_param_type)
                .with_context(|| format!("Expected an alpha-numeric string type: {s}"));
        }

        if s.contains(['[', ']']) {
            return AbiError::err(AbiErrorKind::Error_invalid_param_type);
        }

        // uint, uint32, ...
        if let Some(len_str) = s.strip_prefix("uint") {
            let bits = parse_uint_bits(len_str)
                .with_context(|| format!("Error parsing uint bits: {len_str}"))?
                .unwrap_or_default();
            return Ok(T::uint_checked(bits));
        }

        // int, int32, ...
        if let Some(len_str) = s.strip_prefix("int") {
            let bits = parse_uint_bits(len_str)
                .with_context(|| format!("Error parsing int bits: {len_str}"))?
                .unwrap_or_default();
            return Ok(T::int_checked(bits));
        }

        // bytes, bytes32, ...
        if let Some(len_str) = s.strip_prefix("bytes") {
            if let Some(len) = parse_len(len_str)
                .with_context(|| format!("Error parsing fixed_bytes length: {len_str}"))?
            {
                // Fixed-len bytes.
                return Ok(T::fixed_bytes_checked(len));
            }
            // Otherwise, dynamic-len bytes.
            return Ok(T::bytes());
        }

        // Handle other types.
        match s {
            "address" => Ok(T::address()),
            "bool" => Ok(T::bool()),
            "string" => Ok(T::string()),
            "tuple" => T::empty_tuple(),
            custom => T::custom(custom),
        }
    }
}

fn parse_len(len_str: &str) -> AbiResult<Option<NonZeroLen>> {
    match parse_usize(len_str)? {
        Some(u) => {
            let len = NonZeroLen::new(u)?;
            Ok(Some(len))
        },
        None => Ok(None),
    }
}

fn parse_uint_bits(bits_str: &str) -> AbiResult<Option<UintBits>> {
    match parse_usize(bits_str)? {
        Some(u) => {
            let bits = UintBits::new(u)?;
            Ok(Some(bits))
        },
        None => Ok(None),
    }
}

fn parse_usize(usize_str: &str) -> AbiResult<Option<usize>> {
    if usize_str.is_empty() {
        return Ok(None);
    }
    if usize_str.starts_with('0') {
        return AbiError::err(AbiErrorKind::Error_invalid_param_type)
            .context("Number cannot start with 0");
    }
    usize::from_str(usize_str)
        .map(Some)
        .tw_err(AbiErrorKind::Error_invalid_param_type)
        .with_context(|| format!("Expected a decimal string: {usize_str}"))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::abi::param_type::ParamType;

    #[test]
    fn test_parse_type_recursion() {
        let depth = MAX_RECURSION_DEPTH;
        let mut ty = "uint256".to_string();

        // Append exactly MAX_RECURSION_DEPTH array brackets that must be allowed.
        for _ in 0..depth {
            ty.push_str("[]");
        }
        ParamType::try_from_type_short(&ty).expect("Should parse within recursion limit");

        // Append one more array bracket to exceed the limit.
        ty.push_str("[]");
        ParamType::try_from_type_short(&ty).expect_err("Recursion limit exceeded");
    }
}
