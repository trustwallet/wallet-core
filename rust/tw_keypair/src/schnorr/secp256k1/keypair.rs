use crate::schnorr::secp256k1::private::PrivateKey;
use crate::schnorr::secp256k1::public::PublicKey;

pub struct KeyPair {
    private: PrivateKey,
    public: PublicKey,
}
