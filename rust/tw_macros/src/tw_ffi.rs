use derive_syn_parse::Parse;
use proc_macro2::TokenStream as TokenStream2;
use quote::quote;
use syn::{
    parse::{Parse, ParseStream},
    parse2, Ident, Result, Token,
};

use std::env;
use std::fmt;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};

use crate::code_gen::{TWArg, TWConfig, TWFunction};
use crate::utils::is_uint;

pub mod keywords {
    use syn::custom_keyword;

    custom_keyword!(ty);
    custom_keyword!(class);
    custom_keyword!(name);
    custom_keyword!(static_function);
    custom_keyword!(constructor);
    custom_keyword!(destructor);
    custom_keyword!(method);
    custom_keyword!(property);
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum TWFFIType {
    StaticFunction,
    Constructor,
    Destructor,
    Method,
    Property,
}

impl Parse for TWFFIType {
    fn parse(input: ParseStream) -> Result<Self> {
        let lookahead = input.lookahead1();
        if lookahead.peek(keywords::static_function) {
            input.parse::<keywords::static_function>()?;
            Ok(Self::StaticFunction)
        } else if lookahead.peek(keywords::constructor) {
            input.parse::<keywords::constructor>()?;
            Ok(Self::Constructor)
        } else if lookahead.peek(keywords::destructor) {
            input.parse::<keywords::destructor>()?;
            Ok(Self::Destructor)
        } else if lookahead.peek(keywords::method) {
            input.parse::<keywords::method>()?;
            Ok(Self::Method)
        } else if lookahead.peek(keywords::property) {
            input.parse::<keywords::property>()?;
            Ok(Self::Property)
        } else {
            Err(lookahead.error())
        }
    }
}

impl fmt::Display for TWFFIType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            TWFFIType::StaticFunction => write!(f, "static_function"),
            TWFFIType::Constructor => write!(f, "constructor"),
            TWFFIType::Destructor => write!(f, "destructor"),
            TWFFIType::Method => write!(f, "method"),
            TWFFIType::Property => write!(f, "property"),
        }
    }
}

#[derive(Parse, Clone)]
pub struct TWFFIAttrArgs {
    pub _ty_keyword: Option<keywords::ty>,
    #[parse_if(_ty_keyword.is_some())]
    pub _eq: Option<Token![=]>,
    #[parse_if(_ty_keyword.is_some())]
    pub ty: Option<TWFFIType>,
    #[parse_if(_ty_keyword.is_some())]
    pub _comma: Option<Token![,]>,

    pub _class_keyword: Option<keywords::class>,
    #[parse_if(_class_keyword.is_some())]
    pub _eq2: Option<Token![=]>,
    #[parse_if(_class_keyword.is_some())]
    pub class: Option<Ident>,
    #[parse_if(_class_keyword.is_some())]
    pub _comma2: Option<Token![,]>,

    pub _name_keyword: Option<keywords::name>,
    #[parse_if(_name_keyword.is_some())]
    pub _eq3: Option<Token![=]>,
    #[parse_if(_name_keyword.is_some())]
    pub name: Option<Ident>,
}

fn update_or_append_function(functions: &mut Option<Vec<TWFunction>>, function: TWFunction) {
    if let Some(funcs) = functions {
        if let Some(idx) = funcs.iter().position(|f| f.name == function.name) {
            funcs[idx] = function;
        } else {
            funcs.push(function);
        }
    } else {
        *functions = Some(vec![function]);
    }
}

/// The subdirectory of the Cargo target directory with the lock and temporary files.
/// They are kept out of `bindings/` because `codegen-v2` parses every file there as a YAML manifest,
/// and `cargo clean` removes them together with the other build artifacts.
const STATE_DIR_NAME: &str = "tw_ffi";
const BINDINGS_LOCK_FILE: &str = "bindings.lock";

/// Returns the directory for the lock and temporary files.
/// `rustc` runs from the workspace root, so the default Cargo target directory is `target`.
fn state_dir(workspace_dir: &Path) -> PathBuf {
    let target_dir = env::var_os("CARGO_TARGET_DIR")
        .map(PathBuf::from)
        .unwrap_or_else(|| workspace_dir.join("target"));
    target_dir.join(STATE_DIR_NAME)
}

/// Moves `from` to `to`. Falls back to copying if the files are on different filesystems,
/// which is possible if `CARGO_TARGET_DIR` points elsewhere.
fn replace_file(from: &Path, to: &Path) -> io::Result<()> {
    fs::rename(from, to).or_else(|_| {
        fs::copy(from, to)?;
        fs::remove_file(from)
    })
}

