use crate::manifest::FileInfo;
use crate::Result;

use super::AndroidMainStruct;

pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub android_main_template: &'a str,
}

#[derive(Default)]
pub struct GeneratedAndroidMainTypes {
    pub structs: Vec<AndroidMainStruct>,
}

pub fn generate_android_main_types(mut info: FileInfo) -> Result<GeneratedAndroidMainTypes> {
    let mut outputs = GeneratedAndroidMainTypes::default();

    for strct in info.structs {}

    todo!()
}
