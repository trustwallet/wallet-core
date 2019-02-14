require 'swift_helper'
require 'test/unit'

class SwiftHelperTest < Test::Unit::TestCase
  def test_format_name
    assert_equal(SwiftHelper.format_name('Equal'), '== ')
    assert_equal(SwiftHelper.format_name('Less'), '< ')
    assert_equal(SwiftHelper.format_name('ABCdefGhi'), 'abcdefGhi')
  end

  def test_type
    assert_equal(SwiftHelper.type(TypeDecl.new(name: :bool)), 'Bool')
    assert_equal(SwiftHelper.type(TypeDecl.new(name: :data)), 'Data')
  end
end