/// Adds `function` to the `<bindings_dir>/<class>.yaml` manifest.
///
/// The macro is expanded concurrently: `cargo build` compiles a crate for several targets at once
/// (see `tools/rust-bindgen`), and every `rustc` process updates the same manifests.
/// An unsynchronized read-modify-write loses functions (or reads a half-written file), so
/// the update is done under an exclusive file lock and the manifest is replaced atomically.
fn update_bindings(
    bindings_dir: &Path,
    state_dir: &Path,
    class: &str,
    ty: Option<TWFFIType>,
    function: TWFunction,
) -> io::Result<()> {
    fs::create_dir_all(bindings_dir)?;
    fs::create_dir_all(state_dir)?;

    let lock = fs::OpenOptions::new()
        .create(true)
        .truncate(false)
        .write(true)
        .open(state_dir.join(BINDINGS_LOCK_FILE))?;
    // The lock is released when `lock` is dropped, or by the OS if the process dies.
    lock.lock()?;

    let yaml_file_path = bindings_dir.join(format!("{class}.yaml"));
    let new_config = || TWConfig {
        class: class.to_string(),
        ..Default::default()
    };

    let mut config = match fs::read_to_string(&yaml_file_path) {
        Ok(contents) => serde_yaml::from_str(&contents).unwrap_or_else(|_| new_config()),
        Err(_) => new_config(),
    };
    match ty {
        Some(TWFFIType::StaticFunction) => {
            if let Some(idx) = config
                .static_functions
                .iter()
                .position(|f| f.name == function.name)
            {
                config.static_functions[idx] = function;
            } else {
                config.static_functions.push(function);
            }
        },
        Some(TWFFIType::Constructor) => {
            update_or_append_function(&mut config.constructors, function);
        },
        Some(TWFFIType::Destructor) => {
            config.destructor = Some(function);
        },
        Some(TWFFIType::Method) => {
            update_or_append_function(&mut config.methods, function);
        },
        Some(TWFFIType::Property) => {
            update_or_append_function(&mut config.properties, function);
        },
        None => {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "Invalid FFI type",
            ))
        },
    }

    let yaml_output = serde_yaml::to_string(&config)
        .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
    // Write to a temporary file next to the lock file, then move it into place.
    let tmp_file_path = state_dir.join(format!("{class}.yaml.{}.tmp", std::process::id()));
    fs::write(&tmp_file_path, yaml_output)?;
    replace_file(&tmp_file_path, &yaml_file_path)
}

