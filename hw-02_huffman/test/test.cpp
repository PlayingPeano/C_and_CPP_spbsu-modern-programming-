#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test.h"
#include "huffman.h"
#include <iostream>

namespace frequency_table_tests
{
    TEST_CASE("Smoke")
    {
        huffman_compression::frequency_table ft;
        CHECK(ft._table.empty());
        CHECK(ft._table[0] == 0);
    }

    TEST_CASE("Frequency table constructor from std::map")
    {
        std::map<char, std::size_t> table = {{'a', 2}, {'b', 3}, {'c', 1}};
        huffman_compression::frequency_table ft(table);

        CHECK(ft._table['a'] == 2);
        CHECK(ft._table['b'] == 3);
        CHECK(ft._table['c'] == 1);
    }

    TEST_CASE("Frequency table constructor from std::vector")
    {
        std::vector<char> bytes = {'a', 'b', 'c', 'b', 'a'};
        huffman_compression::frequency_table ft(bytes);

        CHECK(ft._table['a'] == 2);
        CHECK(ft._table['b'] == 2);
        CHECK(ft._table['c'] == 1);
    }

    TEST_CASE("Frequency table operator[]")
    {
        std::map<char, std::size_t> table = {{'a', 2}, {'b', 3}, {'c', 1}};
        huffman_compression::frequency_table ft(table);

        CHECK(ft['a'] == 2);
        CHECK(ft['b'] == 3);
        CHECK(ft['c'] == 1);
        CHECK(ft['d'] == 0);
    }
}

namespace node_tests
{
    TEST_CASE("Smoke")
    {
        huffman_compression::node n;
    }

    TEST_CASE("Constructor with value and frequency")
    {
        huffman_compression::node n('a', 3);
        CHECK(n.get_value() == 'a');
        CHECK(n.get_frequency() == 3);
        CHECK(n.get_left() == nullptr);
        CHECK(n.get_right() == nullptr);
    }

    TEST_CASE("Constructor with left and right nodes")
    {
        std::shared_ptr<huffman_compression::node> left = std::make_shared<huffman_compression::node>('b', 2);
        std::shared_ptr<huffman_compression::node> right = std::make_shared<huffman_compression::node>('c', 1);
        huffman_compression::node n(left, right);
        CHECK(n.get_frequency() == 3);
        CHECK(n.get_left() == left);
        CHECK(n.get_right() == right);
    }
}

namespace tree_tests
{
    TEST_CASE("Smoke")
    {
        huffman_compression::tree t;
        CHECK(t.get_root() == nullptr);
        CHECK(t.get_huffman_code_for_byte('a') == "");
        CHECK(t.get_byte_for_huffman_code("\0") == '\0');
    }

    TEST_CASE("Constructor with root node")
    {
        std::map<char, std::size_t> table = {{'a', 2}, {'b', 3}, {'c', 1}};
        auto ft = huffman_compression::frequency_table(table);
        huffman_compression::tree t(ft);

        CHECK(t.get_huffman_code_for_byte('a') == "11");
        CHECK(t.get_huffman_code_for_byte('b') == "0");
        CHECK(t.get_huffman_code_for_byte('c') == "10");
        CHECK(t.get_byte_for_huffman_code("11") == 'a');
        CHECK(t.get_byte_for_huffman_code("0") == 'b');
        CHECK(t.get_byte_for_huffman_code("10") == 'c');
    }
}

//namespace compressing_tests
//{
//    TEST_CASE("haha")
//    {
//        std::map<char, std::size_t> table = {{'a', 2}, {'b', 3}, {'c', 1}};
//        auto ft = huffman_compression::frequency_table(table);
//        huffman_compression::tree t(ft);
//        std::string inputFilename = "tests/sample.txt";
//        std::string outputFilename = "tests/compressed.txt";
//        huffman_compression::huffman::compress(inputFilename, outputFilename);
//        std::string decompressedFilename = "tests/decompressed.txt";
//        huffman_compression::huffman::decompress(outputFilename, decompressedFilename);
//    }
//}
