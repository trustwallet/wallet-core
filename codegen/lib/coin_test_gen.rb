# Helper for creating/updating CoinType unit tests, based on the registry.json file

require 'erb'
require 'fileutils'
require 'json'

class CoinTestGen
  attr_accessor :templateFile

  def initialize()
    @templateFile = 'TWCoinTypeTests.cpp.erb'
  end

  # Transforms a coin name to a C++ name
  def format_name(n)
    formatted = n

    # Remove whitespaces
    formatted.gsub!(/\s+/, '')

    formatted
  end

  # Transforms number to hex
  def to_hex(i)
    hex = i.to_i().to_s(16)
    hex
  end

  # Display name, or name if not specified
  def display_name(coin)
    name = coin['displayName']
    if name == nil
      name = coin['name']
    end
    name
  end

  # Explorer urls
  def explorer_tx_url(c)
    path = c['explorer']['url'].to_s + c['explorer']['txPath'].to_s
  end
  def explorer_account_url(c)
    path = c['explorer']['url'].to_s + c['explorer']['accountPath'].to_s
  end
  def explorer_sample_tx(c)
    if c['explorer']['sampleTx'].nil?
      "t123"
    else
      c['explorer']['sampleTx']
    end
  end
  def explorer_sample_account(c)
    if c['explorer']['sampleAccount'].nil?
      "a12"
    else
      c['explorer']['sampleAccount']
    end
  end

  def generate_coin_test_file(coin, templateFile, overwriteExisting = true)
    path = File.expand_path(templateFile, File.join(File.dirname(__FILE__), '..', 'lib', 'templates'))
    template = ERB.new(File.read(path), nil, '-')
    result = template.result(binding)

    folder = 'tests/chains/'
    if coin.key?('testFolderName')
      folder += format_name(coin['testFolderName'])
    else
      folder += format_name(coin['name'])
    end

    file = 'TWCoinTypeTests.cpp'
    FileUtils.mkdir_p folder
    path = File.join(folder, file)
    if not File.exist?(path) or overwriteExisting
      File.write(path, result)
      puts "Generated file " + path
    end
  end
end
