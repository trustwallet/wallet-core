use regex::Regex;
use serde::{Deserialize, Serialize};
use std::fmt::Write as _;
use std::fs;
use std::io::Write;

use crate::Error::BadFormat;
use crate::Result;

static IN_DIR: &str = "../rust/bindings/";
static HEADER_OUT_DIR: &str = "../include/TrustWalletCore/";
static SOURCE_OUT_DIR: &str = "../src/Generated/";

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

fn convert_rust_type_to_cpp(ty: &str) -> String {
    let trimmed = ty.replace(" ", "");
    if let Some(captures) = Regex::new(r"^Nonnull<(.+)>$")
        .expect("Failed to create regex")
        .captures(&trimmed)
    {
        format!("{} *_Nonnull", &captures[1])
    } else if let Some(captures) = Regex::new(r"^NonnullMut<(.+)>$")
        .expect("Failed to create regex")
        .captures(&trimmed)
    {
        format!("{} *_Nonnull", &captures[1])
    } else if let Some(captures) = Regex::new(r"^Nullable<(.+)>$")
        .expect("Failed to create regex")
        .captures(&trimmed)
    {
        format!("{} *_Nullable", &captures[1])
    } else if let Some(captures) = Regex::new(r"^NullableMut<(.+)>$")
        .expect("Failed to create regex")
        .captures(&trimmed)
    {
        format!("{} *_Nullable", &captures[1])
    } else {
        match ty {
            "u8" => "uint8_t".to_string(),
            "u16" => "uint16_t".to_string(),
            "u32" => "uint32_t".to_string(),
            "u64" => "uint64_t".to_string(),
            "i8" => "int8_t".to_string(),
            "i16" => "int16_t".to_string(),
            "i32" => "int32_t".to_string(),
            "i64" => "int64_t".to_string(),
            _ => ty.to_string(),
        }
    }
}

fn generate_license(file: &mut std::fs::File) -> Result<()> {
    writeln!(file, "// SPDX-License-Identifier: Apache-2.0")?;
    writeln!(file, "//")?;
    writeln!(file, "// Copyright © 2017 Trust Wallet.\n")?;
    Ok(())
}

fn generate_header_guard(file: &mut std::fs::File) -> Result<()> {
    writeln!(file, "#pragma once\n")?;
    Ok(())
}

fn generate_header_includes(file: &mut std::fs::File, info: &TWConfig) -> Result<()> {
    writeln!(file, "#include \"TWBase.h\"")?;

    // Include headers based on argument types
    let mut included_headers = std::collections::HashSet::new();
    for func in &info.static_functions {
        for arg in &func.args {
            if arg.ty.contains("TWString") && included_headers.insert("TWString.h") {
                writeln!(file, "#include \"TWString.h\"")?;
            }
            if arg.ty.contains("TWData") && included_headers.insert("TWData.h") {
                writeln!(file, "#include \"TWData.h\"")?;
            }
            if arg.ty.contains("TWPrivateKey") && included_headers.insert("TWPrivateKey.h") {
                writeln!(file, "#include \"TWPrivateKey.h\"")?;
            }
            // Additional type checks can be added here in the future
        }
    }

    Ok(())
}

fn generate_class_declaration(file: &mut std::fs::File, info: &TWConfig) -> Result<()> {
    writeln!(file, "TW_EXPORT_CLASS\nstruct {};\n", info.class)?;
    Ok(())
}

fn generate_function_signature(
    class_name: &str,
    func: &TWStaticFunction,
    is_declaration: bool,
) -> Result<String> {
    let return_type = convert_rust_type_to_cpp(&func.return_type);
    let whether_export = if is_declaration {
        "TW_EXPORT_STATIC_METHOD "
    } else {
        ""
    };
    let mut signature = format!("{whether_export}{return_type} {class_name}{}", func.name);
    signature += "(";
    for (i, arg) in func.args.iter().enumerate() {
        write!(
            &mut signature,
            "{} {}",
            convert_rust_type_to_cpp(&arg.ty),
            arg.name
        )
        .map_err(|e| BadFormat(e.to_string()))?;
        if i < func.args.len() - 1 {
            signature += ", ";
        }
    }
    signature += ")";
    Ok(signature)
}

fn generate_function_declaration(
    file: &mut std::fs::File,
    class_name: &str,
    func: &TWStaticFunction,
) -> Result<()> {
    let func_dec = generate_function_signature(class_name, func, true)?;
    for doc in &func.docs {
        writeln!(file, "/// {}", doc)?;
    }
    writeln!(file, "{func_dec};\n")?;
    Ok(())
}

