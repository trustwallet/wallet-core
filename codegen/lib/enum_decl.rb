# frozen_string_literal: true

# Enum declaration.
class EnumDecl
  attr_reader :name, :comment
  attr_accessor :cases, :raw_type
  attr_accessor :methods, :properties, :static_methods, :static_properties

  def initialize(name:, raw_type:, comment:)
    @name = name
    @cases = []
    @raw_type = raw_type
    @methods = []
    @properties = []
    @static_methods = []
    @static_properties = []
    @comment = comment
  end

  def struct?
    false
  end

  def class?
    false
  end

  def enum?
    true
  end

  def type
    TypeDecl.new(name: @name, is_enum: true)
  end
end

# Enum case declaration.
class EnumCaseDecl
  attr_reader :name
  attr_accessor :enum, :value, :string

  def initialize(name:, enum:, value: nil, string: nil)
    @name = name
    @enum = enum
    @value = value
    @string = string
  end
end
