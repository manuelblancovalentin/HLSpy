//
// Created by Manuel Blanco Valentin on 1/7/21.
//

#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <regex>
#include <sys/stat.h>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


// Function to count occurrences of a certain pattern in string
static inline int string_count(std::string mainStr, std::regex pattern){
    // Initialize occurrences
    int occurrences = 0;

    // finding all the match.
    for (std::sregex_iterator it = std::sregex_iterator(mainStr.begin(), mainStr.end(), pattern);
         it != std::sregex_iterator(); it++) {
        std::smatch match;
        match = *it;
        /*
        cout << "\nMatched  string is = " << match.str(0)
             << "\nand it is found at position "
             << match.position(0) << endl;
        cout << "Capture " << match.str(1)
             << " at position " << match.position(1) << endl;
        */
        occurrences += 1;
    }

    return occurrences;
}


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


static inline bool stringStartsWith(std::string mainStr, std::string toMatch)
{
    // std::string::find returns 0 if toMatch is found at starting
    if(mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}

// This function was obtained from: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool file_exists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

// This helps us check if an object is isntance of class or not
template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}