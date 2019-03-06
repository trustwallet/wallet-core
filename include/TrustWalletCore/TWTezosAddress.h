TW_EXPORT_STATIC_METHOD
  
TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address);	TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address);
/// Determines if the data is a valid Tezos address.	/// Determines if the data is a valid Tezos address.
TW_EXPORT_STATIC_METHOD	TW_EXPORT_STATIC_METHOD
bool TWTezosAddressIsValid(TWData *_Nonnull data);	bool TWTezosAddressIsValid(TWData *_Nonnull data);
/// Determines if the string is a valid Tezos address.	/// Determines if the string is a valid Tezos address.
TW_EXPORT_STATIC_METHOD	TW_EXPORT_STATIC_METHOD
bool TWTezosAddressIsValidString(TWString *_Nonnull string);	bool TWTezosAddressIsValidString(TWString *_Nonnull string);
/// Initializes an address from a base58 sring representaion.	/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD	TW_EXPORT_STATIC_METHOD
bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string);	bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string);
/// Initializes an address from a public key.	/// Initializes an address from a public key.
TW_EXPORT_STATIC_METHOD	TW_EXPORT_STATIC_METHOD
bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey);	bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey);
/// Returns the address data.	/// Returns the address data.
TW_EXPORT_STATIC_METHOD	TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress *_Nonnull address);	TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress *_Nonnull address);

TW_EXTERN_C_END