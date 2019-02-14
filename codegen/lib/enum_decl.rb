# frozen_string_literal: true

# Enum declaration.
class EnumDecl
  attr_reader :name
  attr_accessor :cases, :type

  def initialize(name:, type:)
    @name = name
    @cases = []
    @type = type
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
