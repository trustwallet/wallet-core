// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::defined_addresses::*;
use crate::instruction::{AccountMeta, Instruction};
use crate::modules::instruction_builder::system_instruction::SystemInstructionBuilder;
use crate::program::stake_program::StakeProgram;
use serde::{Deserialize, Serialize};
use tw_coin_entry::error::SigningResult;

type UnixTimestamp = i64;
type Epoch = u64;

#[derive(Serialize, Deserialize, Debug, PartialEq, Eq, Clone)]
pub struct Authorized {
    pub staker: SolanaAddress,
    pub withdrawer: SolanaAddress,
}

#[derive(Serialize, Deserialize, Debug, Default, PartialEq, Eq, Clone)]
pub struct Lockup {
    /// UnixTimestamp at which this stake will allow withdrawal, unless the
    /// transaction is signed by the custodian
    pub unix_timestamp: UnixTimestamp,
    /// epoch height at which this stake will allow withdrawal, unless the
    /// transaction is signed by the custodian
    pub epoch: Epoch,
    /// custodian signature on a transaction exempts the operation from
    /// lockup constraints
    pub custodian: SolanaAddress,
}

#[derive(Serialize, Deserialize, Debug, PartialEq, Eq, Clone)]
pub enum StakeAuthorize {
    Staker,
    Withdrawer,
}

#[derive(Serialize, Deserialize, Debug, PartialEq, Eq, Clone)]
pub struct LockupArgs {
    pub unix_timestamp: Option<UnixTimestamp>,
    pub epoch: Option<Epoch>,
    pub custodian: Option<SolanaAddress>,
}

#[derive(Serialize, Deserialize, Debug, PartialEq, Eq, Clone)]
pub enum StakeInstruction {
    /// Initialize a stake with lockup and authorization information
    ///
    /// # Account references
    ///   0. `[WRITE]` Uninitialized stake account
    ///   1. `[]` Rent sysvar
    ///
    /// Authorized carries pubkeys that must sign staker transactions
    ///   and withdrawer transactions.
    /// Lockup carries information about withdrawal restrictions
    Initialize(Authorized, Lockup),

    /// Authorize a key to manage stake or withdrawal
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account to be updated
    ///   1. `[]` Clock sysvar
    ///   2. `[SIGNER]` The stake or withdraw authority
    ///   3. Optional: `[SIGNER]` Lockup authority, if updating StakeAuthorize::Withdrawer before
    ///      lockup expiration
    Authorize(SolanaAddress, StakeAuthorize),

    /// Delegate a stake to a particular vote account
    ///
    /// # Account references
    ///   0. `[WRITE]` Initialized stake account to be delegated
    ///   1. `[]` Vote account to which this stake will be delegated
    ///   2. `[]` Clock sysvar
    ///   3. `[]` Stake history sysvar that carries stake warmup/cooldown history
    ///   4. `[]` Address of config account that carries stake config
    ///   5. `[SIGNER]` Stake authority
    ///
    /// The entire balance of the staking account is staked.  DelegateStake
    ///   can be called multiple times, but re-delegation is delayed
    ///   by one epoch
    DelegateStake,

    /// Split u64 tokens and stake off a stake account into another stake account.
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account to be split; must be in the Initialized or Stake state
    ///   1. `[WRITE]` Uninitialized stake account that will take the split-off amount
    ///   2. `[SIGNER]` Stake authority
    Split(u64),

    /// Withdraw unstaked lamports from the stake account
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account from which to withdraw
    ///   1. `[WRITE]` Recipient account
    ///   2. `[]` Clock sysvar
    ///   3. `[]` Stake history sysvar that carries stake warmup/cooldown history
    ///   4. `[SIGNER]` Withdraw authority
    ///   5. Optional: `[SIGNER]` Lockup authority, if before lockup expiration
    ///
    /// The u64 is the portion of the stake account balance to be withdrawn,
    ///    must be `<= StakeAccount.lamports - staked_lamports`.
    Withdraw(u64),

    /// Deactivates the stake in the account
    ///
    /// # Account references
    ///   0. `[WRITE]` Delegated stake account
    ///   1. `[]` Clock sysvar
    ///   2. `[SIGNER]` Stake authority
    Deactivate,

    /// Set stake lockup
    ///
    /// If a lockup is not active, the withdraw authority may set a new lockup
    /// If a lockup is active, the lockup custodian may update the lockup parameters
    ///
    /// # Account references
    ///   0. `[WRITE]` Initialized stake account
    ///   1. `[SIGNER]` Lockup authority or withdraw authority
    SetLockup {
        unix_timestamp: Option<UnixTimestamp>,
        epoch: Option<Epoch>,
        custodian: Option<SolanaAddress>,
    },

