require 'java_helper'
require 'test/unit'

class JavaHelperTest < Test::Unit::TestCase
  def test_format_name
    assert_equal(JavaHelper.format_name('Equal'), 'equals')
    assert_equal(JavaHelper.format_name('ABCdefGhi'), 'abcdefGhi')
  end

  def test_type
    assert_equal(JavaHelper.type(TypeDecl.new(name: :bool)), 'boolean')
    assert_equal(JavaHelper.type(TypeDecl.new(name: :data)), 'byte[]')
  end
end
