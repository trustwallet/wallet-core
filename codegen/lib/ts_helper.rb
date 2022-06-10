# frozen_string_literal: true

module TsHelper
    def self.parameters(params)
        names = params.map do |param|
          type = type(param.type)
          if param.type.name == :data
            type += ' | Buffer'
          end
          "#{param.name}: #{type}"
        end
        names.join(', ')
    end

    def self.primitive_type(t)
        case t.name
        when :bool
          'boolean'
        when :int, :uint8, :uint16, :uint32, :uint64, :size
          'number'
        when :data
          'Uint8Array | Buffer'
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
          'boolean'
        when :int, :uint8, :int8, :uint16, :int16, :uint32, :int32, :uint64, :int64, :size
          'number'
        when :data
          'Uint8Array'
        when :string
          'string'
        else
          t.name
        end
    end

    def self.combine_declaration_files
        wasm_src = File.expand_path(File.join(File.dirname(__FILE__), '../../wasm'))
        header = File.expand_path('copyright_header.erb', File.join(File.dirname(__FILE__), 'templates'))

        combined = File.open("#{wasm_src}/lib/wallet-core.d.ts", 'w')
        # append header
        combined.write(File.read(header))

        # append .d.ts in src
        Dir.glob("#{wasm_src}/src/*.d.ts").each do |file|
          combined.write(File.read(file))
        end

        # append .d.ts in generated
        Dir.glob("#{wasm_src}/lib/generated/*.d.ts").each do |file|
          combined.write(File.read(file))
        end
        combined.close
        FileUtils.remove_dir("#{wasm_src}/lib/generated", true)
      end
end
