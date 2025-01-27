use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize, Debug)]
pub struct TWConfig {
    pub class: String,
    pub static_functions: Vec<TWStaticFunction>,
}

#[derive(Deserialize, Serialize, Debug)]
pub struct TWStaticFunction {
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
