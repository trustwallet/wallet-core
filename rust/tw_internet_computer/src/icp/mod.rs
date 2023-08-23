pub mod address;
pub mod sign;

mod proto {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}
