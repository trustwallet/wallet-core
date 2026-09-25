// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::sha2::sha512;
use tw_keypair::ed25519::sha512::PrivateKey;
use tw_keypair::ed25519::Signature;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::KeyPairResult;

pub const TON_PERSONAL_MESSAGE_PREFIX: &str = "ton-safe-sign-magic";

pub struct PersonalMessageSigner;

impl PersonalMessageSigner {
    /// Signs an arbitrary message.
    /// https://www.openmask.app/docs/api-reference/rpc-api#ton_personalsign
    /// https://github.com/OpenProduct/openmask-extension/blob/7566ceb2772fed7a3a27d2a67bd34bf89e862557/src/view/screen/notifications/sign/api.ts#L21-L48
    pub fn sign(private_key: &PrivateKey, msg: &str) -> KeyPairResult<Signature> {
        let msg_hash = sha512(msg.as_bytes());

        let mut msg_to_sign = vec![0xff_u8, 0xff];
        msg_to_sign.extend_from_slice(TON_PERSONAL_MESSAGE_PREFIX.as_bytes());
        msg_to_sign.extend_from_slice(msg_hash.as_slice());

        private_key.sign(msg_to_sign)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_hash::H512;

    #[test]
    fn test_sign_personal_message() {
        // The private key has been derived by using [ton-mnemonic](https://www.npmjs.com/package/tonweb-mnemonic/v/0.0.2)
        // from the following mnemonic:
        // document shield addict crime broom point story depend suit satisfy test chicken valid tail speak fortune sound drill seek cube cheap body music recipe
        let private_key = PrivateKey::try_from(
            "112d4e2e700a468f1eae699329202f1ee671d6b665caa2d92dea038cf3868c18",
        )
        .unwrap();

        let signature = PersonalMessageSigner::sign(&private_key, "Hello world").unwrap();
        // The following signature has been computed by calling `window.ton.send("ton_personalSign", { data: "Hello world" });`.
        let expected_sig = "2490fbaa72aec0b77b19162bbbe0b0e3f7afd42cc9ef469f0494cd4a366a4bf76643300cd5991f66bce6006336742b8d1d435d541d244dcc013d428472e89504";
        assert_eq!(signature.to_bytes(), H512::from(expected_sig));
    }
}
