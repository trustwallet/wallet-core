use std::fs;
use std::io::Write;
use tw_misc::code_gen::TWConfig;

use crate::Result;
fn map_type(ty: &str) -> String {
    match ty.trim() {
        s if s.starts_with("* const") => format!("{} *_Nonnull", &s[8..]),
        s if s.starts_with("* mut") => format!("{} *_Nullable", &s[6..]),
        "bool" => "bool".to_string(),
        _ => ty.to_string(),
    }
}

fn generate_conversion_code(ty: &str, name: &str) -> (String, String) {
    match ty {
        "TWString *_Nonnull" => {
            let code = format!("    auto& {name}String = *reinterpret_cast<const std::string*>({name});\n", name=name) +
            format!("    const Rust::TWStringWrapper {name}RustStr = {name}String;\n", name=name).as_str();
            (code, format!("{}RustStr.get()", name))
        },
        _ => ("".to_string(), name.to_string()),
    }
}

pub fn generate_cpp_bindings() -> Result<()> {
    const IN_DIR: &str = "../rust/bindings/";
    const HEADER_OUT_DIR: &str = "../include/TrustWalletCore/";
    const SOURCE_OUT_DIR: &str = "../src/interface/";
    
    std::fs::create_dir_all(HEADER_OUT_DIR)?;
    std::fs::create_dir_all(SOURCE_OUT_DIR)?;

    let entries = fs::read_dir(IN_DIR)?;

    for entry in entries {
        let entry = entry?;
        let file_path = entry.path();

        if file_path.is_dir() {
            println!("Found unexpected directory: {}", file_path.display());
            continue;
        }

        let file_contents = fs::read_to_string(&file_path)?;

        let info: TWConfig = serde_yaml::from_str(&file_contents).expect("Failed to parse YAML file");

        let file_path = format!("{HEADER_OUT_DIR}/{}Gen.h", info.class);

        let mut file = std::fs::File::create(&file_path)?;
        writeln!(file, "// Copyright © 2017 Trust Wallet.\n")?;
        writeln!(file, "#pragma once\n")?;
        writeln!(file, "#include \"TWBase.h\"")?;
        
        // Include headers based on argument types
        let mut included_headers = std::collections::HashSet::new();
        for func in &info.static_functions {
            for arg in &func.args {
                if arg.ty.contains("TWString") && included_headers.insert("TWString.h") {
                    writeln!(file, "#include \"TWString.h\"")?;
                }
                // Additional type checks can be added here in the future
            }
        }

        writeln!(file, "\nTW_EXTERN_C_BEGIN\n")?;

        let class_name = format!("{}", info.class);
        let class_dec = format!("TW_EXPORT_CLASS\nstruct {};\n", class_name.clone());
        writeln!(file, "{}", class_dec)?;

        for func in &info.static_functions {
            let return_type = map_type(&func.return_type);
            let mut func_dec = format!("TW_EXPORT_STATIC_METHOD\n{} {}(", return_type, class_name.clone() + &func.name);
            for (i, arg) in func.args.iter().enumerate() {
                let func_type = map_type(&arg.ty);
                func_dec += format!("{} {}", func_type, arg.name).as_str();
                if i < func.args.len() - 1 {
                    func_dec += ", ";
                }
            }
            func_dec += ");\n";
            writeln!(file, "{}", func_dec)?;
        }

        writeln!(file, "TW_EXTERN_C_END")?;

        file.flush()?;
        
        let file_path = format!("{SOURCE_OUT_DIR}/{}Gen.cpp", info.class);

        let mut file = std::fs::File::create(&file_path)?;
        writeln!(file, "// Copyright © 2017 Trust Wallet.\n")?;
        writeln!(file, "{}", format!("#include <TrustWalletCore/{}Gen.h>", info.class))?;
        writeln!(file, "#include \"rust/Wrapper.h\"")?;

        writeln!(file, "\nusing namespace TW;\n")?;

        for func in &info.static_functions {
            let return_type = map_type(&func.return_type);
            let mut func_dec = format!("{} {}(", return_type, class_name.clone() + &func.name);
            let mut conversion_code = vec![];
            for (i, arg) in func.args.iter().enumerate() {
                let func_type = map_type(&arg.ty);
                let code_with_name = generate_conversion_code(&func_type, &arg.name);
                conversion_code.push(code_with_name);
                func_dec += format!("{} {}", func_type, arg.name).as_str();
                if i < func.args.len() - 1 {
                    func_dec += ", ";
                }
            }
            func_dec += ") {\n";
            for code in &conversion_code {
                func_dec += code.0.as_str();
            }
            if return_type == "TWString *_Nullable" {
                func_dec += format!("    const Rust::TWStringWrapper result = Rust::{}(", func.rust_name).as_str();
                for (i, arg) in conversion_code.iter().enumerate() {
                    func_dec += format!("{}", arg.1).as_str();
                    if i < conversion_code.len() - 1 {
                        func_dec += ", ";
                    }
                }
                func_dec += ");\n";
                func_dec += format!("    if (!result) return nullptr;\n").as_str();
                func_dec += format!("    return TWStringCreateWithUTF8Bytes(result.c_str());\n").as_str();
            }
            func_dec += "}\n";
            writeln!(file, "{}", func_dec)?;
        }

        file.flush()?;
    }
    
    Ok(())
}
