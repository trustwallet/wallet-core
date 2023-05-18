use super::KotlinType;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainEnum {
    #[serde(rename = "enum_name")]
    pub name: String,
    pub value_type: KotlinType,
    pub add_string_value: bool,
    pub variants: Vec<AndroidMainEnumVariant>,
    pub methods: Vec<AndroidMainMethod>,
    pub properties: Vec<AndroidMainProperty>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainEnumVariant {
    pub name: String,
    pub value: String,
    pub as_string: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainStruct {
    #[serde(rename = "struct_name")]
    pub name: String,
    pub is_class: bool,
    pub inits: Vec<AndroidMainInit>,
    pub methods: Vec<AndroidMainMethod>,
    pub properties: Vec<AndroidMainProperty>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainInit {
    pub name: String,
    pub params: Vec<AndroidMainParam>,
    pub is_nullable: bool,
    pub return_call: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainMethod {
    pub name: String,
    pub params: Vec<AndroidMainParam>,
    pub is_static: bool,
    #[serde(rename = "return")]
    pub return_ty: AndroidMainReturn,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainProperty {
    pub name: String,
    #[serde(rename = "return")]
    pub return_ty: AndroidMainReturn,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainParam {
    pub name: String,
    #[serde(rename = "type")]
    pub ty: KotlinType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AndroidMainReturn {
    #[serde(rename = "type")]
    pub ty: KotlinType,
    pub is_nullable: bool,
}
