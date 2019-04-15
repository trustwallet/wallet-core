require 'js_helper'
require 'test/unit'

class JsHelperTest < Test::Unit::TestCase
  def test_format_name
    assert_equal(JsHelper.format_name('Equal'), 'equals')
    assert_equal(JsHelper.format_name('ABCdefGhi'), 'abcdefGhi')
  end

  def test_type
    assert_equal(JsHelper.type(TypeDecl.new(name: :bool)), 'boolean')
    assert_equal(JsHelper.type(TypeDecl.new(name: :data)), 'Uint8Array')
  end
end
