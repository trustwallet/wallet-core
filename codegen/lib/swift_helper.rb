# frozen_string_literal: true

module SwiftHelper
  # Transforms an interface name to a Swift method or property name
  def self.format_name(n)
    return '< ' if n == 'Less'
    return '== ' if n == 'Equal'

    prefix = /^([A-Z]+)/.match(n)[1]
    return n if prefix.nil?
    n.sub(prefix, prefix.downcase)
  end

  def self.parameters(params)
    names = params.map do |param|
      "#{param.name || '_ value'}: #{type(param.type)}"
    end
    names.join(', ')
  end

  def self.arguments(params)
    params.map do |param|
      if param.type.name == :data
        (param.name || 'value') + 'Data'
      elsif param.type.name == :string
        (param.name || 'value') + 'String'
      elsif param.type.is_struct || param.type.is_class
        (param.name || 'value') + (param.type.is_nullable ? '?' : '') + '.rawValue'
      elsif param.type.is_proto
        (param.name || 'value') + 'Data'
      elsif param.type.name == :int
        "Int32(#{param.name || 'value'})"
      elsif param.type.is_enum
        "TW#{param.type.name}(rawValue: #{param.name || 'value'}.rawValue)"
      else
        param.name || 'value'
      end
    end
  end

  def self.type(t)
    case t.name
    when :void
      'Void'
    when :bool
      'Bool'
    when :int
      'Int32'
    when :uint8
      'UInt8'
    when :uint16
      'UInt16'
    when :uint32
      'UInt32'
    when :uint64
      'UInt64'
    when :size
      'Int'
    when :data
      if t.is_nullable
        'Data?'
      else
        'Data'
      end
    when :string
      if t.is_nullable
        'String?'
      else
        'String'
      end
    else
      if t.is_proto
        t.name
      elsif t.is_nullable
        t.name + '?'
      else
        t.name
      end
    end
  end
end
