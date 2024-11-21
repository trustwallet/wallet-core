use tw_scale::ToScale;
use tw_substrate::Era;

#[test]
fn encode_era() {
    let e1 = Era::mortal(8, 429119);
    let e2 = Era::mortal(4, 428861);
    let e3 = Era::mortal(64, 4246319);

    assert_eq!(e1.to_scale(), vec![0x72, 0x00]);
    assert_eq!(e2.to_scale(), vec![0x11, 0x00]);
    assert_eq!(e3.to_scale(), vec![0xf5, 0x02]);
}

// Era tests ported from: https://github.com/paritytech/polkadot-sdk/blob/657b5503a04e97737696fa7344641019350fb521/substrate/primitives/runtime/src/generic/era.rs#L182
#[test]
fn immortal_works() {
    let e = Era::immortal();
    assert_eq!(e.to_scale(), vec![0u8]);
}

#[test]
fn mortal_codec_works() {
    let e = Era::mortal(64, 42);

    let expected = vec![5 + 42 % 16 * 16, 42 / 16];
    assert_eq!(e.to_scale(), expected);
}

#[test]
fn long_period_mortal_codec_works() {
    let e = Era::mortal(32768, 20000);

    let expected = vec![(14 + 2500 % 16 * 16) as u8, (2500 / 16) as u8];
    assert_eq!(e.to_scale(), expected);
}

#[test]
fn era_initialization_works() {
    assert_eq!(Era::mortal(64, 42), Era::Mortal(64, 42));
    assert_eq!(Era::mortal(32768, 20000), Era::Mortal(32768, 20000));
    assert_eq!(Era::mortal(200, 513), Era::Mortal(256, 1));
    assert_eq!(Era::mortal(2, 1), Era::Mortal(4, 1));
    assert_eq!(Era::mortal(4, 5), Era::Mortal(4, 1));
}

#[test]
fn quantized_clamped_era_initialization_works() {
    // clamp 1000000 to 65536, quantize 1000001 % 65536 to the nearest 4
    assert_eq!(
        Era::mortal(1000000, 1000001),
        Era::Mortal(65536, 1000001 % 65536 / 4 * 4)
    );
}
