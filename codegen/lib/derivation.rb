# frozen_string_literal: true

require 'file_editor'

$derivation_file = "include/TrustWalletCore/TWDerivation.h"

# Returns a derivation name if specified.
def derivation_name(deriv)
  return "" if deriv['name'].nil?
  deriv['name'].downcase
end

# Returns a derivation path.
def derivation_path(coin)
  coin['derivation'][0]['path']
end

# Returns a `TWDerivation<X>` if derivation's name is specified, otherwise returns `TWDerivationDefault`
def derivation_enum_name(deriv, coin)
  return "TWDerivationDefault" if deriv['name'].nil?
  "TWDerivation" + format_name(coin['name']) + camel_case(deriv['name'])
end

# Get the last `TWDerivation` enum variant ID.
def get_last_derivation()
  last_derivation_id = nil

  File.open($derivation_file, "r") do |file|
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
def find_derivation(derivation)
  File.open($derivation_file, "r") do |file|
    file.each_line do |line|
      return true if line.include?(derivation)
    end
  end
  return false
end

# Insert a new `TWDerivation<X> = N,` to the end of the enum.
def insert_derivation(derivation)
  new_derivation_id = get_last_derivation() + 1
  target_line = "    #{derivation} = #{new_derivation_id},"
  insert_target_line($derivation_file, target_line, "};\n")
end

# Update TWDerivation enum variants if new derivation appeared.
def update_derivation_enum(coins)
  coins.each do |coin|
    coin['derivation'].each_with_index do |deriv, index|
      deriv_name = derivation_enum_name(deriv, coin)
      if !find_derivation(deriv_name)
        insert_derivation(deriv_name)
      end
    end
  end
end
