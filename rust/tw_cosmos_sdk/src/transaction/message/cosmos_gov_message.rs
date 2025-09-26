// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::proto::cosmos;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use serde::Serialize;
use tw_coin_entry::error::prelude::*;
use tw_proto::to_any;

pub enum VoteOption {
    Unspecified,
    Yes,
    Abstain,
    No,
    NoWithVeto,
}

pub struct VoteMessage<Address: CosmosAddress> {
    pub proposal_id: u64,
    pub voter: Address,
    pub option: VoteOption,
}

impl<Address: CosmosAddress> CosmosMessage for VoteMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        use cosmos::gov::v1beta1::VoteOption as ProtoVoteOption;

        let option = match self.option {
            VoteOption::Unspecified => ProtoVoteOption::VOTE_OPTION_UNSPECIFIED,
            VoteOption::Yes => ProtoVoteOption::VOTE_OPTION_YES,
            VoteOption::Abstain => ProtoVoteOption::VOTE_OPTION_ABSTAIN,
            VoteOption::No => ProtoVoteOption::VOTE_OPTION_NO,
            VoteOption::NoWithVeto => ProtoVoteOption::VOTE_OPTION_NO_WITH_VETO,
        };

        let proto_msg = cosmos::gov::v1beta1::MsgVote {
            proposal_id: self.proposal_id,
            voter: self.voter.to_string().into(),
            option,
        };
        Ok(to_any(&proto_msg))
    }
}

/// TextProposal defines a standard text proposal
#[derive(Clone, Serialize)]
pub struct TextProposal {
    pub title: String,
    pub description: String,
}

impl CosmosMessage for TextProposal {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::gov::v1beta1::TextProposal {
            title: self.title.clone().into(),
            description: self.description.clone().into(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("/cosmos.gov.v1beta1.TextProposal", self)
    }
}

/// MsgExecLegacyContent is used to wrap the legacy content field into a message.
/// This ensures backwards compatibility with v1beta1.MsgSubmitProposal.
pub struct MsgExecLegacyContent {
    pub content: ProtobufMessage,
    pub authority: String,
}

impl CosmosMessage for MsgExecLegacyContent {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::gov::v1::MsgExecLegacyContent {
            content: Some(self.content.clone()),
            authority: self.authority.clone().into(),
        };
        Ok(to_any(&proto_msg))
    }
}

/// DYDX Governance Proposal Message - combines MsgExecLegacyContent with TextProposal
pub struct DydxGovernanceProposal<Address: CosmosAddress> {
    pub title: String,
    pub description: String,
    pub authority: Address,
}

impl<Address: CosmosAddress> CosmosMessage for DydxGovernanceProposal<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        // First create the TextProposal content
        let text_proposal = TextProposal {
            title: self.title.clone(),
            description: self.description.clone(),
        };
        let text_proposal_any = text_proposal.to_proto()?;

        // Then wrap it in MsgExecLegacyContent
        let exec_legacy_content = MsgExecLegacyContent {
            content: text_proposal_any,
            authority: self.authority.to_string(),
        };

        exec_legacy_content.to_proto()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::address::Address;
    use std::str::FromStr;

    #[test]
    fn test_text_proposal_creation() {
        let text_proposal = TextProposal {
            title: "Test Proposal Title".to_string(),
            description: "Test Proposal Description".to_string(),
        };

        // Test protobuf serialization
        let proto_result = text_proposal.to_proto();
        assert!(proto_result.is_ok());

        // Test JSON serialization
        let json_result = text_proposal.to_json();
        assert!(json_result.is_ok());
    }

    #[test]
    fn test_dydx_governance_proposal() {
        let authority = Address::from_str("dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky").unwrap();

        let proposal = DydxGovernanceProposal {
            title: "Title of test proposal".to_string(),
            description: "Description of the test proposal".to_string(),
            authority,
        };

        // Test protobuf serialization
        let proto_result = proposal.to_proto();
        assert!(proto_result.is_ok());

        let proto_msg = proto_result.unwrap();
        assert_eq!(proto_msg.type_url, "/cosmos.gov.v1.MsgExecLegacyContent");
    }

    #[test]
    fn test_msg_exec_legacy_content() {
        let text_proposal = TextProposal {
            title: "Legacy Content Title".to_string(),
            description: "Legacy Content Description".to_string(),
        };

        let text_proposal_any = text_proposal.to_proto().unwrap();

        let exec_legacy = MsgExecLegacyContent {
            content: text_proposal_any,
            authority: "dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky".to_string(),
        };

        let proto_result = exec_legacy.to_proto();
        assert!(proto_result.is_ok());

        let proto_msg = proto_result.unwrap();
        assert_eq!(proto_msg.type_url, "/cosmos.gov.v1.MsgExecLegacyContent");
    }
}
