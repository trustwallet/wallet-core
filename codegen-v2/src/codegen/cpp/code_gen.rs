use heck::ToLowerCamelCase;
use std::fmt::Write as _;
use std::fs;
use std::io::Write;

use super::code_gen_types::*;
use crate::Error::BadFormat;
use crate::Result;

static IN_DIR: &str = "../rust/bindings/";
static HEADER_OUT_DIR: &str = "../include/TrustWalletCore/";
static SOURCE_OUT_DIR: &str = "../src/Generated/";

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
    for (_, func) in info.functions(true) {
        for ty in func.types() {
            let tw_type = TWType::from(ty);
            match tw_type {
                TWType::Pointer(_, header_name) => {
                    if header_name == info.class {
                        continue;
                    }
                    if included_headers.insert(header_name.clone()) {
                        writeln!(file, "#include \"{}.h\"", header_name)?;
                    }
                }
                TWType::Standard(ty) => {
                    if ty.contains("TWFFICoinType")
                        && included_headers.insert("TWCoinType.h".to_string())
                    {
                        // Need to handle this case separately because it's not a pointer type
                        writeln!(file, "#include \"TWCoinType.h\"")?;
                    }
                }
            }
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
    func_type: TWFunctionType,
    func: &TWFunction,
    is_declaration: bool,
) -> Result<String> {
    let return_type = TWType::from(func.return_type.clone()).cpp_type();
    let whether_export = if is_declaration {
        match func_type {
            TWFunctionType::StaticFunction | TWFunctionType::Constructor => {
                "TW_EXPORT_STATIC_METHOD "
            }
            TWFunctionType::Method | TWFunctionType::Destructor => "TW_EXPORT_METHOD ",
            TWFunctionType::Property => "TW_EXPORT_PROPERTY ",
        }
    } else {
        ""
    };
    let mut signature = format!("{whether_export}{return_type} {class_name}{}", func.name);
    signature += "(";
    for (i, arg) in func.args.iter().enumerate() {
        write!(
            &mut signature,
            "{} {}",
            TWType::from(arg.ty.clone()).cpp_type(),
            arg.name.to_lower_camel_case()
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
    func_type: TWFunctionType,
    func: &TWFunction,
) -> Result<()> {
    let func_dec = generate_function_signature(class_name, func_type, func, true)?;
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
    for (func_type, func) in info.functions(true) {
        generate_function_declaration(&mut file, &info.class, func_type, func)?;
    }

    writeln!(file, "TW_EXTERN_C_END")?;

    file.flush()?;

    Ok(())
}

fn generate_wrapper_header(info: &TWConfig) -> Result<()> {
    let class_name = &info.class;
    let wrapper_class_name = class_name.replace("TW", "");
    let file_path = format!("{SOURCE_OUT_DIR}/{}.h", wrapper_class_name);
    let mut file = std::fs::File::create(&file_path)?;

    generate_license(&mut file)?;
    generate_header_guard(&mut file)?;

    writeln!(file, "#include \"rust/Wrapper.h\"\n")?;

    writeln!(
        file,
        "using {wrapper_class_name}Ptr = std::shared_ptr<TW::Rust::{class_name}>;\n",
    )?;

    writeln!(file, "struct {} {{", wrapper_class_name)?;

    let Some(destructor) = &info.destructor else {
        panic!("No destructor found for {}", wrapper_class_name);
    };
    let destructor_name = &destructor.rust_name;
    writeln!(
        file,
        "\texplicit {wrapper_class_name}(TW::Rust::{class_name}* raw_ptr): ptr(raw_ptr, TW::Rust::{destructor_name}) {{}}\n",
    )?;

    writeln!(file, "\t{wrapper_class_name}Ptr ptr;")?;
    writeln!(file, "}};\n")?;

    writeln!(file, "struct {} {{", class_name)?;
    writeln!(file, "\t{wrapper_class_name} impl;")?;
    writeln!(file, "}};\n")?;

    Ok(())
}

fn generate_source_includes(file: &mut std::fs::File, info: &TWConfig) -> Result<()> {
    writeln!(file, "#include <TrustWalletCore/{}.h>", info.class)?;
    writeln!(file, "#include \"rust/Wrapper.h\"")?;

    // Include headers based on argument types
    let mut included_headers = std::collections::HashSet::new();
    for (_, func) in info.functions(true) {
        for ty in func.types() {
            let tw_type = TWType::from(ty);
            let TWType::Pointer(_, header_name) = tw_type else {
                continue;
            };
            if header_name.contains("TWPrivateKey")
                && included_headers.insert("TWPrivateKey.h".to_string())
            {
                writeln!(file, "#include \"../PrivateKey.h\"")?;
            } else if header_name.contains("TWPublicKey")
                && included_headers.insert("TWPublicKey.h".to_string())
            {
                writeln!(file, "#include \"../PublicKey.h\"")?;
            } else if header_name.contains("TWDataVector")
                && included_headers.insert("TWDataVector.h".to_string())
            {
                writeln!(file, "#include \"../DataVector.h\"")?;
            } else if !header_name.contains("TWString") && !header_name.contains("TWData") {
                // Do not need wrapper headers for these types
                let wrapper_header_name = header_name.replace("TW", "");
                if included_headers.insert(wrapper_header_name.clone()) {
                    writeln!(file, "#include \"{}.h\"", wrapper_header_name)?;
                }
            }
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

fn generate_return_type(func: &TWFunction, converted_args: &Vec<String>) -> Result<String> {
    let mut return_string = String::new();
    let tw_type = TWType::from(func.return_type.replace(" ", "").to_string());
    match tw_type {
        TWType::Pointer(pointer_type, ty) => match (pointer_type, ty.as_str()) {
            (TWPointerType::Nullable, "TWString") | (TWPointerType::NullableMut, "TWString") => {
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
            (TWPointerType::NonnullMut, "TWString") | (TWPointerType::Nonnull, "TWString") => {
                write!(
                    &mut return_string,
                    "\tconst Rust::TWStringWrapper result = Rust::{}{}\n\
                    \tconst auto resultString = result.toStringOrDefault();\n\
                    \treturn TWStringCreateWithUTF8Bytes(resultString.c_str());\n",
                    func.rust_name,
                    generate_function_call(&converted_args)?.as_str()
                )
                .map_err(|e| BadFormat(e.to_string()))?;
            }
            (TWPointerType::NullableMut, "TWData") | (TWPointerType::Nullable, "TWData") => {
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
            (TWPointerType::NonnullMut, "TWData") | (TWPointerType::Nonnull, "TWData") => {
                write!(
                    &mut return_string,
                    "\tconst Rust::TWDataWrapper result = Rust::{}{}\n\
                    \tconst auto resultData = result.toDataOrDefault();\n\
                    \treturn TWDataCreateWithBytes(resultData.data(), resultData.size());\n",
                    func.rust_name,
                    generate_function_call(&converted_args)?.as_str()
                )
                .map_err(|e| BadFormat(e.to_string()))?;
            }
            (TWPointerType::NullableMut, "TWPrivateKey")
            | (TWPointerType::Nullable, "TWPrivateKey") => {
                write!(
                    &mut return_string,
                    "\tconst auto result = Rust::{}{}\n\
                    \tconst auto resultRustPrivateKey = Rust::wrapTWPrivateKey(result);\n\
                    \tif (!resultRustPrivateKey.get()) {{ return nullptr; }}\n\
                    \tconst auto resultData = Rust::tw_private_key_bytes(resultRustPrivateKey.get());\n\
                    \tconst auto resultSize = Rust::tw_private_key_size(resultRustPrivateKey.get());\n\
                    \tconst Data out(resultData, resultData + resultSize);\n\
                    \treturn new TWPrivateKey {{ PrivateKey(out) }};\n",
                    func.rust_name,
                    generate_function_call(&converted_args)?.as_str()
                )
                .map_err(|e| BadFormat(e.to_string()))?;
            }
            (TWPointerType::NonnullMut, "TWPrivateKey")
            | (TWPointerType::Nonnull, "TWPrivateKey") => {
                panic!("Nonnull TWPrivateKey is not supported");
            }
            (TWPointerType::NullableMut, "TWPublicKey")
            | (TWPointerType::Nullable, "TWPublicKey") => {
                write!(
                    &mut return_string,
                    "\tconst auto result = Rust::{}{}\n\
                    \tconst auto resultRustPublicKey = Rust::wrapTWPublicKey(result);\n\
                    \tif (!resultRustPublicKey.get()) {{ return nullptr; }}\n\
                    \tconst auto resultData = Rust::tw_public_key_data(resultRustPublicKey.get());\n\
                    \tconst Data out(resultData.data, resultData.data + resultData.size);\n\
                    \treturn new TWPublicKey {{ PublicKey(out, a->impl.type) }};\n",
                    func.rust_name,
                    generate_function_call(&converted_args)?.as_str()
                )
                .map_err(|e| BadFormat(e.to_string()))?;
            }
            (TWPointerType::NonnullMut, "TWPublicKey")
            | (TWPointerType::Nonnull, "TWPublicKey") => {
                panic!("Nonnull TWPublicKey is not supported");
            }
            (pointer_type, class_name) => {
                let wrapper_class_name = class_name.replace("TW", "");
                let null_return = match pointer_type {
                    TWPointerType::Nullable | TWPointerType::NullableMut => {
                        "if (!resultRaw) {{ return nullptr; }}\n"
                    }
                    _ => "",
                };
                write!(
                    &mut return_string,
                    "\tauto* resultRaw = Rust::{}{}\n\
                    {null_return}\
                    \tconst {wrapper_class_name} resultWrapped(resultRaw);\n\
                    \treturn new {class_name} {{ resultWrapped }};\n",
                    func.rust_name,
                    generate_function_call(&converted_args)?.as_str()
                )
                .map_err(|e| BadFormat(e.to_string()))?;
            }
        },
        TWType::Standard(ty) => match ty.as_str() {
            "void" => {
                write!(
                    &mut return_string,
                    "\tRust::{}{}\n",
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
        },
    }
    Ok(return_string)
}

fn generate_conversion_code_with_var_name(tw_type: TWType, name: &str) -> Result<(String, String)> {
    match tw_type {
        TWType::Pointer(ref pointer_type, ref ty) => match (pointer_type, ty.as_str()) {
            (TWPointerType::Nonnull, "TWString") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tauto& {name}String = *reinterpret_cast<const std::string*>({name});\n\
                    \tconst Rust::TWStringWrapper {name}RustStr = {name}String;"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustStr.get()", name)))
            }
            (TWPointerType::Nullable, "TWString") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tRust::TWStringWrapper {name}RustStr;\n\
                    \tif ({name} != nullptr) {{\n\
                    \t\t{name}RustStr = *reinterpret_cast<const std::string*>({name});\n\
                    \t}}"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustStr.get()", name)))
            }
            (TWPointerType::Nonnull, "TWData") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tauto& {name}Data = *reinterpret_cast<const TW::Data*>({name});\n\
                    \tconst Rust::TWDataWrapper {name}RustData = {name}Data;"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustData.get()", name)))
            }
            (TWPointerType::Nullable, "TWData") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tRust::TWDataWrapper {name}RustData;\n\
                    \tif ({name} != nullptr) {{\n\
                        \t\t{name}RustData = *reinterpret_cast<const TW::Data*>({name});\n\
                    \t}}"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustData.get()", name)))
            }
            (TWPointerType::Nonnull, "TWPrivateKey") | (TWPointerType::NonnullMut, "TWPrivateKey") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tauto &{name}PrivateKey = *reinterpret_cast<const TW::PrivateKey*>({name});\n\
                    \tauto* {name}RustRaw = Rust::tw_private_key_create_with_data({name}PrivateKey.bytes.data(), {name}PrivateKey.bytes.size());\n\
                    \tconst auto {name}RustPrivateKey = Rust::wrapTWPrivateKey({name}RustRaw);"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustPrivateKey.get()", name)))
            }
            (TWPointerType::Nullable, "TWPrivateKey") | (TWPointerType::NullableMut, "TWPrivateKey") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tstd::shared_ptr<TW::Rust::TWPrivateKey> {name}RustPrivateKey;\n\
                    \tif ({name} != nullptr) {{\n\
                    \t\tconst auto& {name}PrivateKey = {name};\n\
                    \t\tauto* {name}RustRaw = Rust::tw_private_key_create_with_data({name}PrivateKey->impl.bytes.data(), {name}PrivateKey->impl.bytes.size());\n\
                    \t\t{name}RustPrivateKey = Rust::wrapTWPrivateKey({name}RustRaw);\n\
                    \t}}"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustPrivateKey.get()", name)))
            }
            (TWPointerType::Nonnull, "TWPublicKey") | (TWPointerType::NonnullMut, "TWPublicKey") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tauto &{name}PublicKey = *reinterpret_cast<const TW::PublicKey*>({name});\n\
                    \tconst auto {name}PublicKeyType = static_cast<uint32_t>({name}PublicKey.type);\n\
                    \tauto* {name}RustRaw = Rust::tw_public_key_create_with_data({name}PublicKey.bytes.data(), {name}PublicKey.bytes.size(), {name}PublicKeyType);\n\
                    \tconst auto {name}RustPublicKey = Rust::wrapTWPublicKey({name}RustRaw);"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustPublicKey.get()", name)))
            }
            (TWPointerType::Nullable, "TWPublicKey") | (TWPointerType::NullableMut, "TWPublicKey") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tstd::shared_ptr<TW::Rust::TWPublicKey> {name}RustPublicKey;\n\
                    \tif ({name} != nullptr) {{\n\
                    \t\tconst auto& {name}PublicKey = {name};\n\
                    \t\tconst auto {name}PublicKeyType = static_cast<uint32_t>({name}PublicKey->impl.type);\n\
                    \t\tauto* {name}RustRaw = Rust::tw_public_key_create_with_data({name}PublicKey->impl.bytes.data(), {name}PublicKey->impl.bytes.size(), {name}PublicKeyType);\n\
                    \t\t{name}RustPublicKey = Rust::wrapTWPublicKey({name}RustRaw);\n\
                    \t}}"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustPublicKey.get()", name)))
            }
            (TWPointerType::Nonnull, "TWDataVector") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tconst Rust::TWDataVectorWrapper {name}RustDataVector = createFromTWDataVector({name});"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustDataVector.get()", name)))
            }
            (TWPointerType::Nullable, "TWDataVector") => {
                let mut conversion_code = String::new();
                writeln!(
                    &mut conversion_code,
                    "\tstd::shared_ptr<TW::Rust::TWDataVector> {name}RustDataVector;\n\
                    \tif ({name} != nullptr) {{\n\
                    \t\t{name}RustDataVector = createFromTWDataVector({name});\n\
                    \t}}"
                )
                .map_err(|e| BadFormat(e.to_string()))?;
                Ok((conversion_code, format!("{}RustDataVector.get()", name)))
            }
            _ if tw_type.cpp_type().starts_with("struct ") => {
                Ok(("".to_string(), format!("{name}->impl.ptr.get()")))
            }
            _ => panic!("Unsupported type: {}", tw_type.cpp_type()),
        },
        TWType::Standard(_) => Ok(("".to_string(), name.to_string())),
    }
}

