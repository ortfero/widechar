project('widechar', 'cpp',
    license: 'MIT',
    default_options: [
        'cpp_std=c++17',
        'warning_level=3'])

headers = [
    'include/widechar/widechar.hpp'
]

incdirs = include_directories('./include')

widechar = declare_dependency(
    include_directories: incdirs,
    sources: headers
)

subdir('test')

install_headers(headers, subdir: 'widechar')

pkg = import('pkgconfig')
pkg.generate(
    name: 'widechar',
    filebase: 'widechar',
    subdirs: ['widechar'],
    description: 'C++17 header-only library for conversion between std::string and std::wstring'
)
