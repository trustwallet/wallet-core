# frozen_string_literal: true

module NapiHelper
  # Transforms an interface name to a Napi method name
  def self.format_name(name)
    return 'equals' if name == 'Equal'

    result = name
    match = /^([A-Z]+)/.match(name)
    result = name.sub(match[1], match[1].downcase) unless match.nil?

    result.sub(/_/, '')
  end

  # Transforms a method/property name to a Napi function name
  def self.function_name(entity:, function:)
    "Js_wallet_core_napi_#{entity.name}_#{format_name(function.name)}"
  end

  def self.parameters(params)
    names = params.map do |param|
      ", #{type(param.type)} #{param.name || 'value'}"
    end
    names.join('')
  end

  def self.arguments(params)
    params.map do |param|
      if param.type.is_class
        (param.name || 'value') + 'Instance'
      elsif param.type.is_struct
        '*' + (param.name || 'value') + 'Instance'
      elsif param.type.name == :data
        (param.name || 'value') + 'Data'
      elsif param.type.name == :string
        (param.name || 'value') + 'String'
      elsif param.type.is_enum
        (param.name || 'value') + 'Value'
      elsif param.type.is_proto
        (param.name || 'value') + 'Data'
      else
        param.name || 'value'
      end
    end
  end

  def self.primitive_type(t)
    case t.name
    when :bool
      'bool'
    when :int
      'int'
    when :uint8
      'uint8_t'
    when :size
      'size_t'
    when :uint16
      'uint16_t'
    when :uint32
      'uint32_t'
    when :uint64
      'uint64_t'
    when :string
      'string'
    else
      raise "Invalid type #{t.name}"
    end
  end

  def self.type(t)
    case t.name
    when :void
      'void'
    when :bool
      'Boolean'
    when :int, :uint8, :uint16, :uint32
      'Number'
    when :data
      'Uint8Array'
    when 'Data'
      'Uint8Array'
    when :string, :size, :uint64
      'String'
    else
      if t.is_class || t.is_struct
        'Value'
      elsif t.is_enum
        'Number'
      elsif t.is_proto
        'Uint8Array'
      else
        raise "Invalid type #{t.name}"
      end
    end
  end

  def self.compareMethod(entity)
    FunctionDecl.new(
      name: 'compareTo',
      entity: entity,
      is_method: true,
      return_type: TypeDecl.new(name: :int),
      parameters: [Parameter.new(name: 'thisObject', type: entity.type), Parameter.new(name: 'other', type: entity.type)],
      static: false)
  end
end
