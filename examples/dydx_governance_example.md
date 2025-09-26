# DYDX Governance Proposal Implementation

This document explains how to use the DYDX governance proposal functionality implemented in Trust Wallet Core.

## Overview

The implementation supports creating governance proposals on the dYdX Chain using the `MsgExecLegacyContent` message type with `TextProposal` content. This matches the JSON structure provided in the requirements:

```json
{
  "title": "Title of test proposal",
  "deposit": "2000000000000000000000adydx",
  "summary": "Summary of the test proposal",
  "messages": [
    {
      "@type": "/cosmos.gov.v1.MsgExecLegacyContent",
      "content": {
        "@type": "/cosmos.gov.v1beta1.TextProposal",
        "title": "Title of TextProposal message",
        "description": "Description of TextProposal message"
      },
      "authority": "dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky"
    }
  ]
}
```

## Implementation Details

### 1. Protobuf Definitions

Added two new protobuf files:
- `gov_tx.proto`: Contains v1beta1 governance messages including `TextProposal`
- `gov_v1_tx.proto`: Contains v1 governance messages including `MsgExecLegacyContent`

### 2. Rust Implementation

The following message types were implemented in `cosmos_gov_message.rs`:

#### TextProposal
```rust
pub struct TextProposal {
    pub title: String,
    pub description: String,
}
```

#### MsgExecLegacyContent
```rust
pub struct MsgExecLegacyContent {
    pub content: ProtobufMessage,
    pub authority: String,
}
```

#### DydxGovernanceProposal
```rust
pub struct DydxGovernanceProposal<Address: CosmosAddress> {
    pub title: String,
    pub description: String,
    pub authority: Address,
}
```

### 3. Transaction Builder Integration

Added support in `tx_builder.rs` to handle the new `DydxGovernanceProposal` message type from protobuf input.

### 4. Main Protobuf Schema

Updated `Cosmos.proto` to include the new message type:

```protobuf
message DydxGovernanceProposal {
    string title = 1;
    string description = 2;
    string authority = 3;
}
```

## Usage Example

### Creating a DYDX Governance Proposal

```rust
use tw_cosmos_sdk::transaction::message::cosmos_gov_message::DydxGovernanceProposal;
use tw_cosmos_sdk::address::Address;
use std::str::FromStr;

// Create the governance proposal
let authority = Address::from_str("dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky").unwrap();
let proposal = DydxGovernanceProposal {
    title: "Title of test proposal".to_string(),
    description: "Description of the test proposal".to_string(),
    authority,
};

// Convert to protobuf message for signing
let proto_msg = proposal.to_proto().unwrap();
```

### Using with Transaction Builder

The message can be used with the standard Cosmos SDK transaction builder by including it in the `messages` field of a `SigningInput`.

## Chain Configuration

dYdX Chain is already supported in wallet core with the following configuration:
- Chain ID: `dydx-mainnet-1`
- Coin ID: `22000118`
- Symbol: `DYDX`
- Derivation Path: `m/44'/118'/0'/0/0` (same as Cosmos)
- HRP: `dydx`

## Testing

The implementation includes comprehensive tests that verify:
1. TextProposal creation and serialization
2. MsgExecLegacyContent wrapping functionality
3. DydxGovernanceProposal end-to-end functionality

All tests pass successfully, confirming the implementation works as expected.

## Integration Notes

This implementation provides the foundation for creating and signing dYdX governance proposals. The next steps would be:

1. **Frontend Integration**: Add UI components to collect proposal details
2. **Multisig Support**: Implement the multisig workflow described in the requirements
3. **Broadcasting**: Integrate with dYdX Chain RPC endpoints for proposal submission

The implementation follows the exact message structure required by dYdX Chain governance, ensuring compatibility with the existing CLI workflow while providing a more user-friendly interface through Trust Wallet Core.
