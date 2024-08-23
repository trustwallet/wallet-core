// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use self::functions::process_methods;
use self::inits::process_inits;
use self::properties::process_properties;
use crate::codegen::dart::utils::pretty_name;
use crate::manifest::{DeinitInfo, FileInfo, ParamInfo, ProtoInfo, TypeInfo, TypeVariant};
use crate::{Error, Result};
use handlebars::Handlebars;
use serde_json::json;
use std::fmt::Display;

mod functions;
mod inits;
mod properties;
mod render;
mod utils;

// Re-exports
pub use self::render::{
    generate_dart_types, render_to_strings, GeneratedDartTypes, GeneratedDartTypesStrings,
    RenderInput,
};

/// Represents a Swift struct or class.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartStruct {
    name: String,
    is_class: bool,
    is_public: bool,
    init_instance: bool,
    imports: Vec<String>,
    superclasses: Vec<String>,
    eq_operator: Option<DartOperatorEquality>,
    inits: Vec<DartInit>,
    deinits: Vec<DeinitInfo>,
    methods: Vec<DartFunction>,
    properties: Vec<DartProperty>,
}

/// Represents a Dart enum.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartEnum {
    name: String,
    is_public: bool,
    add_description: bool,
    variants: Vec<DartEnumVariant>,
    value_type: String,
}

/// Represents a Swift enum variant.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartEnumVariant {
    name: String,
    value: String,
    as_string: Option<String>,
}

/// Represents associated methods and properties of an enum. Based on the first
/// codegen, those extensions are placed in a separate file.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartEnumExtension {
    name: String,
    init_instance: bool,
    methods: Vec<DartFunction>,
    properties: Vec<DartProperty>,
}

// Wrapper around a valid Dart type (built in or custom). Meant to be used as
// `<DartType as From<TypeVariant>>::from(...)`.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartType(String);

impl Display for DartType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

/// Represents a Swift function or method.
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartFunction {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub params: Vec<DartParam>,
    pub operations: Vec<DartOperation>,
    #[serde(rename = "return")]
    pub return_type: DartReturn,
    pub comments: Vec<String>,
}

/// Represents a Swift property of a struct/class or enum.
#[derive(Debug, Clone, Serialize, Deserialize)]
struct DartProperty {
    pub name: String,
    pub is_public: bool,
    pub is_override: bool,
    pub operations: Vec<DartOperation>,
    #[serde(rename = "return")]
    pub return_type: DartReturn,
    pub comments: Vec<String>,
}

