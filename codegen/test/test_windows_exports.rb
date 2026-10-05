require 'test/unit'
require 'tmpdir'
require 'open3'
require 'rbconfig'

class WindowsExportsTest < Test::Unit::TestCase
  def test_public_exports_include_unannotated_functions_without_comments_or_duplicates
    Dir.mktmpdir('wallet core exports ') do |directory|
      File.write(File.join(directory, 'TWExample.h'), <<~HEADER)
        // void TWNotAnExport(void);
        /* void TWAlsoNotAnExport(void); */
        TW_EXPORT_CLASS
        struct TWExample;
        TW_DEPRECATED_FOR(4.0, TWExampleCreate)
        TW_EXPORT_STATIC_METHOD
        TWString* _Nonnull TWExampleCreate(TWData* _Nonnull data);
        TW_EXPORT_METHOD
        void TWExampleDelete(struct TWExample* _Nonnull example);
        extern bool TWAnySignerSupportsJSON(enum TWCoinType coin);
        const char* _Nonnull TWStringUTF8Bytes(
            TWString* _Nonnull string) TW_VISIBILITY_DEFAULT;
        void TWDataDelete(TWData* _Nonnull data) TW_VISIBILITY_DEFAULT;
      HEADER
      definition = File.join(directory, 'exports.def')
      _, error, status = Open3.capture3(RbConfig.ruby, File.expand_path('../../tools/windows-exports.rb', __dir__), directory, definition)
      assert_predicate(status, :success?, error)
      assert_equal(["LIBRARY TrustWalletCore", "EXPORTS", "    TWAnySignerSupportsJSON", "    TWDataDelete", "    TWExampleCreate", "    TWExampleDelete", "    TWStringUTF8Bytes"], File.readlines(definition, chomp: true))
    end
  end
end
