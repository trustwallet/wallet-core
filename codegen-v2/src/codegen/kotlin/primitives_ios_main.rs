use super::{
    AndroidMainEnum, AndroidMainEnumVariant, AndroidMainMethod, AndroidMainParam,
    AndroidMainProperty, AndroidMainReturn, KotlinType,
};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainStruct {
    #[serde(rename = "struct_name")]
    pub name: String,
    pub is_class: bool,
    pub inits: Vec<IosMainInit>,
    pub methods: Vec<IosMainMethod>,
    pub properties: Vec<IosMainProperty>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainInit {
    pub params: Vec<IosMainParam>,
    pub param_pass: Vec<String>,
    pub c_ffi_name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainMethod {
    pub name: String,
    pub params: Vec<AndroidMainParam>,
    pub is_static: bool,
    pub call: String,
    #[serde(rename = "return")]
    pub return_ty: IosMainReturn,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainProperty {
    pub name: String,
    pub call: String,
    #[serde(rename = "return")]
    pub return_ty: IosMainReturn,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainEnum {
    #[serde(rename = "enum_name")]
    pub name: String,
    pub value_type: KotlinType,
    pub add_string_value: bool,
    pub variants: Vec<AndroidMainEnumVariant>,
    pub methods: Vec<IosMainMethod>,
    pub properties: Vec<IosMainProperty>,
}

// We just alias those types since they'd basically be identical. We can create
// new types if required.
pub type IosMainParam = AndroidMainParam;
pub type IosMainReturn = AndroidMainReturn;
