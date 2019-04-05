// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


export enum StellarPassphrase {
    STELLAR = 0,
    KIN = 1
}

export class StellarPassphraseUtil {

    public static toString(stellarpassphrase: StellarPassphrase): string {
        switch (stellarpassphrase) {
            case StellarPassphrase.STELLAR: return 'Public Global Stellar Network ; September 2015';
            case StellarPassphrase.KIN: return 'Kin Mainnet ; December 2018';
            default: return '';
        }
    }

}
