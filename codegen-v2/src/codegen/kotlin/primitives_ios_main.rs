use super::KotlinType;

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
	params: Vec<IosMainParam>,
	param_pass: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainMethod {
	name: String,
	params: Vec<IosMainParam>,
	call: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainProperty {
	name: String,
	params: Vec<IosMainParam>,
	call: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct IosMainParam {
	name: String,
    #[serde(rename = "type")]
	ty: KotlinType,
	is_nullable: bool,
}
