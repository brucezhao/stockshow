#include <string>
#include <iconv.h>
#include "encode.h"

std::string Encode::Convert(const std::string & fromCharset, const std::string & toCharset, const std::string input) {
    iconv_t cd = iconv_open(toCharset.c_str(), fromCharset.c_str());
    if (cd == (iconv_t)-1) {
        //throw std::runtime_error("iconv_open failed");
        return "";
    }

    size_t inBytesLeft = input.size();
    size_t outBytesLeft = inBytesLeft * 4; // Assuming output will not be larger than 4 times the input
    char* inBuf = const_cast<char*>(input.c_str());
    char* outBuf = new char[outBytesLeft];
    char* outPtr = outBuf;

    size_t result = iconv(cd, &inBuf, &inBytesLeft, &outPtr, &outBytesLeft);
    if (result == (size_t)-1) {
        delete[] outBuf;
        iconv_close(cd);
        //throw std::runtime_error("iconv failed");
        return "";
    }

    std::string output(outBuf, outPtr - outBuf);
    delete[] outBuf;
    iconv_close(cd);
    return output;
}
