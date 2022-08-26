# frozen_string_literal: true

# Function or method declaration
class FunctionDecl
  attr_reader :name, :entity
  attr_accessor :is_method, :return_type, :parameters, :static, :discardable_result
  attr_accessor :comment, :comment_with_indent

  def initialize(name:, entity:, is_method:, return_type: :void, parameters: [], static: false, discardable_result: false, comment: '')
    @name = name
    @entity = entity
    @is_method = is_method
    @return_type = return_type
    @parameters = parameters
    @static = static
    @discardable_result = discardable_result
    @comment = comment
    @comment_with_indent = comment.to_s.gsub('///', '    ///')
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
