use tw_hash::{H256, H512};
use tw_scale::{Compact, ToScale, impl_struct_scale, impl_enum_scale};

use crate::address::PolkadotAddress;

pub type TxHash = H256;
pub type BlockHash = H256;
pub type BlockNumber = u32;

pub type AccountId = PolkadotAddress;
pub type AccountIndex = u32;

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum MultiSignature {
    Ed25519(H512),
    Sr25519(H512),
    //Ecdsa([u8; 65]),
  }
);

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum MultiAddress {
    /// It's an account ID (pubkey).
    Id(AccountId),
    // /// It's an account index.
    // Index(Compact<AccountIndex>),
    // /// It's some arbitrary raw bytes.
    // Raw(Vec<u8>),
    // /// It's a 32 byte representation.
    // Address32([u8; 32]),
    // /// Its a 20 byte representation.
    // Address20([u8; 20]),
  }
);

impl From<AccountId> for MultiAddress {
  fn from(other: AccountId) -> Self {
    Self::Id(other)
  }
}

impl_struct_scale!(
  #[derive(Clone, Debug, Default)]
  pub struct AdditionalSigned {
    pub spec_version: u32,
    pub tx_version: u32,
    pub genesis_hash: BlockHash,
    pub current_hash: BlockHash,
  }
);

#[derive(Clone, Copy, Debug)]
pub enum Era {
  Immortal,
  Mortal(u64, u64),
}

impl ToScale for Era {
    fn to_scale_into(&self, _out: &mut Vec<u8>) {
        todo!("Scale encode Era")
    }
}

impl_struct_scale!(
  #[derive(Clone, Debug)]
  pub struct Extra {
    era: Era,
    nonce: Compact<u32>,
    tip: Compact<u128>,
  }
);

impl Extra {
  pub fn new(era: Era, nonce: u32) -> Self {
    Self {
      era,
      nonce: Compact(nonce),
      tip: Compact(0u128),
    }
  }

  pub fn nonce(&self) -> u32 {
    self.nonce.0
  }

  pub fn tip(&self) -> u128 {
    self.tip.0
  }
}

#[derive(Clone, Debug)]
pub struct Encoded(pub Vec<u8>);

impl ToScale for Encoded {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend(&self.0);
    }
}

pub struct SignedPayload<'a>((&'a Encoded, &'a Extra, AdditionalSigned));

impl<'a> ToScale for SignedPayload<'a> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.0.to_scale_into(out);
    }
}

impl<'a> SignedPayload<'a> {
  pub fn new(call: &'a Encoded, extra: &'a Extra, additional: AdditionalSigned) -> Self {
    Self((call, extra, additional))
  }
}

/// PreparedTransaction holds all data needed to sign a transaction.
#[derive(Clone, Debug)]
pub struct PreparedTransaction {
  pub call: Encoded,
  pub extra: Extra,
  pub additional: AdditionalSigned,
}

impl PreparedTransaction {
  pub fn new(
    additional: AdditionalSigned,
    extra: Extra,
    call: Encoded,
  ) -> Self {
    Self {
      additional,
      extra,
      call,
    }
  }

  /*
  pub async fn sign(self, signer: &mut impl Signer) -> Result<ExtrinsicV4> {
    let account = signer.account();
    let payload = SignedPayload::new(&self.call, &self.extra, self.additional);
    let payload = payload.to_scale();
    let sig = signer.sign(&payload[..]).await?;

    let xt = ExtrinsicV4::signed(account, sig, self.extra, self.call);
    Ok(xt)
  }
  */
}

impl_struct_scale!(
  #[derive(Clone, Debug)]
  pub struct ExtrinsicSignature {
    pub account: MultiAddress,
    pub signature: MultiSignature,
    pub extra: Extra,
  }
);

/// Current version of the `UncheckedExtrinsic` format.
pub const EXTRINSIC_VERSION: u8 = 4;
pub const SIGNED_EXTRINSIC_BIT: u8 = 0b1000_0000;
pub const UNSIGNED_EXTRINSIC_MASK: u8 = 0b0111_1111;

#[derive(Clone, Debug)]
pub struct ExtrinsicV4 {
  pub signature: Option<ExtrinsicSignature>,
  pub call: Encoded,
}

impl ExtrinsicV4 {
  pub fn signed(account: AccountId, sig: MultiSignature, extra: Extra, call: Encoded) -> Self {
    Self {
      signature: Some(ExtrinsicSignature {
        account: MultiAddress::from(account),
        signature: sig,
        extra,
      }),
      call,
    }
  }
}

impl ToScale for ExtrinsicV4 {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        // 1 byte version id and signature if signed.
        match &self.signature {
          Some(sig) => {
            out.push(EXTRINSIC_VERSION | SIGNED_EXTRINSIC_BIT);
            sig.to_scale_into(out);
          }
          None => {
            out.push(EXTRINSIC_VERSION & UNSIGNED_EXTRINSIC_MASK);
          }
        }
        self.call.to_scale_into(out);
    }
}