fn generate_function_definition(
    file: &mut std::fs::File,
    info: &TWConfig,
    func_type: TWFunctionType,
    func: &TWFunction,
) -> Result<()> {
    let mut func_def = generate_function_signature(&info.class, func_type, func, false)?;
    func_def += " {\n";
    let mut converted_args = vec![];
    for arg in func.args.iter() {
        let func_type = TWType::from(arg.ty.clone());
        let (conversion_code, converted_arg) =
            generate_conversion_code_with_var_name(func_type, &arg.name.to_lower_camel_case())?;
        func_def += conversion_code.as_str();
        converted_args.push(converted_arg);
    }
    let return_string = generate_return_type(func, &converted_args)?;
    func_def += return_string.as_str();
    func_def += "}\n";
    writeln!(file, "{}", func_def)?;
    Ok(())
}

fn generate_destructor_definition(
    file: &mut std::fs::File,
    info: &TWConfig,
    destructor: &TWFunction,
) -> Result<()> {
    let function_signature =
        generate_function_signature(&info.class, TWFunctionType::Destructor, destructor, false)?;
    assert!(
        destructor.args.len() == 1,
        "Destructor must have exactly one argument"
    );
    let arg_name = &destructor.args[0].name.to_lower_camel_case();
    writeln!(
        file,
        "{function_signature}{{\n\
        \tdelete {arg_name};\n\
        }}"
    )?;
    Ok(())
}

fn generate_source(info: &TWConfig) -> Result<()> {
    let file_path = format!("{SOURCE_OUT_DIR}/{}.cpp", info.class);
    let mut file = std::fs::File::create(&file_path)?;

    generate_license(&mut file)?;
    generate_source_includes(&mut file, info)?;

    writeln!(file, "\nusing namespace TW;\n")?;

    for (func_type, func) in info.functions(false) {
        generate_function_definition(&mut file, info, func_type, func)?;
    }
    if let Some(destructor) = &info.destructor {
        generate_destructor_definition(&mut file, info, destructor)?;
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

        if info.is_wrapped() {
            generate_wrapper_header(&info)?;
        }

        generate_header(&info)?;
        generate_source(&info)?;
    }
    Ok(())
}
