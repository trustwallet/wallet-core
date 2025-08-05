// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::PublicKey;
use super::Signature;
use crate::ecdsa::canonical::generate_k;
use crate::traits::DerivableKeyTrait;
use crate::KeyPairResult;
use crate::{traits::SigningKeyTrait, KeyPairError};
use ecdsa::elliptic_curve::PrimeField;
use ecdsa::hazmat::{bits2field, DigestPrimitive};
use k256::elliptic_curve::bigint::ArrayEncoding;
use k256::elliptic_curve::Curve;
use k256::Secp256k1;
use k256::{
    elliptic_curve::{ops::Reduce, sec1::ToEncodedPoint},
    AffinePoint, Scalar, U256,
};
use rfc6979::HmacDrbg;
use secp256k1::rand;
use sha2::{Digest, Sha256};
use std::ops::Deref;
use std::str::FromStr;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::ZeroizeOnDrop;
use zeroize::Zeroizing;

type CurveDigest<C> = <C as DigestPrimitive>::Digest;

#[derive(Debug, Clone, ZeroizeOnDrop)]
/// secp256k1 (K-256) secret key.
pub struct PrivateKey(k256::SecretKey);

impl PrivateKey {
    /// Generates a random private key.
    ///
    /// # Example
    /// ```
    /// use tw_keypair::zilliqa_schnorr::PrivateKey;
    /// let private_key = PrivateKey::create_random();
    /// ```
    pub fn create_random() -> Self {
        Self(k256::SecretKey::random(&mut rand::thread_rng()))
    }

    /// Constructs a private key from a raw secret key.
    pub fn from_slice(slice: &[u8]) -> Result<Self, KeyPairError> {
        Ok(Self(
            k256::SecretKey::from_slice(slice).map_err(|_| KeyPairError::InvalidSecretKey)?,
        ))
    }

    /// Returns corresponding public key of the private key
    pub fn public(&self) -> PublicKey {
        PublicKey::new(self.0.public_key())
    }

    // Taken from https://github.com/Zilliqa/zilliqa-rs/blob/24a0e882bcab634b6e776d94709c1760841023d4/src/crypto/schnorr.rs#L20
    fn sign_inner(&self, k: Scalar, message: &[u8]) -> Option<Signature> {
        let public_key = self.public();

        // 2. Compute the commitment Q = kG, where G is the base point.
        let q = AffinePoint::GENERATOR * k;

        // 3. Compute the challenge r = H(Q, kpub, m)
        let mut hasher = Sha256::new();
        hasher.update(q.to_encoded_point(true).to_bytes());
        hasher.update(public_key.to_encoded_point(true).to_bytes());
        hasher.update(message);
        let r = <Scalar as Reduce<U256>>::reduce_bytes(&hasher.finalize());

        // 4. If r = 0 mod(order), goto 1
        if r.is_zero().into() {
            return None;
        }

        // 5. Compute s = k - r*kpriv mod(order)
        let s: Scalar = k - r.mul(&self.0.as_scalar_primitive().into());

        // 6. If s = 0 goto 1.
        if s.is_zero().into() {
            return None;
        }

        // 7. Signature on m is (r, s)
        k256::ecdsa::Signature::from_scalars(r.to_bytes(), s.to_bytes())
            .map(|signature| Signature { signature })
            .ok()
    }
}

impl TryFrom<&[u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(bytes: &[u8]) -> Result<Self, Self::Error> {
        Self::from_slice(bytes)
    }
}

impl FromStr for PrivateKey {
    type Err = KeyPairError;

    fn from_str(secret_key: &str) -> Result<Self, Self::Err> {
        let bytes = hex::decode(secret_key).map_err(|_| KeyPairError::InvalidSecretKey)?;
        Ok(Self(
            k256::SecretKey::from_slice(&bytes).map_err(|_| KeyPairError::InvalidSecretKey)?,
        ))
    }
}

impl Deref for PrivateKey {
    type Target = k256::SecretKey;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let secret = Zeroizing::new(self.0.to_bytes());
        Zeroizing::new(secret.as_slice().to_vec())
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    // Taken from https://github.com/Zilliqa/zilliqa-rs/blob/24a0e882bcab634b6e776d94709c1760841023d4/src/crypto/schnorr.rs#L20
    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        let priv_scalar = self.0.as_scalar_primitive();
        let entropy_input = &priv_scalar.to_bytes();

        let message_hash = sha2::Sha256::digest(message.as_slice());
        let nonce = bits2field::<Secp256k1>(message_hash.as_slice())
            .map_err(|_| KeyPairError::InvalidSignMessage)?;
        let n = Secp256k1::ORDER.to_be_byte_array();
        let additional_data = &[];

