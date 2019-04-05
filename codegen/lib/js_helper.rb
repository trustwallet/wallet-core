# frozen_string_literal: true
require 'set'

module JsHelper
  # Transforms an interface name to a Js method name
  def self.format_name(name)
    return 'equals' if name == 'Equal'

    result = name
    match = /^([A-Z]+)/.match(name)
    result = name.sub(match[1], match[1].downcase) unless match.nil?

    result.sub(/_/, '')
  end

  # Transforms a proto name name to a JS class name
  def self.proto_to_class(name) # TODO
    ''
  end

  # Transform string to single quotes
  def self.to_single_quotes(name)
    "'#{name[1..-2]}'"
  end

  def self.import_types_from_entity(entity)
    unique_types = []
    methods = entity.properties + entity.methods
    methods.each do |method|
      if !method.parameters.empty? && !self.is_primitive_type(method.return_type)
        unique_types |= [method.return_type.name]
      end
      method.parameters.drop(1).map do |param|
        if !self.is_primitive_type(param.type)
          unique_types |= [param.type.name]
        end
      end
    end
    unique_types
  end


  # Create import statement given type
  def self.str_to_import(t)
      "import { #{t} } from './#{t}'"
  end

  # Transforms an interface name to a Js constant name
  def self.format_constant(name)
    name.upcase
  end

  def self.parameters(params)
    names = params.map do |param|
      "#{param.name || 'value'}: #{type(param.type)}"
    end
    names.join(', ')
  end

  def self.arguments(params)
    params.map do |param|
      param.name || 'value'
    end.join(', ')
  end

  def self.is_primitive_type(t)
    case t.name
    when :void, :bool, :int, :uint8, :uint16, :uint32, :size, :uint64, :data, 'Data', :string
      true
    else
      false
    end
  end

  def self.type(t)
    case t.name
    when :void
      'void'
    when :bool
      'boolean'
    when :int, :uint8, :uint16, :uint32, :size
      'number'
    when :uint64
      'long'
    when :data
      'Uint8Array'
    when 'Data'
      'Uint8Array'
    when :string
      'string'
    else
      if t.is_proto
        proto_to_class(t.name)
      else
        t.name
      end
    end
  end
end
