use crate::Result;
use handlebars::Handlebars;

const ROOT_DIR: &str = "src/codegen/templates/swift/";
const METHOD: &str = "part_method.hbs";

fn file_path(file: &str) -> String {
    format!("{}{}", ROOT_DIR, file)
}

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
    wrap_as: String,
}

#[derive(Serialize, Deserialize)]
pub struct ParamInfo {
    name: String,
    #[serde(rename = "type")]
    param_type: String,
    is_nullable: bool,
    wrap_as: String,
    deter_as: Option<String>,
}

pub fn register_renderer(engine: &mut Handlebars) -> Result<()> {
    engine
        .register_template_file(METHOD, file_path(METHOD))
        .unwrap();
    Ok(())
}
