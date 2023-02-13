use std::ffi::{c_char, CStr, CString};

const ALPHABET_RFC4648: &[u8] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

fn base32_encode(input: &[u8], alphabet: Option<&[u8]>, padding: bool) -> Result<String, String> {
    let alphabet = alphabet.unwrap_or(ALPHABET_RFC4648);
    if alphabet.len() != 32 {
        return Err("Invalid alphabet: must contain 32 characters".to_string());
    }

    let mut result = String::new();
    let mut buffer: u32 = 0;
    let mut buffer_size = 0;

    for &byte in input {
        buffer = (buffer << 8) | u32::from(byte);
        buffer_size += 8;

        while buffer_size >= 5 {
            result.push(char::from(alphabet[(buffer >> (buffer_size - 5)) as usize & 31]));
            buffer_size -= 5;
        }
    }

    if buffer_size > 0 {
        result.push(char::from(alphabet[(buffer << (5 - buffer_size)) as usize & 31]));
    }

    if padding {
        let padding = 8 - (result.len() % 8);
        result.extend(std::iter::repeat('=').take(padding));
    }

    Ok(result)
}

#[no_mangle]
pub extern "C" fn encode_base32(input: *const u8, input_len: usize, alphabet: *const c_char, padding: bool) -> *mut c_char {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };

    let alphabet = match alphabet.is_null() {
        false => {
            let alphabet = unsafe { CStr::from_ptr(alphabet).to_str().unwrap().as_bytes() };
            Some(alphabet)
        }
        true => None,
    };

    match base32_encode(input, alphabet, padding) {
        Ok(result) => CString::new(result).unwrap().into_raw(),
        Err(_) => std::ptr::null_mut(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_base32_encode() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC";

        let result = base32_encode(data, None, false).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_padding() {
        let data = b"Hello, world!";
        let expected = "JBSWY3DPFQQHO33SNRSCC===";

        let result = base32_encode(data, None, true).unwrap();
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base32_encode_filecoin() {
        let alphabet = "abcdefghijklmnopqrstuvwxyz234567";
        let data = b"7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy";
        let expected = "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i";
        let result = base32_encode(data, Some(alphabet.as_bytes()), false).unwrap();
        assert_eq!(result, expected);

        let invalid_alphabet = "invalidalphabet";
        let result = base32_encode(data, Some(invalid_alphabet.as_bytes()), false);
        assert_eq!(result.is_err(), true);
    }

    #[test]
    fn test_base32_encode_ffi() {
        let data = b"Hello, world!";
        let expected_1 = "JBSWY3DPFQQHO33SNRSCC===";
        let expected_2 = "JBSWY3DPFQQHO33SNRSCC";
        let expected_3 = "jbswy3dpfqqho33snrscc===";
        let expected_4 = "jbswy3dpfqqho33snrscc";

        let result_ptr = encode_base32(data.as_ptr(), data.len(), std::ptr::null(), true);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_1);

        let result_ptr = encode_base32(data.as_ptr(), data.len(), std::ptr::null(), false);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_2);

        let alphabet = CString::new("abcdefghijklmnopqrstuvwxyz234567").unwrap();
        let result_ptr = encode_base32(data.as_ptr(), data.len(), alphabet.as_ptr(), true);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_3);

        let result_ptr = encode_base32(data.as_ptr(), data.len(), alphabet.as_ptr(), false);
        let result = unsafe { CString::from_raw(result_ptr) };
        assert_eq!(result.to_str().unwrap(), expected_4);
    }
}
