// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::param_type::ParamType;

/// Output formatter for param type.
pub struct Writer;

impl Writer {
    /// Returns string which is a formatted representation of param.
    pub fn write(param: &ParamType) -> String {
        Writer::write_for_abi(param, true)
    }

    /// If `serialize_tuple_contents` is `true`, tuples will be represented
    /// as list of inner types in parens, for example `(int256,bool)`.
    /// If it is `false`, tuples will be represented as keyword `tuple`.
    pub fn write_for_abi(param: &ParamType, serialize_tuple_contents: bool) -> String {
        match param {
            ParamType::Address => "address".to_owned(),
            ParamType::Bytes => "bytes".to_owned(),
            ParamType::FixedBytes { len } => format!("bytes{len}"),
            ParamType::Int { bits } => format!("int{bits}"),
            ParamType::Uint { bits } => format!("uint{bits}"),
            ParamType::Bool => "bool".to_owned(),
            ParamType::String => "string".to_owned(),
            ParamType::FixedArray { kind, len } => {
                format!(
                    "{}[{len}]",
                    Writer::write_for_abi(kind, serialize_tuple_contents)
                )
            },
            ParamType::Array { ref kind } => {
                format!(
                    "{}[]",
                    Writer::write_for_abi(kind, serialize_tuple_contents)
                )
            },
            ParamType::Tuple { ref params } => {
                if serialize_tuple_contents {
                    let formatted = params
                        .iter()
                        .map(|t| Writer::write_for_abi(&t.kind, serialize_tuple_contents))
                        .collect::<Vec<String>>()
                        .join(",");
                    format!("({formatted})")
                } else {
                    "tuple".to_owned()
                }
            },
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::abi::non_empty_array::NonZeroLen;
    use crate::abi::param::Param;
    use crate::abi::param_type::constructor::TypeConstructor;

    #[test]
    fn test_write_param() {
        assert_eq!(Writer::write(&ParamType::Address), "address");
        assert_eq!(Writer::write(&ParamType::Bytes), "bytes");
        assert_eq!(
            Writer::write(&ParamType::FixedBytes {
                len: NonZeroLen::new(32).unwrap()
            }),
            "bytes32"
        );
        assert_eq!(Writer::write(&ParamType::u256()), "uint256");
        assert_eq!(Writer::write(&ParamType::int(64).unwrap()), "int64");
        assert_eq!(Writer::write(&ParamType::Bool), "bool");
        assert_eq!(Writer::write(&ParamType::String), "string");
        assert_eq!(Writer::write(&ParamType::array(ParamType::Bool)), "bool[]");
        assert_eq!(
            Writer::write(&ParamType::fixed_array(2, ParamType::String).unwrap()),
            "string[2]"
        );
        assert_eq!(
            Writer::write(&ParamType::fixed_array(2, ParamType::array(ParamType::Bool)).unwrap()),
            "bool[][2]"
        );
        assert_eq!(
            Writer::write(&ParamType::array(ParamType::Tuple {
                params: vec![
                    Param::with_type(ParamType::array(ParamType::Tuple {
                        params: vec![
                            Param::with_type(ParamType::int(256).unwrap()),
                            Param::with_type(ParamType::uint(256).unwrap())
                        ]
                    })),
                    Param::with_type(ParamType::fixed_bytes(32).unwrap()),
                ]
            })),
            "((int256,uint256)[],bytes32)[]"
        );

        assert_eq!(
            Writer::write_for_abi(
                &ParamType::array(ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::array(ParamType::i256())),
                        Param::with_type(ParamType::fixed_bytes(32).unwrap()),
                    ]
                }),
                false
            ),
            "tuple[]"
        );
    }
}
