use crate::grammar::{GFunctionDecl, GKeyword, GMarker, GPrimitive, GType, GTypeCategory};
use crate::{Error, Result};
use handlebars::Handlebars;

pub const METHOD_INFO: &str = "part_method.hbs";

#[derive(Serialize, Deserialize)]
pub struct MethodInfo {
    method_name: String,
    is_static: bool,
    #[serde(rename = "return")]
    return_info: ReturnInfo,
    params: Vec<ParamInfo>,
    c_ffi_name: String,
}

#[derive(Serialize, Deserialize)]
pub struct ReturnInfo {
    #[serde(rename = "type")]
    return_type: String,
    is_nullable: bool,
    wrap_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct ParamInfo {
    name: String,
    #[serde(rename = "type")]
    param_type: String,
    is_nullable: bool,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

struct SwiftTypeContext {
    name: String,
    ty: String,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

// Converts the parsed grammar `GType` into the corresponding Swift type,
// including how it should be wrapped and/or deterred. The types must be
// specifically matched/supported, otherwise an error is returned.
fn get_type_str(gty: &GType) -> Result<SwiftTypeContext> {
    // Match `Uint8T` primitive.
    if let GType::Mutable(GTypeCategory::Scalar(GPrimitive::UInt8T)) = gty {
        return Ok(SwiftTypeContext {
            name: "int".to_string(),
            ty: "UInt8".to_string(),
            wrap_as: None,
            deter_as: None,
        });
    }

    if let GType::Mutable(GTypeCategory::Pointer(boxed)) = gty {
        if let GTypeCategory::Unrecognized(ref keyword) = **boxed {
            if keyword.0 == "TWString" {
                return Ok(SwiftTypeContext {
                    name: "string".to_string(),
                    ty: "String".to_string(),
                    wrap_as: Some("TWStringCreateWithNSString(string)".to_string()),
                    deter_as: Some("StringDelete(string)".to_string()),
                });
            } else if keyword.0 == "TWData" {
                return Ok(SwiftTypeContext {
                    name: "data".to_string(),
                    ty: "Data".to_string(),
                    wrap_as: Some("TWDataCreateWithNSData(data)".to_string()),
                    deter_as: Some("TWDataDelete(data)".to_string()),
                });
            }
        }
    }

    Err(Error::Todo)
}

pub fn from_grammar(decl: GFunctionDecl) -> Result<MethodInfo> {
    let c_ffi_name = decl.name.0;
    // TODO:
    let method_name = c_ffi_name.clone();
    let is_static = decl
        .markers
        .0
        .iter()
        .any(|marker| matches!(marker, GMarker::TwExportMethod));

    // ### Method parameters

    let mut params = vec![];
    for param in decl.params {
        // Convert grammar type to (native) Swift type.
        let ctx = get_type_str(&param.ty)?;

        let is_nullable = param.markers.0.iter().any(|marker| {
            // TODO: Why do both of those markers even exist? One would be
            // sufficient (?)
            !matches!(marker, GMarker::NonNull) || matches!(marker, GMarker::Nullable)
        });

        params.push(ParamInfo {
            name: ctx.name,
            param_type: ctx.ty,
            is_nullable,
            wrap_as: ctx.wrap_as,
            deter_as: ctx.deter_as,
        })
    }

    // ### Return value

    // Check marker on whether the return value is nullable.
    let is_nullable =
        decl.return_value.markers.0.iter().any(|marker| {
            !matches!(marker, GMarker::NonNull) || matches!(marker, GMarker::Nullable)
        });

    // Convert grammar type to (native) Swift type.
    let ctx = get_type_str(&decl.return_value.ty)?;

    let info = MethodInfo {
        method_name,
        is_static,
        return_info: ReturnInfo {
            return_type: ctx.ty,
            is_nullable,
            wrap_as: ctx.wrap_as,
        },
        params,
        c_ffi_name,
    };

    Ok(info)
}
