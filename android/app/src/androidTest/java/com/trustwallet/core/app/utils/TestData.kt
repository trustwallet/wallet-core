import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHex
import org.junit.Assert.assertEquals
import org.junit.Test

class TestData {
    init {
        System.loadLibrary("TrustWalletCore");
    }

    @Test
    fun testUsingNumeric() {
        val data = Numeric.hexStringToByteArray("01020304")
        assertEquals(Numeric.toHexString(data), "0x01020304")
    }

    @Test
    fun testUsingExtensions() {
        val data = "01020304".toHexBytes()
        assertEquals(data.toHex(), "0x01020304")

        // with prefix
        val data2 = "0x01020304".toHexBytes()
        assertEquals(data2.toHex(), "0x01020304")
    }

    @Test
    fun testOddLength() {
        val data = "0x0".toHexBytes()
        assertEquals(data.toHex(), "0x00")

        val data2 = "0x28fa6ae00".toHexBytes()
        assertEquals(data2.toHex(), "0x028fa6ae00")
    }
}