    /// Merge two stake accounts.
    ///
    /// Both accounts must have identical lockup and authority keys. A merge
    /// is possible between two stakes in the following states with no additional
    /// conditions:
    ///
    /// * two deactivated stakes
    /// * an inactive stake into an activating stake during its activation epoch
    ///
    /// For the following cases, the voter pubkey and vote credits observed must match:
    ///
    /// * two activated stakes
    /// * two activating accounts that share an activation epoch, during the activation epoch
    ///
    /// All other combinations of stake states will fail to merge, including all
    /// "transient" states, where a stake is activating or deactivating with a
    /// non-zero effective stake.
    ///
    /// # Account references
    ///   0. `[WRITE]` Destination stake account for the merge
    ///   1. `[WRITE]` Source stake account for to merge.  This account will be drained
    ///   2. `[]` Clock sysvar
    ///   3. `[]` Stake history sysvar that carries stake warmup/cooldown history
    ///   4. `[SIGNER]` Stake authority
    Merge,

    /// Authorize a key to manage stake or withdrawal with a derived key
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account to be updated
    ///   1. `[SIGNER]` Base key of stake or withdraw authority
    ///   2. `[]` Clock sysvar
    ///   3. Optional: `[SIGNER]` Lockup authority, if updating StakeAuthorize::Withdrawer before
    ///      lockup expiration
    AuthorizeWithSeed {
        new_authorized_pubkey: SolanaAddress,
        stake_authorize: StakeAuthorize,
        authority_seed: String,
        authority_owner: SolanaAddress,
    },

    /// Initialize a stake with authorization information
    ///
    /// This instruction is similar to `Initialize` except that the withdraw authority
    /// must be a signer, and no lockup is applied to the account.
    ///
    /// # Account references
    ///   0. `[WRITE]` Uninitialized stake account
    ///   1. `[]` Rent sysvar
    ///   2. `[]` The stake authority
    ///   3. `[SIGNER]` The withdraw authority
    ///
    InitializeChecked,

    /// Authorize a key to manage stake or withdrawal
    ///
    /// This instruction behaves like `Authorize` with the additional requirement that the new
    /// stake or withdraw authority must also be a signer.
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account to be updated
    ///   1. `[]` Clock sysvar
    ///   2. `[SIGNER]` The stake or withdraw authority
    ///   3. `[SIGNER]` The new stake or withdraw authority
    ///   4. Optional: `[SIGNER]` Lockup authority, if updating StakeAuthorize::Withdrawer before
    ///      lockup expiration
    AuthorizeChecked(StakeAuthorize),

    /// Authorize a key to manage stake or withdrawal with a derived key
    ///
    /// This instruction behaves like `AuthorizeWithSeed` with the additional requirement that
    /// the new stake or withdraw authority must also be a signer.
    ///
    /// # Account references
    ///   0. `[WRITE]` Stake account to be updated
    ///   1. `[SIGNER]` Base key of stake or withdraw authority
    ///   2. `[]` Clock sysvar
    ///   3. `[SIGNER]` The new stake or withdraw authority
    ///   4. Optional: `[SIGNER]` Lockup authority, if updating StakeAuthorize::Withdrawer before
    ///      lockup expiration
    AuthorizeCheckedWithSeed {
        stake_authorize: StakeAuthorize,
        authority_seed: String,
        authority_owner: SolanaAddress,
    },

    /// Set stake lockup
    ///
    /// This instruction behaves like `SetLockup` with the additional requirement that
    /// the new lockup authority also be a signer.
    ///
    /// If a lockup is not active, the withdraw authority may set a new lockup
    /// If a lockup is active, the lockup custodian may update the lockup parameters
    ///
    /// # Account references
    ///   0. `[WRITE]` Initialized stake account
    ///   1. `[SIGNER]` Lockup authority or withdraw authority
    ///   2. Optional: `[SIGNER]` New lockup authority
    SetLockupChecked {
        unix_timestamp: Option<UnixTimestamp>,
        epoch: Option<Epoch>,
    },

    /// Get the minimum stake delegation, in lamports
    ///
    /// # Account references
    ///   None
    ///
    /// Returns the minimum delegation as a little-endian encoded u64 value.
    /// Programs can use the [`get_minimum_delegation()`] helper function to invoke and
    /// retrieve the return value for this instruction.
    ///
    /// [`get_minimum_delegation()`]: super::tools::get_minimum_delegation
    GetMinimumDelegation,

    /// Deactivate stake delegated to a vote account that has been delinquent for at least
    /// `MINIMUM_DELINQUENT_EPOCHS_FOR_DEACTIVATION` epochs.
    ///
    /// No signer is required for this instruction as it is a common good to deactivate abandoned
    /// stake.
    ///
    /// # Account references
    ///   0. `[WRITE]` Delegated stake account
    ///   1. `[]` Delinquent vote account for the delegated stake account
    ///   2. `[]` Reference vote account that has voted at least once in the last
    ///      `MINIMUM_DELINQUENT_EPOCHS_FOR_DEACTIVATION` epochs
    DeactivateDelinquent,

