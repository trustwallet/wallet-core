# frozen_string_literal: true

# Type declaration
class TypeDecl
  attr_reader :name
  attr_accessor :is_class, :is_struct, :is_enum, :is_proto, :is_nullable, :is_inout, :size

  def initialize(name:, **options)
    @name = name
    @is_class = options.fetch(:is_class, false)
    @is_struct = options.fetch(:is_struct, false)
    @is_enum = options.fetch(:is_enum, false)
    @is_proto = options.fetch(:is_proto, false)
    @is_nullable = options.fetch(:is_nullable, false)
    @is_inout = options.fetch(:is_inout, false)
    @size = options.fetch(:size, nil)
  end

  def self.fromPrimitive(string)
    case string
    when 'void'
      TypeDecl.new(name: :void)
    when 'bool'
      TypeDecl.new(name: :bool)
    when 'int'
      TypeDecl.new(name: :int)
    when 'size_t'
      TypeDecl.new(name: :size)
    when 'uint8_t'
      TypeDecl.new(name: :uint8)
    when 'uint16_t'
      TypeDecl.new(name: :uint16)
    when 'uint32_t'
      TypeDecl.new(name: :uint32)
    when 'uint64_t'
      TypeDecl.new(name: :uint64)
    else
      nil
    end
  end
end