        let mut hmac_drbg =
            HmacDrbg::<CurveDigest<Secp256k1>>::new(entropy_input, &nonce, additional_data);

        for _ in 0..10000 {
            let k = generate_k(&mut hmac_drbg, &n);
            let k_scalar = Scalar::from_repr(k).unwrap();

            if let Some(signature) = self.sign_inner(k_scalar, message.as_slice()) {
                return Ok(signature);
            }
        }

        Err(KeyPairError::SigningError)
    }
}

impl DerivableKeyTrait for PrivateKey {
    fn derive_child(&self, other: H256) -> KeyPairResult<Self> {
        let child_scalar = Option::<k256::NonZeroScalar>::from(k256::NonZeroScalar::from_repr(
            other.take().into(),
        ))
        .ok_or(KeyPairError::InternalError)?;

        let derived_scalar = self.0.to_nonzero_scalar().as_ref() + child_scalar.as_ref();

        let secret = Option::<k256::NonZeroScalar>::from(k256::NonZeroScalar::new(derived_scalar))
            .map(Into::into)
            .ok_or(KeyPairError::InternalError)?;

        Ok(PrivateKey(secret))
    }
}

#[cfg(test)]
mod tests {
    use super::{PrivateKey, PublicKey};
    use crate::traits::VerifyingKeyTrait;
    use k256::{elliptic_curve::PrimeField, FieldBytes, Scalar};
    use tw_encoding::hex;

