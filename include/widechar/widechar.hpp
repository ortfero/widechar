// This file is part of widechar library
// Copyright 2021-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT
#pragma once


#include <cstdint>
#include <cuchar>
#include <string>


namespace widechar {


    namespace ascii {

        inline std::string narrow(std::wstring const& ws) {
            auto ns = std::string(ws.size(), char{});
            for(std::size_t i = 0; i != ws.size(); ++i)
                ns[i] = static_cast<char>(ws[i]);
            return ns;
        }


        inline std::wstring wide(std::string const& ns) {
            std::wstring ws(ns.size(), wchar_t{});
            for(std::size_t i = 0; i != ns.size(); ++i)
                ws[i] = static_cast<wchar_t>(ns[i]);
            return ws;
        }
        
    } // namespace ascii


    namespace detail {

        unsigned char const octets_count[256] = {
            // 0b0xxxxxxx: 0 - 127
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            // 0b10xxxxxx: 128 - 191
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            // 0b110xxxxx: 192 - 223
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            // 0b1110xxxx: 224 - 239
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            // 0b11110xxx: 240 - 247
            4, 4, 4, 4, 4, 4, 4, 4,
            // 0b11111xxx: 248 - 255
            0, 0, 0, 0, 0, 0, 0, 0
        };


        inline char32_t
        code_point_from_utf8(unsigned char const*& octets) noexcept{
            char32_t cp;
            switch(octets_count[*octets]) {
                case 1:
                    cp = octets[0];
                    ++octets;
                    return cp;
                case 2:
                    cp = char32_t(octets[0] - 192) << 6
                        | (octets[1] - 128);
                    octets += 2;
                    return cp;
                case 3:
                    cp = char32_t(octets[0] - 224) << 12
                        | char32_t(octets[1] - 128) << 6
                        | (octets[2] - 128);
                    octets += 3;
                    return cp;
                case 4:
                    cp = char32_t(octets[0] - 240) << 18
                        | char32_t(octets[1] - 128) << 12
                        | char32_t(octets[2] - 128) << 6
                        | (octets[3] - 128);
                    octets += 4;
                    return cp;
                default:
                    cp = char32_t('?');
                    ++octets;
                    return cp;
            }
        }


        inline void
        code_point_to_utf8(char32_t cp, unsigned char*& octets) noexcept {
            if(cp <= 0x7F) {
                octets[0] = cp;
                ++octets;
            } else if(cp <= 0x7FF) {
                octets[0] = 0xC0 | (cp >> 6);
                octets[1] = 0x80 | (cp & 0x3F);
                octets += 2;
            } else if(cp <= 0xFFFF) {
                octets[0] = 0xE0 | (cp >> 12);
                octets[1] = 0x80 | ((cp >> 6) & 0x3F);
                octets[2] = 0x80 | (cp & 0x3F);
                octets += 3;
            } else if(cp <= 0x10FFFF) {
                octets[0] = 0xF0 | (cp >> 18);
                octets[1] = 0x80 | ((cp >> 12) & 0x3F);
                octets[2] = 0x80 | ((cp >> 6) & 0x3F);
                octets[3] = 0x80 | (cp & 0x3F);
                octets += 4;
            } else {
                octets[0] = '?';
                ++octets;
            }
        }


        inline char32_t
        code_point_from_utf16(char16_t const*& utf16) noexcept {
            auto const first = *utf16;
            ++utf16;
            if(first < 0xD800 || first >= 0xDC00)
                return char32_t(first);
            auto const second = *utf16;
            ++utf16;
            return (char32_t(first & 0x3FF) << 10)
                + (second & 0x3FF) + 0x10000;
        }


        inline void
        code_point_to_utf16(char32_t cp, char16_t*& utf16) noexcept {
            if(cp <= 0xFFFF) {
                *utf16++ = char16_t(cp);
            } else {
                cp -= 0x10000;
                *utf16++ = 0xD800 | char16_t(cp >> 10);
                *utf16++ = 0xDC00 | char16_t(cp & 0x3FF);
            }
        }

        
    } // namespace detail


    inline std::string narrow(std::wstring const& ws) {
        std::string ns(ws.size() * 4, char{});
#if defined(_WIN32)
        auto from = reinterpret_cast<char16_t const*>(ws.data());
#else
        auto from = reinterpret_cast<char32_t const*>(ws.data());
#endif
        auto to = reinterpret_cast<unsigned char*>(ns.data());
        while(*from != '\0') {
#if defined(_WIN32)
            auto const cp = detail::code_point_from_utf16(from);
#else
            auto const cp = std::uint32_t(*from);
            ++from;
#endif
            detail::code_point_to_utf8(cp, to);
        }
        ns.resize(reinterpret_cast<char*>(to) - ns.data());
        return ns;
    }


    std::wstring wide(std::string const& ns) {
        std::wstring ws(ns.size(), wchar_t{});
        auto from = reinterpret_cast<unsigned char const*>(ns.data());
#if defined(_WIN32)
        auto to = reinterpret_cast<char16_t*>(ws.data());
#else
        auto to = reinterpret_cast<char32_t*>(ws.data());
#endif
        while(*from != '\0') {
            auto const cp = detail::code_point_from_utf8(from);
#if defined(_WIN32)
            detail::code_point_to_utf16(cp, to);            
#else
            *to = cp;
            ++to;
#endif
        }
        ws.resize(reinterpret_cast<wchar_t*>(to) - ws.data());
        return ws;
    }    


} // namespace uutf8
