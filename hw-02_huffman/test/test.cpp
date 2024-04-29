//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//
//#include "test.h"
//#include "huffman.h"
//#include <iostream>
//#include <fstream>
//
//namespace frequency_table_tests
//{
//    TEST_CASE("Smoke")
//    {
//        huffman_compression::frequency_table ft;
//        CHECK(ft._table.empty());
//        CHECK(ft._table[0] == 0);
//    }
//
//    TEST_CASE("Frequency table constructor from std::map")
//    {
//        std::map<char, std::size_t> table = {{'a', 2},
//                                             {'b', 3},
//                                             {'c', 1}};
//        huffman_compression::frequency_table ft(table);
//
//        CHECK(ft._table['a'] == 2);
//        CHECK(ft._table['b'] == 3);
//        CHECK(ft._table['c'] == 1);
//    }
//
//    TEST_CASE("Frequency table constructor from std::vector")
//    {
//        std::vector<char> bytes = {'a', 'b', 'c', 'b', 'a'};
//        huffman_compression::frequency_table ft(bytes);
//
//        CHECK(ft._table['a'] == 2);
//        CHECK(ft._table['b'] == 2);
//        CHECK(ft._table['c'] == 1);
//    }
//
//    TEST_CASE("Frequency table operator[]")
//    {
//        std::map<char, std::size_t> table = {{'a', 2},
//                                             {'b', 3},
//                                             {'c', 1}};
//        huffman_compression::frequency_table ft(table);
//
//        CHECK(ft['a'] == 2);
//        CHECK(ft['b'] == 3);
//        CHECK(ft['c'] == 1);
//        CHECK(ft['d'] == 0);
//    }
//}
//
//namespace node_tests
//{
//    TEST_CASE("Smoke")
//    {
//        huffman_compression::node n;
//    }
//
//    TEST_CASE("Constructor with value and frequency")
//    {
//        huffman_compression::node n('a', 3);
//        CHECK(n.get_value() == 'a');
//        CHECK(n.get_frequency() == 3);
//        CHECK(n.get_left() == nullptr);
//        CHECK(n.get_right() == nullptr);
//    }
//
//    TEST_CASE("Constructor with left and right nodes")
//    {
//        std::shared_ptr<huffman_compression::node> left = std::make_shared<huffman_compression::node>('b', 2);
//        std::shared_ptr<huffman_compression::node> right = std::make_shared<huffman_compression::node>('c', 1);
//        huffman_compression::node n(left, right);
//        CHECK(n.get_frequency() == 3);
//        CHECK(n.get_left() == left);
//        CHECK(n.get_right() == right);
//    }
//}
//
//namespace tree_tests
//{
//    TEST_CASE("Smoke")
//    {
//        huffman_compression::tree t;
//        CHECK(t.get_root() == nullptr);
//        CHECK(t.get_huffman_code_for_byte('a') == "");
//        CHECK(t.get_byte_for_huffman_code("\0") == '\0');
//    }
//
//    TEST_CASE("Constructor with root node")
//    {
//        std::map<char, std::size_t> table = {{'a', 2},
//                                             {'b', 3},
//                                             {'c', 1}};
//        auto ft = huffman_compression::frequency_table(table);
//        huffman_compression::tree t(ft);
//
//        CHECK(t.get_huffman_code_for_byte('a') == "11");
//        CHECK(t.get_huffman_code_for_byte('b') == "0");
//        CHECK(t.get_huffman_code_for_byte('c') == "10");
//        CHECK(t.get_byte_for_huffman_code("11") == 'a');
//        CHECK(t.get_byte_for_huffman_code("0") == 'b');
//        CHECK(t.get_byte_for_huffman_code("10") == 'c');
//    }
//}
//
//namespace compressing_tests
//{
//    TEST_CASE("Empty file")
//    {
//        std::string inputFilename = "samples/empty.txt";
//        std::string outputFilename = "samples/emptyC.txt";
//        std::tuple<std::size_t, std::size_t, std::size_t> result1 = huffman_compression::huffman::compress(
//                inputFilename, outputFilename);
//        CHECK((std::get<0>(result1) == 0 && std::get<1>(result1) == 0 && std::get<2>(result1) == 0));
//
//        std::string decompressedFilename = "samples/emptyD.txt";
//        std::tuple<std::size_t, std::size_t, std::size_t> result2 = huffman_compression::huffman::decompress(
//                outputFilename, decompressedFilename);
//        CHECK((std::get<0>(result2) == 0 && std::get<1>(result2) == 0 && std::get<2>(result2) == 0));
//    }
//
//    TEST_CASE("All bytes")
//    {
//        std::string inputFilename = "samples/allBytes.txt";
//        std::ofstream out(inputFilename, std::ios::binary);
//        for (int i = -128; i <= 127; ++i)
//        {
//            char ch = static_cast<char>(i);
//            out.write(&ch, 1);
//        }
//        out.close();
//
//        std::string compressedFilename = "samples/allBytesC.txt";
//        huffman_compression::huffman::compress(inputFilename, compressedFilename);
//
//        std::string decompressedFilename = "samples/allBytesD.txt";
//        huffman_compression::huffman::decompress(compressedFilename, decompressedFilename);
//
//        std::ifstream in(decompressedFilename, std::ios::binary);
//        std::ifstream in2(inputFilename, std::ios::binary);
//        while (!in.eof() && !in2.eof())
//        {
//            char ch1, ch2;
//            in.read(&ch1, 1);
//            in2.read(&ch2, 1);
//            CHECK(ch1 == ch2);
//        }
//    }
//
//    TEST_CASE("WarAndPeace")
//    {
//        std::string inputFilename = "samples/book-war-and-peace.txt";
//        std::string compressedFilename = "samples/book-war-and-peaceC.bin";
//        std::string decompressedFilename = "samples/book-war-and-peaceD.txt";
//        huffman_compression::huffman::compress(inputFilename, compressedFilename);
//        huffman_compression::huffman::decompress(compressedFilename, decompressedFilename);
//        std::ifstream in(decompressedFilename, std::ios::binary);
//        std::ifstream in2(inputFilename, std::ios::binary);
//        while (!in.eof() && !in2.eof())
//        {
//            char ch1, ch2;
//            in.read(&ch1, 1);
//            in2.read(&ch2, 1);
//            CHECK(ch1 == ch2);
//        }
//    }
//}
