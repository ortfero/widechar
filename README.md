# widechar

C++17 header-only library for conversion between std::string and std::wstring

## Usage

Drop `include/widechar` at your include path


## Interface

```cpp
namespace widechar {

    // Convert from UTF-16(Windows) or UTF-32(Others) to UTF-8
    std::string narrow(std::wstring const&);
	std::string narrow(std::wstring_view);
    // Convert from UTF-8 to UTF-16(Windows) or UTF-32(Others)
    std::wstring wide(std::string const&);
	std::wstring wide(std::string_view);

    // Version for ASCII-strings
    namespace ascii {
        std::string narrow(std::wstring const&);
		std::string narrow(std::wstring_view);
        std::wstring wide(std::string const&);
		std::string wide(std::wstring);
    }
    
}
```
