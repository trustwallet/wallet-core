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
use std::path::Path;

use crate::code_gen::{TWArg, TWConfig, TWStaticFunction};

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

impl fmt::Display for TWFFIType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            TWFFIType::StaticFunction => write!(f, "static_function"),
        }
    }
}

#[derive(Parse, Clone)]
pub struct TWFFIAttrArgs {
    pub _ty_keyword: Option<keywords::ty>,
    #[parse_if(_ty_keyword.is_some())]
    pub _eq: Option<Token![=]>,
    #[parse_if(_ty_keyword.is_some())]
    pub _ty: Option<TWFFIType>,
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
    let static_function = TWStaticFunction {
        name: args.name.unwrap().to_string(),
        rust_name: func_name,
        args: func_args,
        return_type,
        docs,
    };

    if let Ok(out_dir) = env::var("CARGO_WORKSPACE_DIR") {
        let bindings_dir = Path::new(&out_dir).join("bindings");
        fs::create_dir_all(&bindings_dir).expect("Failed to create bindings directory");
        let yaml_file_path = bindings_dir.join(format!("{}.yaml", class));

        let mut config = if yaml_file_path.exists() {
            match fs::read_to_string(&yaml_file_path) {
                Ok(contents) => match serde_yaml::from_str(&contents) {
                    Ok(config) => config,
                    Err(_) => {
                        fs::remove_file(&yaml_file_path)
                            .expect("Failed to delete invalid YAML file");
                        TWConfig {
                            class,
                            static_functions: vec![],
                        }
                    },
                },
                Err(_) => TWConfig {
                    class,
                    static_functions: vec![],
                },
            }
        } else {
            TWConfig {
                class,
                static_functions: vec![],
            }
        };
        if let Some(idx) = config
            .static_functions
            .iter()
            .position(|f| f.name == static_function.name)
        {
            config.static_functions[idx] = static_function;
        } else {
            config.static_functions.push(static_function);
        }

        let yaml_output: String =
            serde_yaml::to_string(&config).expect("Failed to serialize to YAML");
        fs::write(&yaml_file_path, yaml_output).expect("Failed to write YAML file");
    } else {
        panic!("CARGO_WORKSPACE_DIR is not set");
    }

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
}
