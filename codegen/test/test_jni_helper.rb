require 'jni_helper'
require 'test/unit'

class JNIHelperTest < Test::Unit::TestCase
  def test_format_name
    assert_equal(JNIHelper.format_name('Equal'), 'equals')
    assert_equal(JNIHelper.format_name('ABCdefGhi'), 'abcdefGhi')
  end

  def test_function_name
    entity = EntityDecl.new(name: 'Test', is_struct: false)
    method = FunctionDecl.new(name: 'Function', entity: entity, is_method: true)
    name = JNIHelper.function_name(entity: entity, function: method)
    assert_equal(name, 'Java_wallet_core_jni_Test_function')
  end

  def test_type
    assert_equal(JNIHelper.type(TypeDecl.new(name: :bool)), 'jboolean')
    assert_equal(JNIHelper.type(TypeDecl.new(name: :data)), 'jbyteArray')
  end
end
