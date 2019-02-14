with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "trezor-crypto-dev";
  buildInputs = [ gnumake gcc pkgconfig openssl check ];
}