    #[test]
    fn signing() {
        // From https://github.com/Zilliqa/zilliqa-js/blob/226b371eaac78ed80e7b40b93189b6a97086bdf5/packages/zilliqa-js-crypto/test/schnorr.spec.ts#L23.
        let cases = [
            (
                "A7F1D92A82C8D8FE434D98558CE2B347171198542F112D0558F56BD68807999248336241F30D23E55F30D1C8ED610C4B0235398184B814A29CB45A672ACAE548E9C5F1B0C4158AE59B4D39F6F7E8A105D3FEEDA5D5F3D9E45BFA6CC351E220AE0CE106986D61FF34A11E19FD3650E9B7818FC33A1E0FC02C44557AC8AB50C9B2DEB2F6B5E24C4FDD9F8867BDCE1FF261008E7897970E346207D75E47A158298E5BA2F56246869CC42E362A02731264E60687EF5309D108534F51F8658FB4F080B7CB19EE9AEBD718CC4FA27C8C37DFC1ADA5D133D13ABE03F021E9B1B78CCBD82F7FF2B38C6D48D01E481B2D4FAF7171805FD7F2D39EF4C4F19B9496E81DAB8193B3737E1B27D9C43957166441B93515E8F03C95D8E8CE1E1864FAAD68DDFC5932130109390B0F1FE5CA716805F8362E98DCCAADC86ADBED25801A9A9DCFA6264319DDAFE83A89C51F3C6D199D38DE10E660C37BE872C3F2B31660DE8BC95902B9103262CDB941F77376F5D3DBB7A3D5A387797FC4819A035ECA704CEDB37110EE7F206B0C8805AAEBF4963E7C4708CE8D4E092366E71792A8A3B2BBCDEE321B3E15380C541EF0930888969F7457AFE18588826A419D58311C1784B5484EECDB393F6A0ACA11B91DF0866B500B8DEE501FD7EB9BCE09A17D74124B4605ADFC0777BED9816D8D7E8488544A18D8045CB3283B0A752B881B5F500FADB59010E63D",
                "039E43C9810E6CC09F46AAD38E716DAE3191629534967DC457D3A687D2E2CDDC6A",
                "0F494B8312E8D257E51730C78F8FE3B47B6840C59AAAEC7C2EBE404A2DE8B25A",
                "532B2267C4A3054F380B3357339BDFB379E88366FE61B42ACA05F69BC3F6F54E",
                "3AF3D288E830E96FF8ED0769F45ABDA774CD989E2AE32EF9E985C8505F14FF98",
                "E191EB14A70B5B53ADA45AFFF4A04578F5D8BB2B1C8A22985EA159B53826CDE7",
            ),
            (
                "1B664F8BDA2DBF33CB6BE21C8EB3ECA9D9D5BF144C08E9577ED0D1E5E560875109B340980580473DBC2E689A3BE838E77A0A3348FE960EC9BF81DA36F1868CA5D24788FA4C0C778BF0D12314285495636516CF40861B3D737FD35DBB591C5B5D25916EB1D86176B14E0E67D2D03957F0CF6C87834BF328540588360BA7C7C5F88541634FB7BADE5F94FF671D1FEBDCBDA116D2DA779038ED7679896C29198B2657B58C50EA054F644F4129C8BA8D8D544B727633DD40754398046796E038626FEF9237CE5B615BC08677EE5ABFBD85F73F7F8868CB1B5FBA4C1309F16061AA133821FBE2A758D2BBE6AA040A940D41B7D3B869CEE945150AA4A40E6FF719EEC24B2681CD5CE06B50273436584066046656D5EFED7315759189D68815DDB9E5F8D7FD53B6EC096616A773B9421F6704CED36EF4E484BA0C6C5A4855C71C33A54AC82BE803E5CFD175779FC444B7E6AA9001EEFABEBC0CF99754887C7B0A27AFDDC415F8A02C5AF1EFEA26AD1E5D92B1E29A8FAF5B2186C3094F4A137BCFAA65D7B274214DB64C86F3085B24938E1832FB310A6F064181E298D23062ABC817BA173023C8C04C5C3A1ECBF4AF72372B381FF69865C8F0E3C70B931C45A7419B3C441842EBFACC3D070AC3B433CD120B6E85B72DADCF40B23B173C34F6BE1B1901F6621F1497B085CF8E999D986EF8FF3A889A0238979983A8686F69E10EF9249A87",
                "0245DC2911EDC02F2774E0A40FBEB0112EA60BF513F9EC50889D59FC94C97EC18F",
                "8D566BB87EF69FFDA622E0A59FBAAFE57F486CE65844343A5D9B97DE9C4F619A",
                "948AFFFF6E068CA2F2757BFD6085D6E4C3084B038E5533C5927ECB19EA0D329C",
                "DFEE66E2C4799E73F0F778126A23032608408C27C2E7B3FA45A626BB9BDEB53C",
                "75445CC9DBFE4E7BC64E020FA22CACFA4C40D5AA84DD6AEF661564FCA9746C40",
            ),
            (
                "3444C8501F19A8A78670F748FA401C4020AE086D7157A3837EC721DEF0D6E095928C5B78ED9B95560CE33D5B22778BE66DCEF2D21878D481DFF41A4DEDCAFDCAEAB4BD78629D7EC40FD26F1DD954CA84A3B53B84E9903056E840837A1390F37BB8ADE799DAC1E465D811916547EB4B6A163082E9833634A1224C54F681B8DC70A792C0CB4671D4970CCC80E2168CE920CC8FA07B1F90E9898D16019913ED5B8EE8A8DE7AB6F7895601FD20E49FD73E6F5D24C0D97E67871539F0E4E32CCB6677AFF03356D1F3790945E94039E51A63B3C840B74E3053D95CA71C0D3AC20A9065828D30AB5BFB6188A8F291FB1EB4E1EED03E2F5F558C00D8E3084120DEEB8BFE908429B36A896A45D624E79372CC18DF37DB2D20C9726D4FEF7BECF220138B53BC54C2DA461A9955AFF33F2F93DD96464BF3E883FC5750BDBE79BC2F82427F41DE42659AC4B111D7CEF8085003469DF8C9D3541480C6841707CE4C8F3D003AF982AD35C2733D0FA3B1EE52A6DAB36203D99AEC179A565B5050F480235C3BC560AA28EF5DD5525BFA254E584A86FDBD4BCC5B56551BAD00255CB72F806D7F3C533321B0864007AFBA4E0FF9638517FA8D788F52766F3A28C57C428BFDD4234AA760CE8044DF1E1FBA58E8B1D9C5A79D2AC4592FC31702F7E83351D2160C09C5CEA554F2C93A61C040E225612DF2B550900B097E18638350E3BA15C9AD53CE1861",
                "02237627FE7374061FBD80AEA842DCE76D9206F0DDC7B319F3B30FA75DBD4F009A",
                "009755F442D66585A10B80A49850C77764AD029D1BEA73F4DA45AB331306E6E5",
                "2D78C77B736AD0A00FDF60695C01E96520656C13DC890A5B864672C6CED1C49A",
                "4B73D4D919D7B4DEF330391899EA02023851CABE044E34E18EAE3E10588CECCD",
                "D5DE85C4BDEA5910DC36AEF5660774D65291322C1E87FDA0D00C864E8C5FED29",
            ),
        ];
        for (message, public_key, secret_key, k, r, s) in cases {
            let k =
                Scalar::from_repr(FieldBytes::clone_from_slice(&hex::decode(k).unwrap())).unwrap();
            let message = hex::decode(message).unwrap();
            let secret_key = secret_key.parse::<PrivateKey>().unwrap();
            let public_key = public_key.parse::<PublicKey>().unwrap();

            let signature = secret_key.sign_inner(k, &message).unwrap();

            assert_eq!(signature.signature.r().to_string(), r);
            assert_eq!(signature.signature.s().to_string(), s);

            assert!(public_key.verify(signature, message));
        }
    }
}