    /// Redelegate activated stake to another vote account.
    ///
    /// Upon success:
    ///   * the balance of the delegated stake account will be reduced to the undelegated amount in
    ///     the account (rent exempt minimum and any additional lamports not part of the delegation),
    ///     and scheduled for deactivation.
    ///   * the provided uninitialized stake account will receive the original balance of the
    ///     delegated stake account, minus the rent exempt minimum, and scheduled for activation to
    ///     the provided vote account. Any existing lamports in the uninitialized stake account
    ///     will also be included in the re-delegation.
    ///
    /// # Account references
    ///   0. `[WRITE]` Delegated stake account to be redelegated. The account must be fully
    ///      activated and carry a balance greater than or equal to the minimum delegation amount
    ///      plus rent exempt minimum
    ///   1. `[WRITE]` Uninitialized stake account that will hold the redelegated stake
    ///   2. `[]` Vote account to which this stake will be re-delegated
    ///   3. `[]` Address of config account that carries stake config
    ///   4. `[SIGNER]` Stake authority
    ///
    Redelegate,
}

pub struct DepositStakeArgs {
    pub sender: SolanaAddress,
    pub validator: SolanaAddress,
    pub stake_account: Option<SolanaAddress>,
    pub recent_blockhash: Blockhash,
    pub lamports: u64,
    pub space: u64,
}

pub struct StakeInstructionBuilder;

impl StakeInstructionBuilder {
    /// Creates an Initialize Stake instruction.
    pub fn stake_initialize(
        stake_pubkey: SolanaAddress,
        authorized: Authorized,
        lockup: Lockup,
    ) -> Instruction {
        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::Initialize(authorized, lockup),
            vec![
                AccountMeta::new(stake_pubkey, false),
                AccountMeta::readonly(*SYSVAR_RENT_ID_ADDRESS, false),
            ],
        )
    }

    pub fn withdraw(
        stake_pubkey: SolanaAddress,
        withdrawer_pubkey: SolanaAddress,
        to_pubkey: SolanaAddress,
        lamports: u64,
        custodian_pubkey: Option<SolanaAddress>,
    ) -> Instruction {
        let mut account_metas = vec![
            AccountMeta::new(stake_pubkey, false),
            AccountMeta::new(to_pubkey, false),
            AccountMeta::readonly(*SYSVAR_CLOCK_ID_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_STAKE_HISTORY_ID_ADDRESS, false),
            AccountMeta::readonly(withdrawer_pubkey, true),
        ];

        if let Some(custodian_pubkey) = custodian_pubkey {
            account_metas.push(AccountMeta::readonly(custodian_pubkey, true));
        }

        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::Withdraw(lamports),
            account_metas,
        )
    }

    pub fn delegate(
        stake_pubkey: SolanaAddress,
        vote_pubkey: SolanaAddress,
        authorized_pubkey: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(stake_pubkey, false),
            AccountMeta::readonly(vote_pubkey, false),
            AccountMeta::readonly(*SYSVAR_CLOCK_ID_ADDRESS, false),
            AccountMeta::readonly(*SYSVAR_STAKE_HISTORY_ID_ADDRESS, false),
            AccountMeta::readonly(*STAKE_CONFIG_ID_ADDRESS, false),
            AccountMeta::readonly(authorized_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::DelegateStake,
            account_metas,
        )
    }

    /// 0. `[WRITE]` Delegated stake account
    /// 1. `[]` Clock sysvar
    /// 2. `[SIGNER]` Stake authority
    pub fn deactivate(
        stake_pubkey: SolanaAddress,
        authorized_pubkey: SolanaAddress,
    ) -> Instruction {
        let account_metas = vec![
            AccountMeta::new(stake_pubkey, false),
            AccountMeta::readonly(*SYSVAR_CLOCK_ID_ADDRESS, false),
            AccountMeta::new(authorized_pubkey, true),
        ];
        Instruction::new_with_bincode(
            *STAKE_PROGRAM_ID_ADDRESS,
            StakeInstruction::Deactivate,
            account_metas,
        )
    }

    /// The function represents "stake delegation" operation that consists of several small instructions.
    pub fn deposit_stake(args: DepositStakeArgs) -> SigningResult<Vec<Instruction>> {
        let stake_addr = args.stake_account.unwrap_or_else(|| {
            // no stake address specified, generate a new unique
            StakeProgram::address_from_recent_blockhash(&args.sender, &args.recent_blockhash)
        });
        let seed = StakeProgram::recent_blockhash_as_seed(&args.recent_blockhash);

        let authorized = Authorized {
            staker: args.sender,
            withdrawer: args.sender,
        };
        let lockup = Lockup::default();

        Ok(vec![
            SystemInstructionBuilder::create_account_with_seed(
                args.sender,
                stake_addr,
                args.sender,
                seed,
                args.lamports,
                args.space,
            ),
            StakeInstructionBuilder::stake_initialize(stake_addr, authorized, lockup),
            StakeInstructionBuilder::delegate(stake_addr, args.validator, args.sender),
        ])
    }
}
