use super::{AndroidMainEnum, AndroidMainInit, AndroidMainMethod, AndroidMainProperty};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct CommonMainStruct {
    #[serde(rename = "struct_name")]
    pub name: String,
    pub is_class: bool,
    pub header_init: Option<CommonMainInit>,
    pub inits: Vec<CommonMainInit>,
    pub methods: Vec<CommonMainMethod>,
    pub properties: Vec<CommonMainProperty>,
}

// We just alias those types since they'd basically be identical. We can create
// new types if required.
pub type CommonMainInit = AndroidMainInit;
pub type CommonMainMethod = AndroidMainMethod;
pub type CommonMainProperty = AndroidMainProperty;
pub type CommonMainEnum = AndroidMainEnum;
