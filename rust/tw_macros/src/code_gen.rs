use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize, Debug, Default)]
pub struct TWConfig {
    pub class: String,
    pub static_functions: Vec<TWFunction>,
    pub constructors: Option<Vec<TWFunction>>,
    pub destructor: Option<TWFunction>,
    pub methods: Option<Vec<TWFunction>>,
    pub properties: Option<Vec<TWFunction>>,
}

#[derive(Deserialize, Serialize, Debug)]
pub struct TWFunction {
    pub name: String,
    pub rust_name: String,
    pub args: Vec<TWArg>,
    pub return_type: String,
    pub docs: Vec<String>,
}

#[derive(Deserialize, Serialize, Debug)]
pub struct TWArg {
    pub name: String,
    pub ty: String,
}
