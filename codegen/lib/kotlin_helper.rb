# frozen_string_literal: true

module KotlinHelper
  # Transforms an interface name to a Java method name
  def self.format_name(name)
    return 'equals' if name == 'Equal'

    result = name
    match = /^([A-Z]+)/.match(name)
    result = name.sub(match[1], match[1].downcase) unless match.nil?

    result.sub(/_/, '')
  end

  def self.parameters(params)
    names = params.map do |param|
      name = fix_name(param.name)
      "#{name}: #{type(param.type)}"
    end
    names.join(', ')
  end

  def self.calling_parameters_ios(params)
    names = params.map do |param|
      name = fix_name(param.name)
        "#{name}#{convert_calling_type_ios(param.type)}"
    end
    names.join(', ')
  end

  def self.calling_parameters_android(params)
    names = params.map do |param|
      fix_name(param.name)
    end
    names.join(', ')
  end

  def self.calling_parameters_js(params)
    names = params.map do |param|
      name = fix_name(param.name)
        "#{name}#{convert_calling_type_js(param.type)}"
    end
    names.join(', ')
  end

  def self.fix_name(name)
    case name
    when ''
      "value"
    when 'val'
      "value"
    when 'return'
      '`return`'
    else
      name
    end
  end

  def self.convert_calling_type_ios(t)
    case t.name
    when :data
      "#{if t.is_nullable then '?' else '' end}.toTwData()"
    when :string
      "#{if t.is_nullable then '?' else '' end}.toTwString()"
    else
      if t.is_enum
        "#{if t.is_nullable then '?' else '' end}.value"
      elsif t.is_class
        "#{if t.is_nullable then '?' else '' end}.pointer"
      else
        ''
      end
    end
  end

  def self.convert_calling_type_js(t)
    case t.name
    when :data
      "#{if t.is_nullable then '?' else '' end}.toUInt8Array()"
    when :uint64
      ".toUInt()"
    when :int64
      ".toInt()"
    when :size
      ".toUInt()"
    else
      if t.is_enum
        "#{if t.is_nullable then '?' else '' end}._value"
      elsif t.is_class
        "#{if t.is_nullable then '?' else '' end}._value"
      else
        ''
      end
    end
  end

  def self.convert_calling_return_type_ios(t, expression = '')
    case t.name
    when :data
      "#{expression}.readTwBytes()#{if t.is_nullable then '' else '!!' end}"
    when :string
      "#{expression}.fromTwString()#{if t.is_nullable then '' else '!!' end}"
    else
      if t.is_enum
        "#{t.name}.fromValue(#{expression})#{if t.is_nullable then '' else '!!' end}"
      elsif t.is_class
        if t.is_nullable
          "#{expression}?.let { #{t.name}(it) }"
        else
          "#{t.name}(#{expression}!!)"
        end
      else
        expression
      end
    end
  end

  def self.convert_calling_return_type_js(t, expression = '')
    nullable = "#{if t.is_nullable then '?' else '' end}"
    case t.name
    when :void
      expression
    when :data
      "#{expression}.unsafeCast<UInt8Array#{nullable}>()#{nullable}.toByteArray()"
    when :int
      "#{expression}.unsafeCast<Number>().toInt()"
    when :uint8
      "#{expression}.unsafeCast<Number>().toByte().toUByte()"
    when :uint16
      "#{expression}.unsafeCast<Number>().toShort().toUShort()"
    when :uint32
      "#{expression}.unsafeCast<Number>().toInt().toUInt()"
    when :uint64
      "#{expression}.unsafeCast<Number>().toLong().toULong()"
    when :int8
      "#{expression}.unsafeCast<Number>().toByte()"
    when :int16
      "#{expression}.unsafeCast<Number>().toShort()"
    when :int32
      "#{expression}.unsafeCast<Number>().toInt()"
    when :int64
      "#{expression}.unsafeCast<Number>().toLong()"
    when :size
      "#{expression}.unsafeCast<Number>().toLong().toULong()"
    else
      if t.is_enum
        "#{t.name}.fromValue(#{expression})#{if t.is_nullable then '' else '!!' end}"
      elsif t.is_class
        if t.is_nullable
          "#{expression}.unsafeCast<Any?>()?.let { #{t.name}(it, Unit) }"
        else
          "#{t.name}(#{expression}, Unit)"
        end
      else
        "#{expression} as #{type(t)}"
      end
    end
  end

  def self.arguments(params)
    params.map do |param|
      param.name || 'value'
    end.join(', ')
  end

  def self.type(t)
    nullable = "#{if t.is_nullable then '?' else '' end}"
    case t.name
    when :void
      ""
    when :bool
      "Boolean#{nullable}"
    when :int
      "Int#{nullable}"
    when :uint8
      "UByte#{nullable}"
    when :uint16
      "UShort#{nullable}"
    when :uint32
      "UInt#{nullable}"
    when :uint64
      "ULong#{nullable}"
    when :int8
      "Byte#{nullable}"
    when :int16
      "Short#{nullable}"
    when :int32
      "Int#{nullable}"
    when :int64
      "Long#{nullable}"
    when :size
      "ULong#{nullable}"
    when :data
      "ByteArray#{nullable}"
    when :string
      "String#{nullable}"
    else
      "#{t.name}#{nullable}"
    end
  end

  def self.return_type(t)
    case t.name
    when :void
      ""
    else
      ": #{type(t)}"
    end
  end

end
