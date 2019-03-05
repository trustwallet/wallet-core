# frozen_string_literal: true

module JNIHelper
  # Transforms an interface name to a JNI method name
  def self.format_name(name)
    return 'equals' if name == 'Equal'

    result = name
    match = /^([A-Z]+)/.match(name)
    result = name.sub(match[1], match[1].downcase) unless match.nil?

    result.sub(/_/, '')
  end

  # Transforms a method/property name to a JNI function name
  def self.function_name(entity:, function:, native_prefix: false)
    if native_prefix
      "Java_wallet_core_jni_#{entity.name}_native#{function.name}"
    else
      "Java_wallet_core_jni_#{entity.name}_#{format_name(function.name)}"
    end
  end

  # Transforms a proto name name to a JNI class name
  def self.proto_to_class(name)
    parts = name.split('_')
    return nil if parts.count < 3 || parts[0] != 'TW'

    if parts.count == 3
      "wallet/core/jni/proto/Common$#{parts.last}"
    else
      "wallet/core/jni/proto/#{parts[1]}$#{parts[3]}"
    end
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

  def self.type(t)
    case t.name
    when :void
      'void'
    when :bool
      'jboolean'
    when :int
      'jint'
    when :uint8
      'jchar'
    when :uint16
      'jshort'
    when :uint32
      'jint'
    when :uint64
      'jlong'
    when :size
      'jsize'
    when :data
      'jbyteArray'
    when 'Data'
      'jbyteArray'
    when :string
      'jstring'
    else
      if t.is_class || t.is_struct
        'jobject'
      elsif t.is_enum
        'jobject'
      elsif t.is_proto
        'jobject'
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
