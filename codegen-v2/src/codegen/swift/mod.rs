// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use self::functions::process_methods;
use self::inits::process_inits;
use self::properties::process_properties;
use crate::manifest::{
    DeinitInfo, FileInfo, ParamInfo, ProtoInfo, TypeInfo, TypeVariant,
};
use crate::{Error, Result};
use handlebars::Handlebars;
use serde_json::json;
use std::fmt::Display;

mod functions;
mod inits;
mod properties;

#[derive(Debug, Clone)]
pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub struct_template: &'a str,
    pub enum_template: &'a str,
    pub extension_template: &'a str,
    pub proto_template: &'a str,
}

#[derive(Debug, Clone, Serialize)]
pub struct WithYear<'a, T> {
    pub current_year: u64,
    #[serde(flatten)]
    pub data: &'a T,
}

#[derive(Debug, Clone, Default)]
pub struct RenderOutputStrings {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
    pub protos: Vec<(String, String)>,
}

#[derive(Debug, Clone, Default)]
pub struct RenderOutput {
    structs: Vec<SwiftStruct>,
    enums: Vec<SwiftEnum>,
    extensions: Vec<SwiftEnumExtension>,
    protos: Vec<SwiftProto>,
}

pub fn render_file_info_strings<'a>(input: RenderIntput<'a>) -> Result<RenderOutputStrings> {
    // The current year for the copyright header in the generated bindings.
    let current_year = crate::current_year();
    let file_name = input.file_info.name.replace("TW", "").replace("Proto", "");

    let mut engine = Handlebars::new();
    // Unmatched variables should result in an error.
    engine.set_strict_mode(true);

    engine.register_partial("struct", input.struct_template)?;
    engine.register_partial("enum", input.enum_template)?;
    engine.register_partial("extension", input.extension_template)?;
    engine.register_partial("proto", input.proto_template)?;

    let rendered = render_file_info(input)?;
    let mut out_str = RenderOutputStrings::default();

    //  Render structs.
    for strct in rendered.structs {
        let out = engine.render(
            "struct",
            &WithYear {
                current_year,
                data: &strct,
            },
        )?;

        out_str.structs.push((strct.name.to_string(), out));
    }

    //  Render enums.
    for enm in rendered.enums {
        let out = engine.render(
            "enum",
            &WithYear {
                current_year,
                data: &enm,
            },
        )?;

        out_str.enums.push((enm.name.to_string(), out));
    }

    //  Render extensions.
    for ext in rendered.extensions {
        let out = engine.render(
            "extension",
            &WithYear {
                current_year,
                data: &ext,
            },
        )?;

        out_str.extensions.push((ext.name.to_string(), out));
    }

    //  Render protos.
    if !rendered.protos.is_empty() {
        let out = engine.render(
            "proto",
            &WithYear {
                current_year,
                data: &json!({
                    "protos": &rendered.protos
                }),
            },
        )?;

        out_str.protos.push((file_name, out));
    }

    Ok(out_str)
}

