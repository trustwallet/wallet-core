// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use self::functions::process_methods;
use self::inits::process_inits;
use self::properties::process_properties;
use self::render::pretty_name;
use crate::manifest::{DeinitInfo, FileInfo, ParamInfo, ProtoInfo, TypeInfo, TypeVariant};
use crate::{Error, Result};
use handlebars::Handlebars;
use serde_json::json;
use std::fmt::Display;

mod functions;
mod inits;
mod properties;
mod render;

// Re-exports
pub use self::render::{
    generate_swift_types, render_to_strings, GeneratedSwiftTypes, GeneratedSwiftTypesStrings,
    RenderIntput,
};

/// Represents a Swift struct or class.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftStruct {
    name: String,
    is_class: bool,
    is_public: bool,
    init_instance: bool,
    superclasses: Vec<String>,
    eq_operator: Option<SwiftOperatorEquality>,
    inits: Vec<SwiftInit>,
    deinits: Vec<DeinitInfo>,
    methods: Vec<SwiftFunction>,
    properties: Vec<SwiftProperty>,
}

/// Represents a Swift enum.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnum {
    name: String,
    is_public: bool,
    add_description: bool,
    superclasses: Vec<String>,
    variants: Vec<SwiftEnumVariant>,
}

/// Represents a Swift enum variant.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnumVariant {
    name: String,
    value: String,
    as_string: Option<String>,
}

/// Represents associated methods and properties of an enum. Based on the first
/// codegen, those extensions are placed in a separate file.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnumExtension {
    name: String,
    init_instance: bool,
    methods: Vec<SwiftFunction>,
    properties: Vec<SwiftProperty>,
}

// Wrapper around a valid Swift type (built in or custom). Meant to be used as
// `<SwiftType as From<TypeVariant>>::from(...)`.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftType(String);

impl Display for SwiftType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

/// Represents a Swift function or method.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftFunction {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub params: Vec<SwiftParam>,
    pub operations: Vec<SwiftOperation>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