pub fn tw_ffi(attr: TokenStream2, item: TokenStream2) -> Result<TokenStream2> {
    let args = parse2::<TWFFIAttrArgs>(attr)?;

    let func = parse2::<syn::ItemFn>(item.clone())?;
    let func_name = func.sig.ident.to_string();
    let func_args = func
        .sig
        .inputs
        .iter()
        .map(|arg| match arg {
            syn::FnArg::Typed(syn::PatType { pat, ty, .. }) => (quote!( #pat ), quote!( #ty )),
            _ => (quote!(), quote!()),
        })
        .map(|(name, ty)| TWArg {
            name: name.to_string(),
            ty: ty.to_string(),
        })
        .collect::<Vec<TWArg>>();

    let return_type = func.sig.output;
    let return_type = match return_type {
        syn::ReturnType::Type(_, ty) => quote!(#ty).to_string(),
        _ => "void".to_string(),
    };

    let class = args.class.unwrap().to_string();
    // TODO add support for structs.
    if func_args.iter().any(|arg| is_uint(&arg.ty)) {
        return Err(syn::Error::new(
            proc_macro2::Span::call_site(),
            "Unsigned integers are not supported within class methods. Consider using 'struct' or signed integers. See https://kotlinlang.org/docs/inline-classes.html#mangling"
        ));
    }
    let docs = func
        .attrs
        .iter()
        .flat_map(|attr| {
            if let syn::Meta::NameValue(meta) = &attr.meta {
                if meta.path.is_ident("doc") {
                    if let syn::Expr::Lit(syn::ExprLit {
                        lit: syn::Lit::Str(lit),
                        ..
                    }) = &meta.value
                    {
                        return Some(lit.value().trim().to_string());
                    }
                }
            }
            None
        })
        .collect::<Vec<_>>();
    let function = TWFunction {
        name: args.name.unwrap().to_string(),
        rust_name: func_name,
        args: func_args,
        return_type,
        docs,
    };

    let workspace_dir = env::var("CARGO_WORKSPACE_DIR").unwrap_or_default();
    let workspace_dir = Path::new(&workspace_dir);
    let bindings_dir = workspace_dir.join("bindings");
    update_bindings(
        &bindings_dir,
        &state_dir(workspace_dir),
        &class,
        args.ty,
        function,
    )
    .map_err(|e| {
        syn::Error::new(
            proc_macro2::Span::call_site(),
            format!("Failed to update the '{class}' bindings: {e}"),
        )
    })?;

    Ok(item)
}

#[cfg(test)]
mod tests {
    use super::*;
    use proc_macro2::Span;

    #[test]
    fn test_ffi_attr_arg_parsing() {
        let args = parse2::<TWFFIAttrArgs>(quote! {
            ty = static_function,
            class = MyClass,
            name = MyName
        })
        .unwrap();
        assert_eq!(args.class, Some(Ident::new("MyClass", Span::call_site())));
        assert_eq!(args.name, Some(Ident::new("MyName", Span::call_site())));
    }

    fn test_function(name: &str) -> TWFunction {
        TWFunction {
            name: name.to_string(),
            rust_name: format!("tw_race_{}", name.to_lowercase()),
            args: vec![],
            return_type: "void".to_string(),
            docs: vec![],
        }
    }

    fn update(
        dir: &Path,
        class: &str,
        ty: Option<TWFFIType>,
        function: TWFunction,
    ) -> io::Result<()> {
        update_bindings(
            &dir.join("bindings"),
            &dir.join("target").join(STATE_DIR_NAME),
            class,
            ty,
            function,
        )
    }

    fn test_workspace_dir(test: &str) -> std::path::PathBuf {
        let dir = env::temp_dir().join(format!("tw_ffi_{test}_{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(&dir).unwrap();
        dir
    }

    /// Several `rustc` processes expand `#[tw_ffi]` for the same class at the same time.
    /// None of the functions must be lost.
    #[test]
    fn test_concurrent_updates_do_not_lose_functions() {
        const THREADS: usize = 32;

        let dir = test_workspace_dir("race");
        let barrier = std::sync::Arc::new(std::sync::Barrier::new(THREADS));

        let handles = (0..THREADS)
            .map(|i| {
                let dir = dir.clone();
                let barrier = barrier.clone();
                std::thread::spawn(move || {
                    barrier.wait();
                    update(
                        &dir,
                        "Race",
                        Some(TWFFIType::StaticFunction),
                        test_function(&format!("Function{i}")),
                    )
                    .unwrap();
                })
            })
            .collect::<Vec<_>>();
        for handle in handles {
            handle.join().unwrap();
        }

        let yaml = fs::read_to_string(dir.join("bindings/Race.yaml")).unwrap();
        let config: TWConfig = serde_yaml::from_str(&yaml).unwrap();
        let mut names = config
            .static_functions
            .into_iter()
            .map(|f| f.name)
            .collect::<Vec<_>>();
        names.sort();
        let mut expected = (0..THREADS)
            .map(|i| format!("Function{i}"))
            .collect::<Vec<_>>();
        expected.sort();
        assert_eq!(names, expected);

        fs::remove_dir_all(&dir).unwrap();
    }

    /// `codegen-v2` parses every file in `bindings/` as a manifest, so it must contain manifests only.
    #[test]
    fn test_bindings_dir_contains_manifests_only() {
        let dir = test_workspace_dir("only_manifests");
        update(
            &dir,
            "Only",
            Some(TWFFIType::StaticFunction),
            test_function("First"),
        )
        .unwrap();
        update(
            &dir,
            "Only",
            Some(TWFFIType::Method),
            test_function("Second"),
        )
        .unwrap();

        let files = fs::read_dir(dir.join("bindings"))
            .unwrap()
            .map(|e| e.unwrap().file_name().into_string().unwrap())
            .collect::<Vec<_>>();
        assert_eq!(files, vec!["Only.yaml".to_string()]);

        // The lock file is kept in the target directory, and no temporary files are left behind.
        let state_files = fs::read_dir(dir.join("target").join(STATE_DIR_NAME))
            .unwrap()
            .map(|e| e.unwrap().file_name().into_string().unwrap())
            .collect::<Vec<_>>();
        assert_eq!(state_files, vec![BINDINGS_LOCK_FILE.to_string()]);

        fs::remove_dir_all(&dir).unwrap();
    }

    #[test]
    fn test_update_replaces_function_with_the_same_name() {
        let dir = test_workspace_dir("replace");
        for rust_name in ["old", "new"] {
            let mut function = test_function("Same");
            function.rust_name = rust_name.to_string();
            update(&dir, "Replace", Some(TWFFIType::StaticFunction), function).unwrap();
        }

        let yaml = fs::read_to_string(dir.join("bindings/Replace.yaml")).unwrap();
        let config: TWConfig = serde_yaml::from_str(&yaml).unwrap();
        assert_eq!(config.static_functions.len(), 1);
        assert_eq!(config.static_functions[0].rust_name, "new");

        fs::remove_dir_all(&dir).unwrap();
    }
}
