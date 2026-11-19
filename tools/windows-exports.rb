#!/usr/bin/env ruby
# SPDX-License-Identifier: Apache-2.0

require 'fileutils'
require 'stringio'
$LOAD_PATH.unshift(File.expand_path('../codegen/lib', __dir__))
require 'parser'

abort 'usage: windows-exports.rb PUBLIC_HEADER_DIRECTORY OUTPUT.def' unless ARGV.length == 2
ARGV.map! { |path| path.tr('\\', '/') }
names = []
Dir.glob(File.join(ARGV[0], '*.h')).sort.each do |path|
  # Reuse the bindings parser for annotated methods and properties.
  output = $stdout
  begin
    $stdout = StringIO.new
    entity = Parser.new(path: path).parse
  ensure
    $stdout = output
  end
  if entity
    [:methods, :properties, :static_methods, :static_properties].each do |group|
      entity.public_send(group).each { |function| names << "TW#{entity.name}#{function.name}" }
    end
  end
  # Plain C functions are not annotated for bindings.
  declarations = File.read(path).gsub(%r{/\*.*?\*/|//[^\n]*}m, '')
  names.concat(declarations.scan(/\b(TW[A-Z][A-Za-z0-9_]+|hrpForString|stringForHRP)\s*\([^;{}]*\)\s*(?:TW_VISIBILITY_DEFAULT)?\s*;/m).flatten)
end
names = names.uniq.sort
abort 'No public C API declarations found' if names.empty?
FileUtils.mkdir_p(File.dirname(ARGV[1]))
File.write(ARGV[1], "LIBRARY TrustWalletCore\nEXPORTS\n" + names.map { |name| "    #{name}\n" }.join)
puts "Windows public C API: #{names.length} exports"