/// Uses the given input templates to render all files.
pub fn render_file_info<'a>(input: RenderIntput<'a>) -> Result<RenderOutput> {
    let mut info = input.file_info;
    let mut outputs = RenderOutput::default();

    // Render structs/classes.
    for strct in info.structs {
        let obj = ObjectVariant::Struct(&strct.name);

        // Process items.
        let (inits, mut methods, properties);
        (inits, info.inits) = process_inits(&ObjectVariant::Struct(&strct.name), info.inits)?;
        (methods, info.functions) = process_methods(&obj, info.functions)?;
        (properties, info.properties) = process_properties(&obj, info.properties)?;

        // Avoid rendering empty structs.
        if inits.is_empty() && methods.is_empty() && properties.is_empty() {
            continue;
        }

        // Stip "TW" prefix if present.
        let struct_name = strct
            .name
            .strip_prefix("TW")
            .unwrap_or(&strct.name)
            .to_string();

        // Add superclasses.
        let superclasses = if struct_name.ends_with("Address") {
            vec!["Address".to_string()]
        } else {
            vec![]
        };

        // Handle equality operator.
        let eq_method = methods.iter().enumerate().find(|(_, f)| f.name == "equal");
        let eq_operator = if let Some((idx, _func)) = eq_method {
            let operator = SwiftOperatorEquality {
                c_ffi_name: format!("{}Equal", strct.name),
            };

            // Remove that method from the `methods` list.
            methods.remove(idx);

            Some(operator)
        } else {
            None
        };

        outputs.structs.push(SwiftStruct {
            name: struct_name,
            is_class: strct.is_class,
            init_instance: strct.is_class,
            superclasses,
            eq_operator,
            inits: inits,
            // TODO:
            deinits: info.deinits.clone(),
            methods,
            properties,
        });
    }

    // Render enums.
    for enm in info.enums {
        let obj = ObjectVariant::Enum(&enm.name);

        // Process items.
        let (methods, properties);
        (methods, info.functions) = process_methods(&obj, info.functions)?;
        (properties, info.properties) = process_properties(&obj, info.properties)?;

        // Stip "TW" prefix if present.
        let enum_name = enm.name.strip_prefix("TW").unwrap_or(&enm.name).to_string();

        // Add superclasses.
        let value_type = SwiftType::from(enm.value_type);
        let mut superclasses = vec![value_type.0, "CaseIterable".to_string()];

        let mut add_class = false;

        // Convert to Swift enum variants
        let variants = enm
            .variants
            .into_iter()
            .map(|info| {
                if info.as_string.is_some() {
                    add_class = true;
                }

                SwiftEnumVariant {
                    name: info.name,
                    value: info.value,
                    as_string: info.as_string,
                }
            })
            .collect();

        if add_class {
            superclasses.push("CustomStringConvertible".to_string());
        }

        outputs.enums.push(SwiftEnum {
            name: enum_name.clone(),
            is_public: enm.is_public,
            add_description: add_class,
            superclasses,
            variants,
        });

        // Avoid rendering empty extension for enums.
        if methods.is_empty() && properties.is_empty() {
            continue;
        }

        outputs.extensions.push(SwiftEnumExtension {
            name: enum_name,
            init_instance: true,
            methods,
            properties,
        });
    }

    // Render Protobufs.
    if !info.protos.is_empty() {
        for proto in info.protos {
            outputs.protos.push(SwiftProto::try_from(proto)?);
        }
    }

    Ok(outputs)
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftStruct {
    name: String,
    is_class: bool,
    init_instance: bool,
    superclasses: Vec<String>,
    eq_operator: Option<SwiftOperatorEquality>,
    inits: Vec<SwiftInit>,
    deinits: Vec<DeinitInfo>,
    methods: Vec<SwiftFunction>,
    properties: Vec<SwiftProperty>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnum {
    name: String,
    is_public: bool,
    add_description: bool,
    superclasses: Vec<String>,
    variants: Vec<SwiftEnumVariant>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnumVariant {
    name: String,
    value: usize,
    as_string: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftEnumExtension {
    name: String,
    init_instance: bool,
    methods: Vec<SwiftFunction>,
    properties: Vec<SwiftProperty>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftType(String);

impl Display for SwiftType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

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

#[derive(Debug, Clone, Serialize, Deserialize)]
struct SwiftProperty {
    pub name: String,
    pub is_public: bool,
    pub operations: Vec<SwiftOperation>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
/// The operation to be interpreted by the templating engine. This handles
/// parameters and C FFI calls in an appropriate way, depending on context.
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
        let name = value
            .0
            .replace("_", "")
            .replace("TW", "")
            .replace("Proto", "");

        Ok(SwiftProto {
            name,
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

// Covenience function: process the paremter, returning the operation for
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
