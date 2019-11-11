# Helper for creating/updating CoinType unit tests, based on the coins.json file

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
    #formatted = formatted.sub(/^([a-z]+)/, &:upcase)
    formatted = formatted.sub(/\s/, '')
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

  def self.coin_type(path)
    path.split('/')[2].chomp("'")
  end

  def generate_coin_test_file(coin)
    path = File.expand_path(templateFile, File.join(File.dirname(__FILE__), '..', 'lib', 'templates'))
    template = ERB.new(File.read(path), nil, '-')
    result = template.result(binding)

    folder = 'tests/' + format_name(coin['name'])
    file = 'TWCoinTypeTests.cpp'
    FileUtils.mkdir_p folder
    path = File.join(folder, file)
    File.write(path, result)
    puts "Generated file " + path
  end
end
