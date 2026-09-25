use proc_macro::TokenStream;

mod code_gen;
mod tw_ffi;
mod utils;

#[proc_macro_attribute]
pub fn tw_ffi(attr: TokenStream, item: TokenStream) -> TokenStream {
    match tw_ffi::tw_ffi(attr.into(), item.into()) {
        Ok(item) => item.into(),
        Err(e) => e.to_compile_error().into(),
    }
}
