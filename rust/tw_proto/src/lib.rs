#[allow(non_snake_case)]
#[rustfmt::skip]
mod generated {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

pub use generated::TW::*;
