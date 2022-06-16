# frozen_string_literal: true

module WasmCppHelper
    # Transforms an interface name to a cpp method name
    def self.format_name(name)
      result = name
      match = /^([A-Z]+)/.match(name)
      result = name.sub(match[1], match[1].downcase) unless match.nil?
      result
    end

    # Transforms a method/property name to a cpp function name

    def self.class_name(entity:)
      "Wasm" + entity.name
    end

    def self.function_name(entity:, function:)
      "#{format_name(function.name)}"
    end

    def self.parameters(params)
      names = params.map do |param|
        "#{type(param.type)} #{param.name}"
      end
      names.join(', ')
    end

    def self.arguments(params)
      params.map do |param|
        if param.type.name == :data
          "&#{param.name}Data"
        elsif param.type.name == :string
          '&' + param.name
        elsif param.type.is_struct || param.type.is_class
          param.name + '->instance'
        else
          param.name
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
        'std::string'
      else
        raise "Invalid type #{t.name}"
      end
    end

    def self.type(t)
      case t.name
      when :void
        'void'
      when :bool
        'bool'
      when :int
        'int'
      when :uint8
        'uint8_t'
      when :uint16
        'uint16_t'
      when :uint32
        'uint32_t'
      when :uint64
        'uint64_t'
      when :int8
        'int8_t'
      when :int16
        'int16_t'
      when :int32
        'int32_t'
      when :int64
        'int64_t'
      when :size
        'size_t'
      when :data
          'const std::string&'
      when :string
          'const std::string&'
      else
        if t.is_enum
          "TW#{t.name}"
        elsif t.is_struct || t.is_class
          "Wasm#{t.name}*"
        else
          t.name
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
