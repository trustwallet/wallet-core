require 'parser'
require 'test/unit'

class ParserTest < Test::Unit::TestCase
  def test_parse_method
    parser = Parser.new(path: '', string: 'void TWPrivateKeyDelete(struct TWPrivateKey *_Nonnull pk);')
    func = parser.parse_func

    assert_equal(func.return_type.name, :void)
    assert_equal(func.name, 'TWPrivateKeyDelete')
    assert_equal(func.parameters.first.name, 'pk')
    assert_equal(func.parameters.first.type.name, 'PrivateKey')
  end

  def test_parse_method_align_pointer_left
    parser = Parser.new(path: '', string: 'struct TWPublicKey* _Nullable TWPublicKeyCreateWithData(TWData* _Nonnull someData);')
    func = parser.parse_func

    assert_equal(func.return_type.name, 'PublicKey')
    assert_equal(func.name, 'TWPublicKeyCreateWithData')
    assert_equal(func.parameters.first.name, 'someData')
    assert_equal(func.parameters.first.type.name, :data)
  end

   def test_parse_method_align_pointer_center
    parser = Parser.new(path: '', string: 'bool TWPublicKeyIsValid(TWData * _Nonnull data);')
    func = parser.parse_func

    assert_equal(func.return_type.name, :bool)
    assert_equal(func.name, 'TWPublicKeyIsValid')
    assert_equal(func.parameters.first.name, 'data')
    assert_equal(func.parameters.first.type.name, :data)
  end

  def test_parse_invalid_method
    parser = Parser.new(path: '', string: '
      TW_EXPORT_METHOD
      void TWPrivateKeyDelete(struct Key *_Nonnull pk);
    ')
    assert_raise(RuntimeError) do
      parser.parse
    end
  end

  def test_init
    parser = Parser.new(path: '', string: '
      TW_EXPORT_CLASS
      struct TWPrivateKey;

      TW_EXPORT_STATIC_METHOD
      struct TWPrivateKey *_Nonnull TWPrivateKeyCreate(void);

      TW_EXPORT_METHOD
      void TWPrivateKeyDelete(struct TWPrivateKey *_Nonnull pk);
    ')
    parser.parse

    assert_not_nil(parser.entity)
    assert_equal(parser.entity.name, 'PrivateKey')
    assert_equal(parser.entity.is_struct, false)
    assert_equal(parser.entity.methods.first.name, 'Delete')
    assert_equal(parser.entity.static_methods.first.name, 'Create')
  end

  def test_parse_public_key
    parser = Parser.new(path: File.expand_path(File.join(File.dirname(__FILE__), '../../include/TrustWalletCore/TWPublicKey.h')))
    parser.parse

    assert_not_nil(parser.entity)
    assert_equal(parser.entity.name, 'PublicKey')
    assert_equal(parser.entity.is_struct, false)
  end
end
