# frozen_string_literal: true

require 'erb'
require 'fileutils'
require 'json'

require 'entity_decl'
require 'code_generator'
require 'coin_test_gen'

# Coin template generation

$flag_comment = " // TODO remove if the blockchain already exists, or just remove this comment if not"

# Transforms a coin name to a C++ name
def self.format_name(coin)
    formatted = coin['name']
    formatted = formatted.gsub(/-/, ' ')
    formatted = formatted.gsub(/\./, ' ')
    formatted = formatted.gsub(/\s/, '')
    formatted
end

def self.format_name_lowercase(coin)
format_name(coin).downcase
end

def self.format_name_uppercase(coin)
format_name(coin).upcase
end

def self.generate_file(templateFile, folder, fileName, coin)
    @coin = coin
    name = format_name(coin)
    path = File.expand_path(templateFile, File.join(File.dirname(__FILE__), '..', 'lib', 'templates'))
    template = ERB.new(File.read(path), nil, '-')
    result = template.result(binding)

    FileUtils.mkdir_p folder
    path = File.join(folder, fileName)
    File.write(path, result)
    puts "Generated file " + path
end

def self.insert_coin_type(coin, mode)
    target_file = "include/TrustWalletCore/TWCoinType.h"
    target_line = "    TWCoinType#{format_name(coin)} = #{coin['coinId']},\n"
    if insert_target_line(target_file, target_line, "};\n")
        if (mode != "evm")
            insert_blockchain_type(coin)
        end
    end
end

def insert_blockchain_type(coin)
    target_file = "include/TrustWalletCore/TWBlockchain.h"
    line_number = File.readlines(target_file).count + 2 # add offset because of removed blockchain enum type
    target_line = "    TWBlockchain#{coin['blockchain']} = #{line_number - 17}, " + $flag_comment + "\n"
    insert_target_line(target_file, target_line, "};\n")
end

def insert_coin_entry(coin)
    target_file = "src/Coin.cpp"
    entryName = coin['blockchain']
    target_line = "#include \"#{entryName}/Entry.h\"" + $flag_comment + "\n"
    insert_target_line(target_file, target_line, "// end_of_coin_includes_marker_do_not_modify\n")
    target_line = "#{entryName}::Entry #{entryName}DP;" + $flag_comment + "\n"
    insert_target_line(target_file, target_line, "// end_of_coin_dipatcher_declarations_marker_do_not_modify\n")
    target_line = "        case TWBlockchain#{entryName}: entry = &#{entryName}DP; break;" + $flag_comment + "\n"
    insert_target_line(target_file, target_line, "        // end_of_coin_dipatcher_switch_marker_do_not_modify\n")
end

def self.insert_target_line(target_file, target_line, original_line)
    lines = File.readlines(target_file)
    index = lines.index(target_line)
    if !index.nil?
        puts "Line is already present, file: #{target_file}  line: #{target_line}"
        return true
    end
    index = lines.index(original_line)
    if index.nil?
        puts "WARNING: Could not find line! file: #{target_file}  line: #{original_line}"
        return false
    end
    lines.insert(index, target_line)
    File.open(target_file, "w+") do |f|
        f.puts(lines)
    end
    puts "Updated file: #{target_file}  new line: #{target_line}"
    return true
end

def generate_skeleton(coin_id, mode)
    puts "New coin template for coin '#{coin_id}' #{mode} requested"

    json_string = File.read('registry.json')
    coins = JSON.parse(json_string).sort_by { |x| x['name'] }

    entity = EntityDecl.new(name: "New" + coin_id, is_struct: false, comment: '')
    file = "new"+ coin_id

    generator = CodeGenerator.new(entities: [entity], files: [file], output_folder: ".")

    @coins = coins

    coin_test_gen = CoinTestGen.new()

    # Find coin in list of coins, by Id
    coinSelect = coins.select {|c| c['id'] == coin_id}
    if coinSelect.length() == 0
        puts "Error: coin #{coin_id} not found!"
        return
    end
    coin = coinSelect.first
    name = format_name(coin)


    insert_coin_type(coin, mode)
    if (mode != "evm")
        insert_coin_entry(coin)

        generate_file("newcoin/Address.h.erb", "src/#{name}", "Address.h", coin)
        generate_file("newcoin/Address.cpp.erb", "src/#{name}", "Address.cpp", coin)
        generate_file("newcoin/Entry.h.erb", "src/#{name}", "Entry.h", coin)
        generate_file("newcoin/Entry.cpp.erb", "src/#{name}", "Entry.cpp", coin)
        generate_file("newcoin/Proto.erb", "src/proto", "#{name}.proto", coin)
        generate_file("newcoin/Signer.h.erb", "src/#{name}", "Signer.h", coin)
        generate_file("newcoin/Signer.cpp.erb", "src/#{name}", "Signer.cpp", coin)

        generate_file("newcoin/AddressTests.cpp.erb", "tests/chains/#{name}", "AddressTests.cpp", coin)
        generate_file("newcoin/SignerTests.cpp.erb", "tests/chains/#{name}", "SignerTests.cpp", coin)
        generate_file("newcoin/TransactionCompilerTests.cpp.erb", "tests/chains/#{name}", "TransactionCompilerTests.cpp", coin)
        generate_file("newcoin/TWAddressTests.cpp.erb", "tests/chains/#{name}", "TWAnyAddressTests.cpp", coin)
        generate_file("newcoin/TWSignerTests.cpp.erb", "tests/chains/#{name}", "TWAnySignerTests.cpp", coin)
        generate_file("newcoin/AddressTests.kt.erb", "android/app/src/androidTest/java/com/trustwallet/core/app/blockchains/#{format_name_lowercase(coin)}", "Test#{name}Address.kt", coin)
        generate_file("newcoin/SignerTests.kt.erb", "android/app/src/androidTest/java/com/trustwallet/core/app/blockchains/#{format_name_lowercase(coin)}", "Test#{name}Signer.kt", coin)
        generate_file("newcoin/Tests.swift.erb", "swift/Tests/Blockchains", "#{name}Tests.swift", coin)
    end

    coin_test_gen.generate_coin_test_file(coin, 'TWCoinTypeTests.cpp.erb', true)

    puts "please tools/generate-files to generate Swift/Java/Protobuf files"
end
