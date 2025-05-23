//
// Created by Marek on 30.9.2019.
//


#pragma once

#include <string>
#include "common.hpp"
#include "sections.hpp"
#include "unidecode/utf8_string_iterator.hpp"

namespace unidecode {


    template<typename InputIterator, typename OutputIterator>
    void Unidecode(const InputIterator& in_begin, const InputIterator& in_end, OutputIterator out_it) {
        for (auto it = in_begin; it != in_end; ++it) {
            unicode_char codepoint = *it;

            // TODO: function from this !
            if (codepoint < 0x80) {
                *out_it = static_cast<char>(codepoint);
                ++out_it;
            } else if (codepoint <= 0xeffff) {  // Characters in Private Use Area and above are ignored

                uint32_t section = codepoint >> 8;
                uint32_t position = codepoint & 0xff; // only ast two hex digits
                if (section >= 256 || position >= 256)
                    continue;

                auto table = kUnidecodeData[section];
                if (table != nullptr) {
                    const char* symbol = table[position];
                    while (*symbol != 0) {
                        *out_it = *symbol;
                        ++out_it;
                        symbol++;
                    }
                }
            }
        }

    }

    std::string UnidecodeString(const std::string &str) {
        unidecode::Utf8StringIterator _begin = str.c_str();
        unidecode::Utf8StringIterator _end = str.c_str() + str.length();
        std::string output;
        unidecode::Unidecode(_begin, _end, std::back_inserter(output));
        return output;
    }

}
