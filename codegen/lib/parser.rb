# frozen_string_literal: true

require 'strscan'

require 'entity_decl'
require 'enum_decl'
require 'function_decl'
require 'type_decl'

# C header parser
class Parser
  attr_reader :path, :entity, :entity_comment

  def initialize(path:, string: nil)
    @path = path
    @buffer = StringScanner.new(string || File.read(path))
    @entity = nil
  end

  # Parses a C header file for class/struct declarations
  def parse
    clear_comment
    until @buffer.eos?
      @buffer.skip(/\s*/)

      if !@buffer.scan(/\/\//).nil?
        @entity_comment = @entity_comment + '//' + @buffer.scan_until(/(\r\n|\r|\n)/)
        next
      end

      if !@buffer.scan(/TW_EXTERN_C_BEGIN/).nil?
        # This is to ignore very first comments from the file
        clear_comment
        next
      end
      
      @entity_comment = @entity_comment.strip

      # Look for TW_EXPORT statements
      if !@buffer.scan(/TW_EXPORT_[A-Z_]+/).nil?
        # Handle statements
        case @buffer[0]
        when 'TW_EXPORT_CLASS'
          handle_class
        when 'TW_EXPORT_STRUCT'
          handle_struct
        when 'TW_EXPORT_ENUM'
          handle_enum
        when 'TW_EXPORT_FUNC'
          handle_func
        when 'TW_EXPORT_METHOD'
          handle_method
        when 'TW_EXPORT_PROPERTY'
          handle_property
        when 'TW_EXPORT_STATIC_METHOD'
          handle_static_method
        when 'TW_EXPORT_STATIC_PROPERTY'
          handle_static_property
        end

        clear_comment
      end

      break if @buffer.skip_until(/\n/).nil?
    end

    @entity
  end

  # Parses a type.
  def parse_type
    @buffer.skip(/\s*/)
    if @buffer.scan(/(const )?struct TW(\w+)\s?\*\s?(_Nullable|_Nonnull)/)
      return TypeDecl.new(name: @buffer[2], is_class: true, is_nullable: @buffer[3] == '_Nullable', is_inout: @buffer[1].nil?)
    elsif @buffer.scan(/(const )?struct TW(\w+)/)
      return TypeDecl.new(name: @buffer[2], is_struct: true)
    elsif @buffer.scan(/TWData\s?\*\s?(_Nullable|_Nonnull)/)
      return TypeDecl.new(name: :data, is_nullable: @buffer[1] == '_Nullable', is_inout: false)
    elsif @buffer.scan(/TWString\s?\*\s?(_Nullable|_Nonnull)/)
      return TypeDecl.new(name: :string, is_nullable: @buffer[1] == '_Nullable', is_inout: false)
    elsif @buffer.scan(/enum TW(\w+)/)
      return TypeDecl.new(name: @buffer[1], is_enum: true)
    elsif @buffer.scan(/TW_(\w+)/)
      return TypeDecl.new(name: @buffer[0], is_proto: true)
    elsif @buffer.scan(/(\w+)/)
      type = TypeDecl.fromPrimitive(@buffer[1])
      report_error "Invalid primitive type '#{@buffer[1]}'" if type.nil?
      return type
    else
      report_error "Invalid type #{@buffer.scan(/\w+/)}"
    end
  end

  # Parses a function prototype.
  def parse_func
    return_type = parse_type
    @buffer.skip(/\s*/)
    scan_or_fail(/\w+/, 'Invalid function name')

    func = FunctionDecl.new(name: @buffer[0], entity: @entity, is_method: true, return_type: return_type, comment: @entity_comment)
    @buffer.skip(/\s*/)

    scan_or_fail(/\(/, 'Invalid function declaration. Expected (')
    until @buffer.scan(/\)/)
      type = parse_type
      @buffer.skip(/\s*/)

      # Special case for `x f(void)`` style prototypes
      break if type.name == :void && @buffer.scan(/\)/)

      name = ''
      if type.name == :data && !type.size.nil?
        name = 'result'
      elsif type.name == :string && !type.size.nil?
        name = 'result'
      else
        name = @buffer.scan(/\w+/)
      end
      param = Parameter.new(name: name, type: type)
      func.parameters << param
      @buffer.skip(/\s*,\s*/)
    end

    @buffer.skip(/;/)
    func
  end

  def parse_discardable_result
    @buffer.skip(/\s*/)
    @buffer.scan(/TW_METHOD_DISCARDABLE_RESULT(\s*)/) != nil
  end

  def handle_class
    @buffer.skip(/\s*/)
    report_error 'Invalid type name' if @buffer.scan(/struct TW(\w+)\s*;/).nil?
    report_error 'Found more than one class/struct in the same file' unless @entity.nil?
    @entity = EntityDecl.new(name: @buffer[1], is_struct: false, comment: @entity_comment)
    puts "Found a class #{@entity.name}"
  end

  def handle_struct
    @buffer.skip(/\s*/)
    report_error 'Invalid type name at' if @buffer.scan(/struct TW(\w+)\s*\{?/).nil?
    report_error 'Found more than one class/struct in the same file' unless @entity.nil?
    @entity = EntityDecl.new(name: @buffer[1], is_struct: true, comment: @entity_comment)
    puts "Found a struct #{@buffer[1]}"
  end

  def handle_enum
    @buffer.skip(/\s*/)
    report_error 'Invalid enum type name' if @buffer.scan(/\((\w*)\)/).nil?
    type = @buffer[1]

    @buffer.skip(/\s*/)
    report_error 'Invalid enum' if @buffer.scan(/enum TW(\w+)\s*\{/).nil?
    @entity = EnumDecl.new(name: @buffer[1], raw_type: TypeDecl.fromPrimitive(type), comment: @entity_comment)
    incremental_value = 0

    until @buffer.eos?
      break if @buffer.skip_until(/\n/).nil?

      # Look for end of declaration
      @buffer.skip(/\s*/)
      break if @buffer.scan(/\}/)

      # Look for case statements
      if @buffer.scan(%r{TW#{@entity.name}(\w+)\s*\/\*\s*(".*")\s*\*\/,})
        case_decl = EnumCaseDecl.new(name: @buffer[1], enum: @entity, value: incremental_value, string: @buffer[2])
        incremental_value += 1
        @entity.cases << case_decl
      elsif @buffer.scan(/TW#{@entity.name}(\w+)\s*(=\s*(\w+))?\s*,/)
        case_decl = EnumCaseDecl.new(name: @buffer[1], enum: @entity, value: @buffer[3])
        v = Integer(@buffer[3])
        incremental_value = v.to_i + 1 unless v.nil?
        @entity.cases << case_decl
      else
        next
      end
    end

    puts "Found an enum #{@entity.name}"
  end

  def handle_func
    report_error 'Free functions not supported'
  end

  def handle_method
    report_error 'Method found before class/struct definition' if @entity.nil?
    discardable = parse_discardable_result
    method = parse_func
    method.discardable_result = discardable

    # Remove prefix
    unless method.name.start_with? "TW#{@entity.name}"
      report_error 'Method name needs to start with class/struct name'
    end
    method.name.slice! "TW#{@entity.name}"

    # Check first parameter
    if method.parameters.count.zero? || @entity.name != method.parameters.first.type.name
      report_error 'First parameter on a method needs to be the struct or class the method belongs to'
    end
    if @entity.struct? && method.parameters.first.type.is_class
      report_error 'First parameter on a struct method needs to be the struct'
    end
    if @entity.class? && !method.parameters.first.type.is_class
      report_error 'First parameter on a class method needs to be the class'
    end
    if @entity.enum? && !method.parameters.first.type.is_enum
      report_error 'Only parameter on a enum method needs to be the enum'
    end

    @entity.methods << method
  end

  def handle_property
    report_error 'Method found before class/struct definition' if @entity.nil?
    method = parse_func

    # Remove prefix
    unless method.name.start_with? "TW#{@entity.name}"
      report_error 'Method name needs to start with class/struct name'
    end
    method.name.slice! "TW#{@entity.name}"

    # Check first parameter
    if method.parameters.count < 1 || @entity.name != method.parameters.first.type.name
      report_error 'Only parameter on a property needs to be the struct or class the property belongs to'
    end
    if method.parameters.count == 2 && method.parameters[1].type.name != :data
      report_error "A property's second parameter can only be result data"
    end
    if method.parameters.count > 2
      report_error 'Only parameter on a property needs to be the struct or class the property belongs to'
    end
    if @entity.struct? && method.parameters.first.type.is_class
      report_error 'Only parameter on a struct property needs to be the struct'
    end
    if @entity.class? && !method.parameters.first.type.is_class
      report_error 'Only parameter on a class property needs to be the class'
    end
    if @entity.enum? && !method.parameters.first.type.is_enum
      report_error 'Only parameter on a enum property needs to be the enum'
    end

    @entity.properties << method
  end

  def handle_static_method
    report_error 'Method found before class/struct definition' if @entity.nil?
    discardable = parse_discardable_result
    method = parse_func
    method.static = true
    method.discardable_result = discardable

    # Remove prefix
    unless method.name.start_with? "TW#{@entity.name}"
      report_error "Static method name needs to start with class/struct name. Method name #{method.name} does not start with TW#{@entity.name}."
    end
    method.name.slice! "TW#{@entity.name}"

    @entity.static_methods << method
  end

  def handle_static_property
    report_error 'Method found before class/struct definition' if @entity.nil?
    method = parse_func
    method.static = true

    # Remove prefix
    unless method.name.start_with? "TW#{@entity.name}"
      report_error 'Method name needs to start with class/struct name'
    end
    method.name.slice! "TW#{@entity.name}"

    unless method.parameters.count.zero?
      report_error 'Static properties can have no parameters'
    end

    @entity.static_properties << method
  end

  private

  def scan_or_fail(regex, error)
    report_error error if @buffer.scan(regex).nil?
  end

  def report_error(message)
    raise "#{@path}:#{current_line_number} #{message}"
  end

  def current_line_number
    @buffer.string[0..@buffer.pos].count("\n") + 1
  end

  def clear_comment
    @entity_comment = ''
  end
end
