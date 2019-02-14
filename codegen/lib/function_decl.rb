# frozen_string_literal: true

# Function or method declaration
class FunctionDecl
  attr_reader :name, :entity
  attr_accessor :is_method, :return_type, :parameters, :static

  def initialize(name:, entity:, is_method:, return_type: :void, parameters: [], static: false)
    @name = name
    @entity = entity
    @is_method = is_method
    @return_type = return_type
    @parameters = parameters
    @static = static
  end
end

# Function parameter
class Parameter
  attr_reader :name, :type

  def initialize(name:, type:)
    @name = name
    @type = type
  end
end
