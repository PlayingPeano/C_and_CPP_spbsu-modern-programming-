#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "test.h"
#include "huffman.h"
#include "bitstream.h"
#include <iostream>
#include <fstream>
#include <string>

namespace frequency_table_tests
{
    TEST_CASE("Smoke")
    {
        huffman_compression::frequency_table ft;
        CHECK(ft._table.empty());
        CHECK(ft._table['0'] == 0);
    }

    TEST_CASE("Frequency table constructor from std::map")
    {
        std::map<char, std::size_t> table = {{'a', 2},
                                             {'b', 3},
                                             {'c', 1}};
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
        std::map<char, std::size_t> table = {{'a', 2},
                                             {'b', 3},
                                             {'c', 1}};
        huffman_compression::frequency_table ft(table);

        CHECK(ft['a'] == 2);
        CHECK(ft['b'] == 3);
        CHECK(ft['c'] == 1);
        CHECK(ft['d'] == 0);
    }

    TEST_CASE("Frequency table GetSizeOfTable")
    {
        huffman_compression::frequency_table ft;
        CHECK(ft.GetSizeOfTable() == 0);

        ft._table = {{'a', 2},
                     {'b', 3},
                     {'c', 1}};
        CHECK(ft.GetSizeOfTable() == 3);

        ft._table['d'] = 0;
        CHECK(ft.GetSizeOfTable() == 4);
    }
}

namespace node_tests
{
    TEST_CASE("Getters")
    {
        huffman_compression::node n;
        CHECK(n.GetValue() == 0);
        CHECK(n.GetFrequency() == 0);
        CHECK(n.GetLeft() == nullptr);
        CHECK(n.GetRight() == nullptr);
    }

    TEST_CASE("Default constructor")
    {
        huffman_compression::node n;
        CHECK(n.GetValue() == 0);
        CHECK(n.GetFrequency() == 0);
        CHECK(n.GetLeft() == nullptr);
        CHECK(n.GetRight() == nullptr);
    }

    TEST_CASE("Constructor with value and frequency")
    {
        huffman_compression::node n('a', 3);
        CHECK(n.GetValue() == 'a');
        CHECK(n.GetFrequency() == 3);
        CHECK(n.GetLeft() == nullptr);
        CHECK(n.GetRight() == nullptr);
    }

    TEST_CASE("Constructor with left and right nodes")
    {
        std::shared_ptr<huffman_compression::node> left = std::make_shared<huffman_compression::node>('b', 2);
        std::shared_ptr<huffman_compression::node> right = std::make_shared<huffman_compression::node>('c', 1);
        huffman_compression::node n(left, right);
        CHECK(n.GetFrequency() == 3);
        CHECK(n.GetLeft() == left);
        CHECK(n.GetRight() == right);
    }
}

namespace tree_tests
{
    TEST_CASE("Smoke")
    {
        huffman_compression::tree t;
        CHECK(t.GetRoot() == nullptr);
        CHECK(t.GetHuffmanCodeForByte('a') == huffman_constants::STR_EMPTY);
        CHECK(t.GetByteForHuffmanCode(std::string{}) == huffman_constants::CHAR_EMPTY);
    }

    TEST_CASE("Constructor with root node")
    {
        std::map<char, std::size_t> table = {{'a', 2},
                                             {'b', 3},
                                             {'c', 1}};
        auto ft = huffman_compression::frequency_table(table);
        huffman_compression::tree t(ft);

        CHECK(t.GetHuffmanCodeForByte('a') == "11");
        CHECK(t.GetHuffmanCodeForByte('b') == "0");
        CHECK(t.GetHuffmanCodeForByte('c') == "10");
        CHECK(t.GetByteForHuffmanCode("11") == 'a');
        CHECK(t.GetByteForHuffmanCode("0") == 'b');
        CHECK(t.GetByteForHuffmanCode("10") == 'c');
    }
}

namespace bitstream_tests
{
    TEST_CASE("Smoke")
    {
        std::string outputFilename = "test/empty.txt";
        std::ofstream out(outputFilename);
        CHECK(out.is_open());
        std::string bytes{};
        bitstream::write(bytes, out);
        out.close();

        std::ifstream in(outputFilename);
        char ch{};
        in.read(&ch, 1);
        CHECK(in.eof());
        in.close();
    }

    TEST_CASE("Write some bytes")
    {
        std::string outputFilename = "test/allBytesOut.txt";
        std::ofstream out(outputFilename, std::ios::binary | std::ios::trunc);
        CHECK(out.is_open());
        std::string bytes{"000000011111111101111111"};
        bitstream::write(bytes, out);
        out.close();

        std::ifstream in(outputFilename, std::ios::binary);
        CHECK(in.is_open());

        char ch{};
        in.read(&ch, 1);
        CHECK(ch == static_cast<char>(-128));
        in.read(&ch, 1);
        CHECK(ch == static_cast<char>(255));
        in.read(&ch, 1);
        CHECK(ch == static_cast<char>(-2));
        in.read(&ch, 1);
        CHECK(in.eof());
        in.close();
    }

