// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[repr(u32)]
#[derive(strum_macros::FromRepr)]
pub enum HDVersion {
    None = 0,

    // Bitcoin
    XPUB = 0x0488b21e,
    XPRV = 0x0488ade4,
    YPUB = 0x049d7cb2,
    YPRV = 0x049d7878,
    ZPUB = 0x04b24746,
    ZPRV = 0x04b2430c,
    VPUB = 0x045f1cf6,
    VPRV = 0x045f18bc,
    TPUB = 0x043587cf,
    TPRV = 0x04358394,

    // Litecoin
    LTUB = 0x019da462,
    LTPV = 0x019d9cfe,
    MTUB = 0x01b26ef6,
    MTPV = 0x01b26792,
    TTUB = 0x0436f6e1,
    TTPV = 0x0436ef7d,

    // Decred
    DPUB = 0x2fda926,
    DPRV = 0x2fda4e8,

    // Dogecoin
    DGUB = 0x02facafd,
    DGPV = 0x02fac398,
}

impl HDVersion {
    pub fn is_public(&self) -> bool {
        matches!(
            self,
            HDVersion::XPUB
                | HDVersion::YPUB
                | HDVersion::ZPUB
                | HDVersion::VPUB
                | HDVersion::TPUB
                | HDVersion::LTUB
                | HDVersion::MTUB
                | HDVersion::TTUB
                | HDVersion::DPUB
                | HDVersion::DGUB
        )
    }

    pub fn is_private(&self) -> bool {
        matches!(
            self,
            HDVersion::XPRV
                | HDVersion::YPRV
                | HDVersion::ZPRV
                | HDVersion::VPRV
                | HDVersion::TPRV
                | HDVersion::LTPV
                | HDVersion::MTPV
                | HDVersion::TTPV
                | HDVersion::DPRV
                | HDVersion::DGPV
        )
    }
}
