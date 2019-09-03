// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//


import { WalletCore } from '../WalletCore';

export enum Curve {
    SECP256K1 = 0,
    ED25519 = 1,
    ED25519BLAKE2BNANO = 2,
    CURVE25519 = 3,
    NIST256P1 = 4
}

export class CurveUtil {

    public static toString(curve: Curve): string {
        switch (curve) {
            case Curve.SECP256K1: return 'secp256k1';
            case Curve.ED25519: return 'ed25519';
            case Curve.ED25519BLAKE2BNANO: return 'ed25519-blake2b-nano';
            case Curve.CURVE25519: return 'curve25519';
            case Curve.NIST256P1: return 'nist256p1';
            default: return '';
        }
    }

}
