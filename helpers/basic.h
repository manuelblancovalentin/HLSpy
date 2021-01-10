//
// Created by Manuel Blanco Valentin on 1/8/21.
//

#ifndef HLSPY_BASIC_H
#define HLSPY_BASIC_H

// Define Path separator to build dirs and files
#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args );

#endif //HLSPY_BASIC_H