/// The operation to be interpreted by the templating engine. This handles
/// parameters and C FFI calls in an appropriate way, depending on context.
#[derive(Debug, Clone, Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
pub enum DartOperation {
    // Results in:
    // ```dart
    // final <var_name> = <call>;
    // ```
    Call {
        var_name: String,
        call: String,
    },
    // Results in:
    // ```dart
    // final UnsafeRawPointer? ptr;
    // if alphabet != null {
    //     ptr = TWStringCreateWithNSString(alphabet);
    // } else {
    //     ptr = null;
    // }
    // final alphabet = ptr;
    // ```
    CallOptional {
        var_name: String,
        call: String,
    },
    // Results in:
    // ```swift
    // final <var_name> = <call>;
    // if <var_name> == null {
    //     return null;
    // }
    // ```
    GuardedCall {
        var_name: String,
        call: String,
    },
    // Results in:
    // ```dart
    //  <call>(<var_name>);
    DeferCall {
        var_name: String,
        call: Option<String>,
    },
    // Results in:
    // ```dart
    // if (ptr != null) {
    //    <call>(<var_name>);
    // }
    // ```
    DeferOptionalCall {
        var_name: String,
        call: Option<String>,
    },
    // Results in:
    // ```dart
    // return <call>;
    // ```
    Return {
        call: String,
    },
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartParam {
    pub name: String,
    #[serde(rename = "type")]
    pub param_type: DartType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartReturn {
    #[serde(rename = "type")]
    pub param_type: DartType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartInit {
    pub name: String,
    pub class_name: String,
    pub is_nullable: bool,
    pub is_public: bool,
    pub params: Vec<DartParam>,
    pub operations: Vec<DartOperation>,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartProto {
    pub name: String,
    pub c_ffi_name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DartOperatorEquality {
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

impl TryFrom<ProtoInfo> for DartProto {
    type Error = Error;

    fn try_from(value: ProtoInfo) -> std::result::Result<Self, Self::Error> {
        Ok(DartProto {
            // Convert the name into an appropriate format.
            name: pretty_name(value.0.clone()),
            c_ffi_name: value.0,
        })
    }
}

/// Convert the `TypeVariant` into the appropriate Dart type.
impl From<TypeVariant> for DartType {
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

        DartType(res)
    }
}

// Convenience function: process the parameter, returning the operation for
// handling the C FFI call (if any).
fn param_c_ffi_call(param: &ParamInfo) -> Option<DartOperation> {
    let op = match &param.ty.variant {
        // E.g. `final param = TWStringCreateWithNSString(param);`
        TypeVariant::String => {
            let (var_name, call) = (
                param.name.clone(),
                format!("TWStringCreateWithNSString({})", param.name),
            );

            // If the parameter is nullable, add special handler.
            if param.ty.is_nullable {
                DartOperation::CallOptional { var_name, call }
            } else {
                DartOperation::Call { var_name, call }
            }
        }
        TypeVariant::Data => {
            let (var_name, call) = (
                param.name.clone(),
                format!("TWDataCreateWithNSData({})", param.name),
            );

            // If the parameter is nullable, add special handler.
            if param.ty.is_nullable {
                DartOperation::CallOptional { var_name, call }
            } else {
                DartOperation::Call { var_name, call }
            }
        }
        // E.g.
        // - `final param = param.rawValue`
        // - `final param = param?.rawValue`
        TypeVariant::Struct(_) => {
            // For nullable structs, we do not use the special
            // `CallOptional` handler but rather use the question mark
            // operator.
            let (var_name, call) = if param.ty.is_nullable {
                (param.name.clone(), format!("{}?.rawValue", param.name))
            } else {
                (param.name.clone(), format!("{}.rawValue", param.name))
            };

            DartOperation::Call { var_name, call }
        }
        // E.g. `final param = TWSomeEnum(rawValue: param.rawValue);`
        // Note that it calls the constructor of the enum, which calls
        // the underlying "*Create*" C FFI function.
        TypeVariant::Enum(enm) => DartOperation::Call {
            var_name: param.name.clone(),
            call: format!("{enm}(rawValue: {}.rawValue)", param.name),
        },
        // Skip processing parameter, reference the parameter by name
        // directly, as defined in the function interface (usually the
        // case for primitive types).
        _ => return None,
    };

    Some(op)
}

fn param_c_ffi_defer_call(param: &ParamInfo) -> Option<DartOperation> {
    let op = match &param.ty.variant {
        TypeVariant::String => {
            let (var_name, call) = (
                param.name.clone(),
                Some(format!("TWStringDelete({})", param.name)),
            );

            if param.ty.is_nullable {
                DartOperation::DeferOptionalCall { var_name, call }
            } else {
                DartOperation::DeferCall { var_name, call }
            }
        }
        TypeVariant::Data => {
            let (var_name, call) = (
                param.name.clone(),
                Some(format!("TWDataDelete({})", param.name)),
            );

            if param.ty.is_nullable {
                DartOperation::DeferOptionalCall { var_name, call }
            } else {
                DartOperation::DeferCall { var_name, call }
            }
        }
        _ => return None,
    };

    Some(op)
}

// Convenience function: wrap the return value, returning the operation. Note
// that types are wrapped differently when returning, compared to
// `param_c_ffi_call`; such as using `TWStringNSString` instead of
// `TWDataCreateWithNSData` for Strings.
fn wrap_return(ty: &TypeInfo) -> DartOperation {
    match &ty.variant {
        // E.g.`return TWStringNSString(result)`
        TypeVariant::String => DartOperation::Return {
            call: "TWStringNSString(result)".to_string(),
        },
        TypeVariant::Data => DartOperation::Return {
            call: "TWDataNSData(result)".to_string(),
        },
        // E.g. `return SomeEnum(rawValue: result.rawValue)`
        TypeVariant::Enum(_) => DartOperation::Return {
            call: format!(
                "{}(rawValue: result.rawValue)!",
                DartType::from(ty.variant.clone())
            ),
        },
        // E.g. `return SomeStruct(rawValue: result)`
        TypeVariant::Struct(_) => DartOperation::Return {
            call: format!("{}(rawValue: result)", DartType::from(ty.variant.clone())),
        },
        _ => DartOperation::Return {
            call: "result".to_string(),
        },
    }
}
