# frozen_string_literal: true

require 'erb'
require 'java_helper'
require 'jni_helper'
require 'swift_helper'

# Code generation
class CodeGenerator
  attr_accessor :entities, :entity, :files, :output_folder
  attr_reader :locals

  def initialize(entities:, files:, output_folder:)
    @entities = entities
    @files = files
    @output_folder = output_folder
  end

  # Renders a template
  def render_template(header:, template:, output_subfolder:, extension:)
    @entities.zip(files) do |entity, file|
      # Make current entity available to templates
      @entity = entity

      code = +''
      code << render(header) unless header.nil?
      string = render(template)
      unless string.nil? || string.empty?
        code << "\n" unless header.nil?
        code << string

        path = File.expand_path(File.join(output_folder, output_subfolder, "#{file}.#{extension}"))
        File.write(path, code)
      end
    end
  end

  def render_swift
    render_template(header: 'swift/header.erb', template: 'swift.erb', output_subfolder: 'swift/Sources', extension: 'swift')

    framework_header = render('swift/TrustWalletCore.h.erb')
    framework_header_path = File.expand_path(File.join(output_folder, 'swift/Sources', 'TrustWalletCore.h'))
    File.write(framework_header_path, framework_header)
  end

  def render_java
    render_template(header: 'java/header.erb', template: 'java.erb', output_subfolder: 'jni/java/com/wallet/crypto/trustapp/jni', extension: 'java')
  end

  def render_jni_h
    render_template(header: 'jni/header.erb', template: 'jni_h.erb', output_subfolder: 'jni/cpp', extension: 'h')
  end

  def render_jni_c
    render_template(header: 'jni/header.erb', template: 'jni_c.erb', output_subfolder: 'jni/cpp', extension: 'c')
  end

  def render(file, locals = {})
    @locals = locals
    path = File.expand_path(file, File.join(File.dirname(__FILE__), 'templates'))
    template = ERB.new(File.read(path), nil, '-')
    template.result(binding)
  end

  def should_return_data(method)
    return false if method.parameters.empty?

    method.return_type.name == :data
  end

  def should_return_string(method)
    return false if method.parameters.empty?

    method.return_type.name == :string
  end

  def should_return_object(method)
    method.return_type.is_struct || method.return_type.is_class
  end
end
