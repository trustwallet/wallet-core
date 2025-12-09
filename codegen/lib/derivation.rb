# frozen_string_literal: true

require 'file_editor'

$derivation_file = "include/TrustWalletCore/TWDerivation.h"
$derivation_file_rust = "rust/tw_coin_registry/src/tw_derivation.rs"

# Returns a derivation name if specified.
def derivation_name(deriv)
  return "" if deriv['name'].nil?
  deriv['name'].downcase
end

# Returns a string of `<Coin><Derivation>` if derivation's name is specified, otherwise returns `Default`.
def derivation_enum_name_no_prefix(deriv, coin)
  return "Default" if deriv['name'].nil?
  format_name(coin['name']) + camel_case(deriv['name'])
end

# Returns a string of `TWDerivation<Coin><Derivation>` if derivation's name is specified, otherwise returns `TWDerivationDefault`.
def derivation_enum_name(deriv, coin)
  return "TWDerivation" + derivation_enum_name_no_prefix(deriv, coin)
end

# Returns a derivation path.
def derivation_path(coin)
  coin['derivation'][0]['path']
end

# Get the last `TWDerivation` enum variant ID.
def get_last_derivation(file_path)
  last_derivation_id = nil

  File.open(file_path, "r") do |file|
    file.each_line do |line|
      # Match lines that define a TWDerivation enum value
      if line =~ /TWDerivation\w+\s*=\s*(\d+),/
        last_derivation_id = $1.to_i
      end
    end
  end

  last_derivation_id
end

# Returns whether the TWDerivation enum contains the given `derivation` variant.
def find_derivation(file_path, derivation)
  File.open(file_path, "r") do |file|
    file.each_line do |line|
      return true if line.include?(derivation)
    end
  end
  return false
end

# Insert a new `TWDerivation<X> = N,` to the end of the enum.
def insert_derivation(file_path, derivation, derivation_id)
  target_line = "    #{derivation} = #{derivation_id},"
  insert_target_line(file_path, target_line, "    // end_of_derivation_enum - USED TO GENERATE CODE\n")
end

# Update TWDerivation enum variants if new derivation appeared.
def update_derivation_enum(coins)
  coins.each do |coin|
    coin['derivation'].each_with_index do |deriv, index|
      deriv_name = derivation_enum_name(deriv, coin)
      if !find_derivation($derivation_file, deriv_name)
        new_derivation_id = get_last_derivation($derivation_file) + 1
        insert_derivation($derivation_file, deriv_name, new_derivation_id)

        rust_deriv_name = derivation_enum_name_no_prefix(deriv, coin)
        insert_derivation($derivation_file_rust, rust_deriv_name, new_derivation_id)
      end
    end
  end
end
