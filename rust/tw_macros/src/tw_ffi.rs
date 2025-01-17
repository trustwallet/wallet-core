use derive_syn_parse::Parse;
use proc_macro2::TokenStream as TokenStream2;
use quote::quote;
use syn::{parse::{Parse, ParseStream}, parse2, parse_quote, Ident, Result, Token};
use serde::{Deserialize, Serialize};

use std::env;
use std::fs;
use std::path::Path;
use std::path::PathBuf;
pub mod keywords {
    use syn::custom_keyword;

    custom_keyword!(ty);
    custom_keyword!(class);
    custom_keyword!(name);
    custom_keyword!(static_function);
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum TWFFIType {
    StaticFunction,
}

impl Parse for TWFFIType {
    fn parse(input: ParseStream) -> Result<Self> {
        let lookahead = input.lookahead1();
        if lookahead.peek(keywords::static_function) {
            input.parse::<keywords::static_function>()?;
            Ok(Self::StaticFunction)
        } else {
            Err(lookahead.error())
        }
    }
}

impl ToString for TWFFIType {
    fn to_string(&self) -> String {
        match self {
            TWFFIType::StaticFunction => "static_function".to_string(),
        }
    }
}

#[derive(Parse, Clone)]
pub struct TWFFIAttrArgs {
    pub ty_keyword: Option<keywords::ty>,
    #[parse_if(ty_keyword.is_some())]
    pub _eq: Option<Token![=]>,
    #[parse_if(ty_keyword.is_some())]
    pub ty: Option<TWFFIType>,
    #[parse_if(ty_keyword.is_some())]
    pub _comma: Option<Token![,]>,

    pub class_keyword: Option<keywords::class>,
    #[parse_if(class_keyword.is_some())]
    pub _eq2: Option<Token![=]>,
    #[parse_if(class_keyword.is_some())]
    pub class: Option<Ident>,
    #[parse_if(class_keyword.is_some())]
    pub _comma2: Option<Token![,]>,

    pub name_keyword: Option<keywords::name>,
    #[parse_if(name_keyword.is_some())]
    pub _eq3: Option<Token![=]>,
    #[parse_if(name_keyword.is_some())]
    pub name: Option<Ident>,
}


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

pub fn tw_ffi(attr: TokenStream2, item: TokenStream2) -> Result<TokenStream2> {
    let args = parse2::<TWFFIAttrArgs>(attr)?;

    let func = parse2::<syn::ItemFn>(item.clone())?;
    let func_name = func.sig.ident.to_string();
    let func_args = func.sig.inputs.iter().map(|arg| match arg {
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
    let static_function = TWStaticFunction {
        name: args.name.unwrap().to_string(),
        rust_name: func_name,
        args: func_args,
        return_type,
    };

    if let Ok(out_dir) = env::var("CARGO_WORKSPACE_DIR") {
        let bindings_dir = Path::new(&out_dir).join("bindings");
        fs::create_dir_all(&bindings_dir).expect("Failed to create bindings directory");
        let yaml_file_path = bindings_dir.join(format!("{}.yaml", class));
        
        let mut config = if yaml_file_path.exists() {
            serde_yaml::from_str(&fs::read_to_string(&yaml_file_path).expect("Failed to read existing YAML file")).expect("Failed to parse YAML file")
        } else {
            TWConfig {
                class,
                static_functions: vec![],
            }
        }; 
        config.static_functions.push(static_function);
        
        let yaml_output: String = serde_yaml::to_string(&config).expect("Failed to serialize to YAML");        
        fs::write(&yaml_file_path, yaml_output).expect("Failed to write YAML file");
    } else {
        panic!("CARGO_WORKSPACE_DIR is not set");
    }

    Ok(item)
}

#[test]
fn test_ffi_attr_arg_parsing() {
    let args = parse2::<TWFFIAttrArgs>(quote!{
        ty = static_function,
        class = MyClass,
        name = MyName
    }).unwrap();
}
