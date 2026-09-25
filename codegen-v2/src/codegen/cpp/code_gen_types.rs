use regex::Regex;
use serde::{Deserialize, Serialize};

pub enum TWFunctionType {
    StaticFunction,
    Constructor,
    Destructor,
    Method,
    Property,
}

#[derive(Deserialize, Serialize, Debug, Default)]
pub struct TWConfig {
    pub class: String,
    pub static_functions: Vec<TWFunction>,
    pub constructors: Option<Vec<TWFunction>>,
    pub destructor: Option<TWFunction>,
    pub methods: Option<Vec<TWFunction>>,
    pub properties: Option<Vec<TWFunction>>,
}

impl TWConfig {
    pub fn functions(&self, include_destructor: bool) -> Vec<(TWFunctionType, &TWFunction)> {
        let mut functions = self
            .static_functions
            .iter()
            .map(|f| (TWFunctionType::StaticFunction, f))
            .collect::<Vec<_>>();
        if let Some(constructors) = &self.constructors {
            functions.extend(
                constructors
                    .iter()
                    .map(|f| (TWFunctionType::Constructor, f)),
            );
        }
        if let Some(methods) = &self.methods {
            functions.extend(methods.iter().map(|f| (TWFunctionType::Method, f)));
        }
        if let Some(properties) = &self.properties {
            functions.extend(properties.iter().map(|f| (TWFunctionType::Property, f)));
        }
        if include_destructor {
            if let Some(destructor) = &self.destructor {
                functions.push((TWFunctionType::Destructor, destructor));
            }
        }
        functions
    }

    pub fn is_wrapped(&self) -> bool {
        self.constructors.is_some() && self.destructor.is_some()
    }
}

#[derive(Deserialize, Serialize, Debug)]
pub struct TWFunction {
    pub name: String,
    pub rust_name: String,
    pub args: Vec<TWArg>,
    pub return_type: String,
    pub docs: Vec<String>,
}

impl TWFunction {
    pub fn types(&self) -> Vec<String> {
        self.args
            .iter()
            .map(|arg| arg.ty.clone())
            .chain(std::iter::once(self.return_type.clone()))
            .collect()
    }
}

#[derive(Deserialize, Serialize, Debug, Clone)]
pub struct TWArg {
    pub name: String,
    pub ty: String,
}

#[derive(Debug)]
pub enum TWPointerType {
    Nonnull,
    NonnullMut,
    Nullable,
    NullableMut,
}

#[derive(Debug)]
pub enum TWType {
    Pointer(TWPointerType, String),
    Standard(String),
}

impl From<String> for TWType {
    fn from(ty: String) -> Self {
        let trimmed = ty.replace(" ", "");
        if let Some(captures) = Regex::new(r"^Nonnull<(.+)>$")
            .expect("Failed to create regex")
            .captures(&trimmed)
        {
            TWType::Pointer(TWPointerType::Nonnull, captures[1].to_string())
        } else if let Some(captures) = Regex::new(r"^NonnullMut<(.+)>$")
            .expect("Failed to create regex")
            .captures(&trimmed)
        {
            TWType::Pointer(TWPointerType::NonnullMut, captures[1].to_string())
        } else if let Some(captures) = Regex::new(r"^Nullable<(.+)>$")
            .expect("Failed to create regex")
            .captures(&trimmed)
        {
            TWType::Pointer(TWPointerType::Nullable, captures[1].to_string())
        } else if let Some(captures) = Regex::new(r"^NullableMut<(.+)>$")
            .expect("Failed to create regex")
            .captures(&trimmed)
        {
            TWType::Pointer(TWPointerType::NullableMut, captures[1].to_string())
        } else {
            TWType::Standard(trimmed)
        }
    }
}

impl TWType {
    pub fn cpp_type(&self) -> String {
        match self {
            TWType::Standard(ty) => match ty.as_str() {
                "u8" => "uint8_t".to_string(),
                "u16" => "uint16_t".to_string(),
                "u32" => "uint32_t".to_string(),
                "u64" => "uint64_t".to_string(),
                "i8" => "int8_t".to_string(),
                "i16" => "int16_t".to_string(),
                "i32" => "int32_t".to_string(),
                "i64" => "int64_t".to_string(),
                "TWFFICoinType" => "enum TWCoinType".to_string(),
                _ => ty.to_string(),
            },
            TWType::Pointer(pointer_type, ty) => {
                let ty = match ty.as_str() {
                    "TWString" | "TWData" => ty.to_string(),
                    _ => format!("struct {}", ty),
                };
                match pointer_type {
                    TWPointerType::Nonnull => format!("{} *_Nonnull", ty),
                    TWPointerType::NonnullMut => format!("{} *_Nonnull", ty),
                    TWPointerType::Nullable => format!("{} *_Nullable", ty),
                    TWPointerType::NullableMut => format!("{} *_Nullable", ty),
                }
            }
        }
    }
}
