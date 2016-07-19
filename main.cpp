#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>

#include <iostream>
#include <sstream>

using namespace std;
using namespace boost::iostreams;

int main() {
    ofstream ofile("hello.z", ios_base::out | ios_base::binary);
    filtering_streambuf<output> out;
    out.push(zlib_compressor());
    out.push(ofile);

    std::istringstream is("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    boost::iostreams::copy(is, out);

    ifstream file("hello.z", ios_base::in | ios_base::binary);
    filtering_streambuf<input> in;
    in.push(zlib_decompressor());
    in.push(file);
    boost::iostreams::copy(in, cout);
    system("pause");
    return 0;
}

