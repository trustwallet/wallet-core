require 'napi_helper'
require 'test/unit'

class NapiHelperTest < Test::Unit::TestCase
  def test_format_name
    assert_equal(NapiHelper.format_name('Equal'), 'equals')
    assert_equal(NapiHelper.format_name('ABCdefGhi'), 'abcdefGhi')
  end

  def test_function_name
    entity = EntityDecl.new(name: 'Test', is_struct: false)
    method = FunctionDecl.new(name: 'Function', entity: entity, is_method: true)
    name = NapiHelper.function_name(entity: entity, function: method)
    assert_equal(name, 'Js_wallet_core_napi_Test_function')
  end

  def test_type
    assert_equal(NapiHelper.type(TypeDecl.new(name: :bool)), 'Boolean')
    assert_equal(NapiHelper.type(TypeDecl.new(name: :data)), 'Uint8Array')
  end
end
