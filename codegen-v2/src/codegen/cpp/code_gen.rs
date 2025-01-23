use serde::{Deserialize, Serialize};
use std::fmt::Write as _;
use std::fs;
use std::io::Write;

use crate::Error::BadFormat;
use crate::Result;

static IN_DIR: &str = "../rust/bindings/";
static HEADER_OUT_DIR: &str = "../include/Generated/TrustWalletCore/";
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
}

#[derive(Deserialize, Serialize, Debug)]
pub struct TWArg {
    pub name: String,
    pub ty: String,
}

fn convert_rust_type_to_cpp(ty: &str) -> String {
    match ty.trim() {
        s if s.starts_with("* const") => format!("{} *_Nonnull", &s[8..]),
        s if s.starts_with("* mut") => format!("{} *_Nullable", &s[6..]),
        "bool" => "bool".to_string(),
        _ => ty.to_string(),
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
    writeln!(file, "#include <TrustWalletCore/TWBase.h>")?;

    // Include headers based on argument types
    let mut included_headers = std::collections::HashSet::new();
    for func in &info.static_functions {
        for arg in &func.args {
            if arg.ty.contains("TWString") && included_headers.insert("TWString.h") {
                writeln!(file, "#include <TrustWalletCore/TWString.h>")?;
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
        "#include <Generated/TrustWalletCore/{}.h>",
        info.class
    )?;
    writeln!(file, "#include \"rust/Wrapper.h\"")?;
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
    func_call += ");\n";
    Ok(func_call)
}

fn generate_return_type(func: &TWStaticFunction, converted_args: &Vec<String>) -> Result<String> {
    let mut return_string = String::new();
    match func.return_type.as_str() {
        "* mut TWString" => {
            write!(
                &mut return_string,
                "    const Rust::TWStringWrapper result = Rust::{}",
                func.rust_name
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            return_string += generate_function_call(&converted_args)?.as_str();
            writeln!(&mut return_string, "    if (!result) {{ return nullptr; }}")
                .map_err(|e| BadFormat(e.to_string()))?;
            writeln!(
                &mut return_string,
                "    return TWStringCreateWithUTF8Bytes(result.c_str());"
            )
            .map_err(|e| BadFormat(e.to_string()))?;
        }
        _ => {
            writeln!(&mut return_string, "    return Rust::{}", func.rust_name)
                .map_err(|e| BadFormat(e.to_string()))?;
            return_string += generate_function_call(&converted_args)?.as_str();
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
                "    auto& {name}String = *reinterpret_cast<const std::string*>({name});",
                name = name
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            writeln!(
                &mut conversion_code,
                "    const Rust::TWStringWrapper {name}RustStr = {name}String;",
                name = name
            )
            .map_err(|e| BadFormat(e.to_string()))?;
            Ok((conversion_code, format!("{}RustStr.get()", name)))
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
