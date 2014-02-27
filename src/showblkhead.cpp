#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include "util.hpp"
using namespace bc;

bool load_blk_header(block_header_type& blk_header, const std::string& filename)
{
    std::ostringstream contents;
    if (filename == "-")
    {
        std::istreambuf_iterator<char> first(std::cin);
        std::istreambuf_iterator<char> last;
        contents << std::string(first, last);
    }
    else
    {
        std::ifstream infile(filename, std::ifstream::binary);
        if (!infile)
        {
            std::cerr << "showblkhead: Bad file." << std::endl;
            return false;
        }
        contents << infile.rdbuf();
    }
    data_chunk raw_blk_header = decode_hex(contents.str());
    try
    {
        satoshi_load(raw_blk_header.begin(), raw_blk_header.end(), blk_header);
    }
    catch (end_of_stream)
    {
        std::cerr << "showblkhead: Deserializing block header failed."
            << std::endl;
        return false;
    }
    return true;
}

void show_blk_header(const block_header_type& blk_header)
{
    std::cout << "hash: " << hash_block_header(blk_header) << std::endl;
    std::cout << "version: " << blk_header.version << std::endl;
    std::cout << "previous_block_hash: "
        << blk_header.previous_block_hash << std::endl;
    std::cout << "merkle: " << blk_header.merkle << std::endl;
    std::cout << "timestamp: " << blk_header.timestamp << std::endl;
    std::cout << "bits: " << blk_header.bits << std::endl;
    std::cout << "nonce: " << blk_header.nonce << std::endl;
}

void json_show_blk_header(const block_header_type& blk_header)
{
    std::cout << "{" << std::endl;
    std::cout << "  \"hash\": \"" << hash_block_header(blk_header)
        << "\"," << std::endl;
    std::cout << "  \"version\": " << blk_header.version << "," << std::endl;
    std::cout << "  \"previous_block_hash\": \""
        << blk_header.previous_block_hash << "\"," << std::endl;
    std::cout << "  \"merkle\": \"" << blk_header.merkle << "\"," << std::endl;
    std::cout << "  \"timestamp\": " << blk_header.timestamp
        << "," << std::endl;
    std::cout << "  \"bits\": " << blk_header.bits << "," << std::endl;
    std::cout << "  \"nonce\": " << blk_header.nonce << std::endl;
    std::cout << "}" << std::endl;
}

int main(int argc, char** argv)
{
    std::string filename = "-";
    bool json_output = false;
    for (size_t i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "-j" || arg == "--json")
        {
            json_output = true;
            continue;
        }
        if (filename != "-")
        {
            std::cerr << "Usage: sx showblkhead [-j] FILENAME" << std::endl;
            return -1;
        }
        filename = arg;
    }
    block_header_type blk_header;
    if (!load_blk_header(blk_header, filename))
        return -1;
    if (json_output)
        json_show_blk_header(blk_header);
    else
        show_blk_header(blk_header);
    return 0;
}