pub fn generate_header(info: &TWConfig) -> Result<()> {
    let file_path = format!("{HEADER_OUT_DIR}/{}.h", info.class);
    let mut file = std::fs::File::create(&file_path)?;

    generate_license(&mut file)?;
    generate_header_guard(&mut file)?;
    generate_header_includes(&mut file, info)?;

    writeln!(file, "\nTW_EXTERN_C_BEGIN\n")?;

    generate_class_declaration(&mut file, info)?;
    for func in &info.static_functions {
        generate_function_declaration(&mut file, &info.class, func)?;
    }

    writeln!(file, "TW_EXTERN_C_END")?;

    file.flush()?;

    Ok(())
}

fn generate_source_includes(file: &mut std::fs::File, info: &TWConfig) -> Result<()> {
    writeln!(
        file,
        "#include <TrustWalletCore/{}.h>",
        info.class
    )?;
    writeln!(file, "#include \"rust/Wrapper.h\"")?;

    // Include headers based on argument types
    let mut included_headers = std::collections::HashSet::new();
    for func in &info.static_functions {
        for arg in &func.args {
            if arg.ty.contains("TWPrivateKey") && included_headers.insert("TWPrivateKey.h") {
                writeln!(file, "#include \"../PrivateKey.h\"")?;
            }
            // Additional type checks can be added here in the future
        }
    }
    
    Ok(())
}

fn generate_function_call(args: &Vec<String>) -> Result<String> {
    let mut func_call = "(".to_string();
    for (i, arg) in args.iter().enumerate() {
        write!(&mut func_call, "{arg}").map_err(|e| BadFormat(e.to_string()))?;
        if i < args.len() - 1 {
            func_call += ", ";
        }
    }
    func_call += ");";
    Ok(func_call)
}

fn generate_return_type(func: &TWStaticFunction, converted_args: &Vec<String>) -> Result<String> {
    let mut return_string = String::new();
    match func.return_type.replace(" ", "").as_str() {
        "NullableMut<TWString>" | "Nullable<TWString>" => {
            write!(
                &mut return_string,
                "\tconst Rust::TWStringWrapper result = Rust::{}{}\n\
                \tif (!result) {{ return nullptr; }}\n\
                \treturn TWStringCreateWithUTF8Bytes(result.c_str());\n",
                func.rust_name,
                generate_function_call(&converted_args)?.as_str()
            )
            .map_err(|e| BadFormat(e.to_string()))?;
        }
        "NullableMut<TWData>" | "Nullable<TWData>" => {
            write!(
                &mut return_string,
                "\tconst Rust::TWDataWrapper result = Rust::{}{}\n\
                \tif (!result.ptr) {{ return nullptr; }}\n\
                \tconst auto resultData = result.toDataOrDefault();\n\
                \treturn TWDataCreateWithBytes(resultData.data(), resultData.size());\n",
                func.rust_name,
                generate_function_call(&converted_args)?.as_str()
            )
            .map_err(|e| BadFormat(e.to_string()))?;
        }
        "NullableMut<TWPrivateKey>" | "Nullable<TWPrivateKey>" => {
            write!(
                &mut return_string,
                "\tconst auto result = Rust::{}{}\n\
                \tif (!result) {{ return nullptr; }}\n\
                \tauto resultData = Rust::tw_private_key_bytes(result);\n\
                \tauto resultSize = Rust::tw_private_key_size(result);\n\
                \tData out(resultData, resultData + resultSize);\n\
                \treturn new TWPrivateKey {{ PrivateKey(out) }};\n",
                func.rust_name,
                generate_function_call(&converted_args)?.as_str()
            )
            .map_err(|e| BadFormat(e.to_string()))?;
        }
        _ => {
            write!(
                &mut return_string,
                "\treturn Rust::{}{}\n",
                func.rust_name,
                generate_function_call(&converted_args)?.as_str()
            )
            .map_err(|e| BadFormat(e.to_string()))?;
        }
    }
    Ok(return_string)
}

