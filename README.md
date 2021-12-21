# widechar

Conversion between std::string and std::wstring

## Usage

Drop `widechar` at your include path

## Supported platforms and compilers

Any C++11

## Interface

```cpp
namespace widechar {

    // Convert from UTF-16(Windows) or UTF-32(Others) to UTF-8
    std::string narrow(std::wstring const&);
    // Convert from UTF-8 to UTF-16(Windows) or UTF-32(Others)
    std::wstring wide(std::string const&);

    // Version for ASCII-strings
    namespace ascii {
        std::string narrow(std::wstring const&);
        std::wstring wide(std::string const&);
    }
    
}
```