    TEST_CASE("Read some bytes")
    {
        std::string outputFilename = "test/allBytesOut.txt";
        std::ofstream out(outputFilename, std::ios::binary | std::ios::trunc);
        CHECK(out.is_open());
        std::vector<char> bytes = {'a', 'b', 'a', 'c', 'a', 'b', 'a'};
        auto ft = huffman_compression::frequency_table(bytes);
        huffman_compression::tree t(ft);
        std::string codedText;
        for (char ch: bytes)
        {
            codedText.append(t.GetHuffmanCodeForByte(ch));
        }
        std::size_t size = codedText.size();
        while (codedText.size() % huffman_constants::BITS_IN_ONE_BYTE != huffman_constants::SIZE_T_ZERO)
        {
            codedText.append(huffman_constants::STR_ZERO);
        }
        bitstream::write(codedText, out);
        out.close();

        std::ifstream in(outputFilename, std::ios::binary);
        CHECK(in.is_open());

        std::string data = bitstream::read(size, in, t.GetMapBytesForHuffmanCodes());
        std::string str(bytes.begin(), bytes.end());
        CHECK(data == str);
    }
}

namespace compressing_and_decompressing_tests
{
    TEST_CASE("Empty file")
    {
        std::string inputFilename = "samples/empty.txt";
        std::string outputFilename = "samples/emptyC.txt";
        std::ifstream in(inputFilename);
        CHECK(in.is_open());
        std::ofstream out(outputFilename);
        CHECK(out.is_open());

        std::tuple<std::size_t, std::size_t, std::size_t> result1 = huffman_compression::Compress(
                in, out);
        CHECK((std::get<0>(result1) == 0 && std::get<1>(result1) == 0 && std::get<2>(result1) == 0));

        in.close();
        out.close();

        std::ifstream inC(outputFilename);
        CHECK(inC.is_open());
        std::string decompressedFilename = "samples/emptyD.txt";
        std::ofstream outD(decompressedFilename);
        CHECK(outD.is_open());
        std::tuple<std::size_t, std::size_t, std::size_t> result2 = huffman_compression::Decompress(
                inC, outD);
        CHECK((std::get<0>(result2) == 0 && std::get<1>(result2) == 0 && std::get<2>(result2) == 0));

        inC.close();
        outD.close();
    }

    TEST_CASE("All bytes")
    {
        std::string inputFilename = "samples/allBytes.txt";
        std::ofstream out1(inputFilename);
        for (int i = -128; i <= 127; ++i)
        {
            char ch = static_cast<char>(i);
            out1.write(&ch, 1);
        }
        out1.close();

        std::ifstream in(inputFilename);
        CHECK(in.is_open());
        std::string compressedFilename = "samples/allBytesC.txt";
        std::ofstream outC(compressedFilename);
        huffman_compression::Compress(in, outC);
        in.close();
        outC.close();

        std::ifstream inC(compressedFilename);
        CHECK(inC.is_open());
        std::string decompressedFilename = "samples/allBytesD.txt";
        std::ofstream outD(decompressedFilename);
        CHECK(outD.is_open());

        huffman_compression::Decompress(inC, outD);
        inC.close();
        outD.close();

        std::ifstream i(decompressedFilename);
        std::ifstream i2(inputFilename);
        while (!i.eof() && !i2.eof())
        {
            char ch1, ch2;
            i.read(&ch1, 1);
            i2.read(&ch2, 1);
            CHECK(ch1 == ch2);
        }
        i.close();
        i2.close();
    }

    TEST_CASE("WarAndPeace")
    {
        std::string inputFilename = "samples/book-war-and-peace.txt";
        std::string compressedFilename = "samples/book-war-and-peaceC.bin";
        std::ifstream in(inputFilename, std::ios::binary);
        CHECK(in.is_open());

        std::ofstream outC(compressedFilename, std::ios::binary | std::ios::trunc);
        CHECK(outC.is_open());

        huffman_compression::Compress(in, outC);
        in.close();
        outC.close();

        std::string decompressedFilename = "samples/book-war-and-peaceD.txt";
        std::ofstream outD(decompressedFilename, std::ios::binary | std::ios::trunc);
        CHECK(outD.is_open());

        std::ifstream inC(compressedFilename, std::ios::binary);
        CHECK(inC.is_open());

        huffman_compression::Decompress(inC, outD);

        std::ifstream inD(decompressedFilename, std::ios::binary);
        std::ifstream inF(inputFilename, std::ios::binary);
        while (!inD.eof() && !inF.eof())
        {
            char ch1, ch2;
            inD.read(&ch1, 1);
            inF.read(&ch2, 1);
            if (!inD.eof() && !inF.eof())
            {
                CHECK(ch1 == ch2);
            }
        }
        inD.close();
        inF.close();
    }
}