fn generate_conversion_code_with_var_name(ty: &str, name: &str) -> Result<(String, String)> {
    match ty {
        "TWString *_Nonnull" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tauto& {name}String = *reinterpret_cast<const std::string*>({name});\n\
                \tconst Rust::TWStringWrapper {name}RustStr = {name}String;"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}RustStr.get()", name)))
        }
        "TWString *_Nullable" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tconst TW::Rust::TWString* {name}Ptr;\n\
                \tif ({name} != nullptr) {{\n\
                    \t\tauto& {name}String = *reinterpret_cast<const std::string*>({name});\n\
                    \t\tconst Rust::TWStringWrapper {name}RustStr = {name}String;\n\
                    \t\t{name}Ptr = {name}RustStr.get();\n\
                \t}} else {{\n\
                    \t\t{name}Ptr = nullptr;\n\
                \t}}"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}Ptr", name)))
        }
        "TWData *_Nonnull" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tauto& {name}Data = *reinterpret_cast<const TW::Data*>({name});\n\
                \tconst Rust::TWDataWrapper {name}RustData = {name}Data;"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}RustData.get()", name)))
        }
        "TWData *_Nullable" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tconst TW::Rust::TWData* {name}Ptr;\n\
                \tif ({name} != nullptr) {{\n\
                    \t\tauto& {name}Data = *reinterpret_cast<const TW::Data*>({name});\n\
                    \t\tconst Rust::TWDataWrapper {name}RustData = {name}Data;\n\
                    \t\t{name}Ptr = {name}RustData.get();\n\
                \t}} else {{\n\
                    \t\t{name}Ptr = nullptr;\n\
                \t}}"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}Ptr", name)))
        }
        "TWPrivateKey *_Nonnull" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tauto* {name}RustRaw = Rust::tw_private_key_create_with_data({name}->impl.bytes.data(), {name}->impl.bytes.size());\n\
                \tconst auto {name}RustPrivateKey = std::shared_ptr<Rust::TWPrivateKey>({name}RustRaw, Rust::tw_private_key_delete);"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}RustPrivateKey.get()", name)))
        }
        "TWPrivateKey *_Nullable" => {
            let mut conversion_code = String::new();
            writeln!(
                &mut conversion_code,
                "\tconst TW::Rust::TWPrivateKey* {name}Ptr;\n\
                \tif ({name} != nullptr) {{\n\
                    \t\tconst auto {name}PrivateKey = *{name};\n\
                    \t\tauto& {name}Data = {name}PrivateKey.impl.bytes;\n\
                    \t\tauto* {name}RustRaw = Rust::tw_private_key_create_with_data({name}Data.data(), {name}Data.size());\n\
                    \t\tconst auto {name}RustPrivateKey = std::shared_ptr<Rust::TWPrivateKey>({name}RustRaw, Rust::tw_private_key_delete);\n\
                    \t\t{name}Ptr = {name}RustPrivateKey.get();\n\
                \t}} else {{\n\
                    \t\t{name}Ptr = nullptr;\n\
                \t}}"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}Ptr", name)))
        }
        _ => Ok(("".to_string(), name.to_string())),
    }
}

fn generate_function_definition(
    file: &mut std::fs::File,
    info: &TWConfig,
    func: &TWStaticFunction,
) -> Result<()> {
    let mut func_def = generate_function_signature(&info.class, func, false)?;
    func_def += " {\n";
    let mut converted_args = vec![];
    for arg in func.args.iter() {
        let func_type = convert_rust_type_to_cpp(&arg.ty);
        let (conversion_code, converted_arg) =
            generate_conversion_code_with_var_name(&func_type, &arg.name)?;
        func_def += conversion_code.as_str();
        converted_args.push(converted_arg);
    }
    let return_string = generate_return_type(func, &converted_args)?;
    func_def += return_string.as_str();
    func_def += "}\n";
    writeln!(file, "{}", func_def)?;
    Ok(())
}

fn generate_source(info: &TWConfig) -> Result<()> {
    let file_path = format!("{SOURCE_OUT_DIR}/{}.cpp", info.class);
    let mut file = std::fs::File::create(&file_path)?;

    generate_license(&mut file)?;
    generate_source_includes(&mut file, info)?;

    writeln!(file, "\nusing namespace TW;\n")?;

    for func in &info.static_functions {
        generate_function_definition(&mut file, info, func)?;
    }

    file.flush()?;

    Ok(())
}

pub fn generate_cpp_bindings() -> Result<()> {
    std::fs::create_dir_all(HEADER_OUT_DIR)?;
    std::fs::create_dir_all(SOURCE_OUT_DIR)?;

    let entries = fs::read_dir(IN_DIR)?;
    for entry in entries {
        let file_path = entry?.path();
        if file_path.is_dir() {
            println!("Found unexpected directory: {}", file_path.display());
            continue;
        }

        let file_contents = fs::read_to_string(&file_path)?;
        let info: TWConfig =
            serde_yaml::from_str(&file_contents).expect("Failed to parse YAML file");

        generate_header(&info)?;
        generate_source(&info)?;
    }
    Ok(())
}
