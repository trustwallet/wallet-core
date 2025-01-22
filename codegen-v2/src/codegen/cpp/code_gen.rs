use serde::{Deserialize, Serialize};
use std::fs;
use std::io::Write;

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
    let class_name = format!("{}", info.class);
    let class_dec = format!("TW_EXPORT_CLASS\nstruct {};\n", class_name.clone());
    writeln!(file, "{}", class_dec)?;
    Ok(())
}

fn generate_function_signature(
    class_name: &str,
    func: &TWStaticFunction,
    is_declaration: bool,
) -> String {
    let return_type = convert_rust_type_to_cpp(&func.return_type);
    let mut signature = format!(
        "{}{} {}{}",
        if is_declaration {
            "TW_EXPORT_STATIC_METHOD "
        } else {
            ""
        },
        return_type,
        class_name,
        &func.name
    );
    signature += "(";
    for (i, arg) in func.args.iter().enumerate() {
        signature += format!("{} {}", convert_rust_type_to_cpp(&arg.ty), arg.name).as_str();
        if i < func.args.len() - 1 {
            signature += ", ";
        }
    }
    signature += ")";
    signature
}

fn generate_function_declaration(
    file: &mut std::fs::File,
    class_name: &str,
    func: &TWStaticFunction,
) -> Result<()> {
    let mut func_dec = generate_function_signature(class_name, func, true);
    func_dec += ";\n";
    writeln!(file, "{}", func_dec)?;
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
        "{}",
        format!("#include <Generated/TrustWalletCore/{}.h>", info.class)
    )?;
    writeln!(file, "#include \"rust/Wrapper.h\"")?;
    Ok(())
}

fn generate_function_call(args: &Vec<String>) -> String {
    let mut func_call = "(".to_string();
    for (i, arg) in args.iter().enumerate() {
        func_call += format!("{}", arg).as_str();
        if i < args.len() - 1 {
            func_call += ", ";
        }
    }
    func_call += ");\n";
    func_call
}

fn generate_return_string(func: &TWStaticFunction, converted_args: &Vec<String>) -> String {
    let mut return_string = String::new();
    match func.return_type.as_str() {
        "* mut TWString" => {
            return_string += format!(
                "    const Rust::TWStringWrapper result = Rust::{}",
                func.rust_name
            )
            .as_str();
            return_string += generate_function_call(&converted_args).as_str();
            return_string += format!("    if (!result) {{ return nullptr; }}\n").as_str();
            return_string +=
                format!("    return TWStringCreateWithUTF8Bytes(result.c_str());\n").as_str();
        }
        _ => {
            return_string += format!("    return Rust::{}(", func.rust_name).as_str();
            return_string += generate_function_call(&converted_args).as_str();
        }
    }
    return_string
}

fn generate_conversion_code_with_var_name(ty: &str, name: &str) -> (String, String) {
    match ty {
        "TWString *_Nonnull" => {
            let code = format!(
                "    auto& {name}String = *reinterpret_cast<const std::string*>({name});\n",
                name = name
            ) + format!(
                "    const Rust::TWStringWrapper {name}RustStr = {name}String;\n",
                name = name
            )
            .as_str();
            (code, format!("{}RustStr.get()", name))
        }
        _ => ("".to_string(), name.to_string()),
    }
}

fn generate_function_definition(
    file: &mut std::fs::File,
    info: &TWConfig,
    func: &TWStaticFunction,
) -> Result<()> {
    let mut func_def = generate_function_signature(&info.class, func, false);
    func_def += " {\n";
    let mut converted_args = vec![];
    for arg in func.args.iter() {
        let func_type = convert_rust_type_to_cpp(&arg.ty);
        let (conversion_code, converted_arg) =
            generate_conversion_code_with_var_name(&func_type, &arg.name);
        func_def += conversion_code.as_str();
        converted_args.push(converted_arg);
    }
    let return_string = generate_return_string(func, &converted_args);
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