/// Represents a Swift property of a struct/class or enum.
#[derive(Debug, Clone, Serialize, Deserialize)]
struct SwiftProperty {
    pub name: String,
    pub is_public: bool,
    pub operations: Vec<SwiftOperation>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

/// The operation to be interpreted by the templating engine. This handles
/// parameters and C FFI calls in an appropriate way, depending on context.
#[derive(Debug, Clone, Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
pub enum SwiftOperation {
    // Results in:
    // ```swift
    // let <var_name> = <call>
    // defer {
    //     <defer.String>(<var_name>)
    // }
    // ```
    Call {
        var_name: String,
        call: String,
        defer: Option<String>,
    },
    // Results in:
    // ```swift
    // let ptr: UnsafeRawPointer?
    // if let alphabet = alphabet {
    //     ptr = TWStringCreateWithNSString(alphabet)
    // } else {
    //     ptr = nil
    // }
    // ```
    // ... with an optional `defer` operation.
    CallOptional {
        var_name: String,
        call: String,
        defer: Option<String>,
    },
    // Results in:
    // ```swift
    // let <var_name> = <call>
    // guard let <var_name> = <var_name> else {
    //     return nil
    // }
    // ```
    GuardedCall {
        var_name: String,
        call: String,
    },
    // Results in:
    // ```swift
    // return <call>
    // ```
    Return {
        call: String,
    },
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftParam {
    pub name: String,
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftReturn {
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftInit {
    pub name: String,
    pub is_nullable: bool,
    pub is_public: bool,
    pub params: Vec<SwiftParam>,
    pub operations: Vec<SwiftOperation>,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftProto {
    pub name: String,
    pub c_ffi_name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftOperatorEquality {
    pub c_ffi_name: String,
}

/// Used for the individual `process_*` functions.
enum ObjectVariant<'a> {
    Struct(&'a str),
    Enum(&'a str),
}

impl<'a> ObjectVariant<'a> {
    fn name(&'a self) -> &'a str {
        match self {
            ObjectVariant::Struct(n) | ObjectVariant::Enum(n) => n,
        }
    }
}

impl TryFrom<ProtoInfo> for SwiftProto {
    type Error = Error;

    fn try_from(value: ProtoInfo) -> std::result::Result<Self, Self::Error> {
        Ok(SwiftProto {
            // Convert the name into an appropriate format.
            name: pretty_name(value.0.clone()),
            c_ffi_name: value.0,
        })
    }
}

/// Convert the `TypeVariant` into the appropriate Swift type.
impl From<TypeVariant> for SwiftType {
    fn from(value: TypeVariant) -> Self {
        let res = match value {
            TypeVariant::Void => "Void".to_string(),
            TypeVariant::Bool => "Bool".to_string(),
            TypeVariant::Char => "Character".to_string(),
            TypeVariant::ShortInt => "Int16".to_string(),
            TypeVariant::Int => "Int32".to_string(),
            TypeVariant::UnsignedInt => "UInt32".to_string(),
            TypeVariant::LongInt => "Int64".to_string(),
            TypeVariant::Float => "Float".to_string(),
            TypeVariant::Double => "Double".to_string(),
            TypeVariant::SizeT => "Int".to_string(),
            TypeVariant::Int8T => "Int8".to_string(),
            TypeVariant::Int16T => "Int16".to_string(),
            TypeVariant::Int32T => "Int32".to_string(),
            TypeVariant::Int64T => "Int64".to_string(),
            TypeVariant::UInt8T => "UInt8".to_string(),
            TypeVariant::UInt16T => "UInt16".to_string(),
            TypeVariant::UInt32T => "UInt32".to_string(),
            TypeVariant::UInt64T => "UInt64".to_string(),
            TypeVariant::String => "String".to_string(),
            TypeVariant::Data => "Data".to_string(),
            TypeVariant::Struct(n) | TypeVariant::Enum(n) => {
                // We strip the "TW" prefix for Swift representations of
                // structs/enums.
                n.strip_prefix("TW").map(|n| n.to_string()).unwrap_or(n)
            }
        };

        SwiftType(res)
    }
}

// Covenience function: process the parameter, returning the operation for
// handling the C FFI call (if any).
fn param_c_ffi_call(param: &ParamInfo) -> Option<SwiftOperation> {
    let op = match &param.ty.variant {
        // E.g. `let param = TWStringCreateWithNSString(param)`
        TypeVariant::String => {
            let (var_name, call, defer) = (
                param.name.clone(),
                format!("TWStringCreateWithNSString({})", param.name),
                Some(format!("TWStringDelete({})", param.name)),
            );

            // If the parameter is nullable, add special handler.
            if param.ty.is_nullable {
                SwiftOperation::CallOptional {
                    var_name,
                    call,
                    defer,
                }
            } else {
                SwiftOperation::Call {
                    var_name,
                    call,
                    defer,
                }
            }
        }
        TypeVariant::Data => {
            let (var_name, call, defer) = (
                param.name.clone(),
                format!("TWDataCreateWithNSData({})", param.name),
                Some(format!("TWDataDelete({})", param.name)),
            );

            // If the parameter is nullable, add special handler.
            if param.ty.is_nullable {
                SwiftOperation::CallOptional {
                    var_name,
                    call,
                    defer,
                }
            } else {
                SwiftOperation::Call {
                    var_name,
                    call,
                    defer,
                }
            }
        }
        // E.g.
        // - `let param = param.rawValue`
        // - `let param = param?.rawValue`
        TypeVariant::Struct(_) => {
            // For nullable structs, we do not use the special
            // `CallOptional` handler but rather use the question mark
            // operator.
            let (var_name, call, defer) = if param.ty.is_nullable {
                (
                    param.name.clone(),
                    format!("{}?.rawValue", param.name),
                    None,
                )
            } else {
                (param.name.clone(), format!("{}.rawValue", param.name), None)
            };

            SwiftOperation::Call {
                var_name,
                call,
                defer,
            }
        }
        // E.g. `let param = TWSomeEnum(rawValue: param.rawValue)`
        // Note that it calls the constructor of the enum, which calls
        // the underlying "*Create*" C FFI function.
        TypeVariant::Enum(enm) => SwiftOperation::Call {
            var_name: param.name.clone(),
            call: format!("{enm}(rawValue: {}.rawValue)", param.name),
            defer: None,
        },
        // Skip processing parameter, reference the parameter by name
        // directly, as defined in the function interface (usually the
        // case for primitive types).
        _ => return None,
    };

    Some(op)
}

// Convenience funcion: wrap the return value, returning the operation. Note
// that types are wrapped differently when returning, compared to
// `param_c_ffi_call`; such as using `TWStringNSString` instead of
// `TWDataCreateWithNSData` for Strings.
fn wrap_return(ty: &TypeInfo) -> SwiftOperation {
    match &ty.variant {
        // E.g.`return TWStringNSString(result)`
        TypeVariant::String => SwiftOperation::Return {
            call: "TWStringNSString(result)".to_string(),
        },
        TypeVariant::Data => SwiftOperation::Return {
            call: "TWDataNSData(result)".to_string(),
        },
        // E.g. `return SomeEnum(rawValue: result.rawValue)`
        TypeVariant::Enum(_) => SwiftOperation::Return {
            call: format!(
                "{}(rawValue: result.rawValue)!",
                SwiftType::from(ty.variant.clone())
            ),
        },
        // E.g. `return SomeStruct(rawValue: result)`
        TypeVariant::Struct(_) => SwiftOperation::Return {
            call: format!("{}(rawValue: result)", SwiftType::from(ty.variant.clone())),
        },
        _ => SwiftOperation::Return {
            call: "result".to_string(),
        },
    }
}
