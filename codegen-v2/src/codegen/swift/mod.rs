use self::functions::process_object_methods;
use self::inits::process_inits;
use self::properties::process_object_properties;
use crate::manifest::{FileInfo, ParamInfo, ProtoInfo, TypeInfo, TypeVariant};
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

#[derive(Debug, Clone, Default)]
pub struct RenderOutput {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
    pub protos: Vec<(String, String)>,
}

pub fn render_file_info<'a>(input: RenderIntput<'a>) -> Result<RenderOutput> {
    let mut engine = Handlebars::new();
    // Unmatched variables should result in an error.
    engine.set_strict_mode(true);

    engine.register_partial("struct", input.struct_template)?;
    engine.register_partial("enum", input.enum_template)?;
    engine.register_partial("extension", input.extension_template)?;
    engine.register_partial("proto", input.proto_template)?;

    let mut info = input.file_info;
    let mut outputs = RenderOutput::default();

    // Render structs/classes.
    for strct in info.structs {
        let (inits, mut methods, properties);
        (inits, info.inits) = process_inits(&ObjectVariant::Struct(&strct.name), info.inits)?;
        (methods, info.functions) =
            process_object_methods(&ObjectVariant::Struct(&strct.name), info.functions)?;
        (properties, info.properties) =
            process_object_properties(&ObjectVariant::Struct(&strct.name), info.properties)?;

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
            vec!["Address"]
        } else {
            vec![]
        };

        // Handle equality operator.
        let equality_method = methods.iter().enumerate().find(|(_, f)| f.name == "equal");

        let equality_operator = if let Some((idx, _func)) = equality_method {
            let operator = SwiftOperatorEquality {
                c_ffi_name: format!("{}Equal", strct.name),
            };

            // Remove that method from the `methods` list.
            methods.remove(idx);

            Some(operator)
        } else {
            None
        };

        let payload = json!({
            "name": struct_name,
            "is_class": strct.is_class,
            "init_instance": strct.is_class,
            "superclasses": superclasses,
            "eq_operator": equality_operator,
            "inits": inits,
            "deinits": info.deinits,
            "methods": methods,
            "properties": properties,
        });

        //  Render struct.
        let out = engine.render("struct", &payload)?;
        outputs.structs.push((struct_name.to_string(), out));
    }

    // Render enums.
    for enm in info.enums {
        let (methods, properties);
        (methods, info.functions) =
            process_object_methods(&ObjectVariant::Enum(&enm.name), info.functions)?;
        (properties, info.properties) =
            process_object_properties(&ObjectVariant::Enum(&enm.name), info.properties)?;

        // Stip "TW" prefix if present.
        let enum_name = enm.name.strip_prefix("TW").unwrap_or(&enm.name).to_string();

        // Add superclasses.
        let value_type = SwiftType::from(enm.value_type);
        let mut superclasses = vec![value_type.0.as_str(), "CaseIterable"];

        // If the enum variants have `as_string` fields, we generate a
        // description method.
        let description: Option<Vec<(&str, &str)>> =
            if enm.variants.iter().any(|e| e.as_string.is_some()) {
                superclasses.push("CustomStringConvertible");

                let mut desc = vec![];
                for var in &enm.variants {
                    if let Some(ref text) = var.as_string {
                        desc.push((var.name.as_str(), text.as_str()))
                    } else {
                        // If at least one variant has a `as_string` field, than
                        // *all* of others must too.
                        return Err(Error::BadFormat(
                            "missing as_string field for enum variant".to_string(),
                        ));
                    }
                }

                Some(desc)
            } else {
                None
            };

        let enum_payload = json!({
            "name": enum_name,
            "is_public": enm.is_public,
            "superclasses": superclasses,
            "variants": enm.variants,
            "description": description,
        });

        // Render enum.
        let out = engine.render("enum", &enum_payload)?;
        outputs.enums.push((enum_name.to_string(), out));

        // Avoid rendering empty extension for enums.
        if methods.is_empty() && properties.is_empty() {
            continue;
        }

        let extension_payload = json!({
            "name": enum_name,
            "init_instance": true,
            "parent_classes": [],
            "methods": methods,
            "properties": properties,
        });

        // Render enum extension.
        let out = engine.render("extension", &extension_payload)?;
        outputs.extensions.push((enum_name.to_string(), out));
    }

    // Render Protobufs.
    if !info.protos.is_empty() {
        let file_name = info
            .name
            .strip_prefix("TW")
            .unwrap_or(&info.name)
            .strip_suffix("Proto")
            .unwrap_or(&info.name)
            .to_string();

        let protos = info
            .protos
            .into_iter()
            .map(SwiftProto::try_from)
            .collect::<Result<Vec<_>>>()?;

        let payload = json!({
            "protos": protos,
        });

        // Render protobuf.
        let out = engine.render("proto", &payload)?;
        outputs.protos.push((file_name, out));
    }

    Ok(outputs)
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
        let mut name = value.0.replace("_", "");
        name = name.replace("TW", "");
        name = name.replace("Proto", "");

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
            TypeVariant::Void => "()".to_string(),
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
