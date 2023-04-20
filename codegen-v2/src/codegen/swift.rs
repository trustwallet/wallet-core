use crate::Result;
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
