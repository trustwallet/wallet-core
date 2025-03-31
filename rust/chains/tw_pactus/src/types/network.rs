use tw_coin_entry::error::prelude::*;

/// Represents the type of network (e.g., Mainnet or Testnet).
///
/// The `CoinType` for Mainnet is defined as `21888`, and for Testnet, it is `21777`.
///
/// The network type does not affect the decoding or encoding of addresses or transactions.
/// Instead, it is primarily used to facilitate the conversion of an address or public key
/// into its string representation (using bech32m).
///
/// Note: TrustWallet Core does not provide an API for converting a public key directly
/// to its string representation; it only converts it to a hex representation.
/// However, it provides the API to convert an address to its string representation.
#[derive(Debug, Clone, PartialEq)]
pub enum Network {
    /// The network type is either unknown or not explicitly set.
    ///
    /// Address raw bytes do not inherently carry the network type,
    /// but the address string carries the network using an HRP (Human-Readable Part).
    /// - Mainnet addresses start with `pc1...`.
    /// - Testnet addresses start with `tpc1...`.
    ///
    /// When deriving an address from a string, the network type is inferred from the HRP.
    /// When decoding an address from a public key or raw data, the network type must be explicitly set.
    Unknown = 0,

    /// Represents the Mainnet network.
    Mainnet = 1,

    /// Represents the Testnet network.
    Testnet = 2,
}

const MAINNET_ADDRESS_HRP: &str = "pc";
const TESTNET_ADDRESS_HRP: &str = "tpc";

pub const MAINNET_PUBLIC_KEY_HRP: &str = "public";
pub const TESTNET_PUBLIC_KEY_HRP: &str = "tpublic";

impl Network {
    pub fn try_from_hrp(hrp: &str) -> Result<Self, AddressError> {
        match hrp {
            MAINNET_ADDRESS_HRP => Ok(Network::Mainnet),
            TESTNET_ADDRESS_HRP => Ok(Network::Testnet),
            _ => Err(AddressError::InvalidHrp),
        }
    }

    pub fn address_hrp(&self) -> Result<&'static str, AddressError> {
        match &self {
            Network::Mainnet => Ok(MAINNET_ADDRESS_HRP),
            Network::Testnet => Ok(TESTNET_ADDRESS_HRP),
            Network::Unknown => Err(AddressError::InvalidHrp),
        }
    }

    pub fn public_key_hrp(&self) -> Result<&'static str, AddressError> {
        match &self {
            Network::Mainnet => Ok(MAINNET_PUBLIC_KEY_HRP),
            Network::Testnet => Ok(TESTNET_PUBLIC_KEY_HRP),
            Network::Unknown => Err(AddressError::InvalidHrp),
        }
    }
}
