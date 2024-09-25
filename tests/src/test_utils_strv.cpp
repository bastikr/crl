#include <gtest/gtest.h>

#include <crl/utils/strv.h>

TEST(Crlv, sizev) {
    using crl::utils::strv::sizev;

    std::string_view b0;
    std::string_view b1 = "1";
    std::string_view b4 = "1234";

    {
        std::array<std::string_view, 0> b{};
        size_t s = sizev(b);
        ASSERT_EQ(s, 0);
    }

    {
        std::array<std::string_view, 1> b{b0};
        size_t s = sizev(b);
        ASSERT_EQ(s, 0);
    }

    {
        std::array<std::string_view, 1> b{b1};
        size_t s = sizev(b);
        ASSERT_EQ(s, 1);
    }

    {
        std::array<std::string_view, 1> b{b4};
        size_t s = sizev(b);
        ASSERT_EQ(s, 4);
    }

    {
        std::array<std::string_view, 3> b{b0, b4, b0};
        size_t s = sizev(b);
        ASSERT_EQ(s, 4);
    }

    {
        std::array<std::string_view, 3> b{b4, b1, b4};
        size_t s = sizev(b);
        ASSERT_EQ(s, 9);
    }
}

TEST(Crlv, findv) {
    using crl::utils::strv::findv;
    using crl::utils::strv::sizev;

    std::string_view b1 = "logs";
    std::string_view b2 = "/";
    std::string_view b3 = "source";
    std::string_view b4 = "/t.txt";
    std::array<std::string_view, 4> b{b1, b2, b3, b4};

    size_t p = findv(b, 0, sizev(b), '/');
    ASSERT_EQ(p, 4);
    p = findv(b, 0, std::string_view::npos, '/');
    ASSERT_EQ(p, 4);
    p = findv(b, 4, std::string_view::npos, '/');
    ASSERT_EQ(p, 4);
    p = findv(b, 5, std::string_view::npos, '/');
    ASSERT_EQ(p, 11);
    p = findv(b, 12, std::string_view::npos, '/');
    ASSERT_EQ(p, std::string_view::npos);
    p = findv(b, sizev(b) + 1, std::string_view::npos, '/');
    ASSERT_EQ(p, std::string_view::npos);
}

TEST(Crlv, equalv) {
    using crl::utils::strv::equalv;
    std::string_view b0;
    std::string_view b1 = "x";
    std::string_view b4 = "1234";

    {
        std::array<std::string_view, 0> b{};
        ASSERT_TRUE(equalv(b, 0, std::string_view::npos, ""));
        ASSERT_TRUE(equalv(b, 1, std::string_view::npos, ""));
        ASSERT_TRUE(equalv(b, 0, 0, ""));

        ASSERT_FALSE(equalv(b, 0, std::string_view::npos, "1"));
        ASSERT_FALSE(equalv(b, 1, std::string_view::npos, "1"));
        ASSERT_FALSE(equalv(b, 0, 0, "1"));
    }

    {
        std::array<std::string_view, 2> b{b0, b0};
        ASSERT_TRUE(equalv(b, 0, std::string_view::npos, ""));
        ASSERT_TRUE(equalv(b, 1, std::string_view::npos, ""));
        ASSERT_TRUE(equalv(b, 0, 0, ""));

        ASSERT_FALSE(equalv(b, 0, std::string_view::npos, "1"));
        ASSERT_FALSE(equalv(b, 1, std::string_view::npos, "1"));
        ASSERT_FALSE(equalv(b, 0, 0, "1"));
    }

    {
        std::array<std::string_view, 1> b{b1};
        ASSERT_TRUE(equalv(b, 0, std::string_view::npos, "x"));
        ASSERT_TRUE(equalv(b, 0, 1, "x"));

        ASSERT_FALSE(equalv(b, 0, std::string_view::npos, "1"));
        ASSERT_FALSE(equalv(b, 1, std::string_view::npos, "x"));
        ASSERT_FALSE(equalv(b, 0, 0, "x"));
    }

    {
        std::array<std::string_view, 1> b{b4};
        ASSERT_TRUE(equalv(b, 0, std::string_view::npos, "1234"));
        ASSERT_TRUE(equalv(b, 0, 4, "1234"));
        ASSERT_TRUE(equalv(b, 1, std::string_view::npos, "234"));
        ASSERT_TRUE(equalv(b, 1, 2, "23"));

        ASSERT_FALSE(equalv(b, 1, std::string_view::npos, "1234"));
        ASSERT_FALSE(equalv(b, 0, std::string_view::npos, "1233"));
    }

    {
        std::array<std::string_view, 2> b{b1, b4};
        ASSERT_TRUE(equalv(b, 0, std::string_view::npos, "x1234"));
        ASSERT_TRUE(equalv(b, 0, 5, "x1234"));
        ASSERT_TRUE(equalv(b, 1, std::string_view::npos, "1234"));
        ASSERT_TRUE(equalv(b, 2, std::string_view::npos, "234"));
        ASSERT_TRUE(equalv(b, 0, 4, "x123"));
        ASSERT_TRUE(equalv(b, 2, 2, "23"));

        ASSERT_FALSE(equalv(b, 0, std::string_view::npos, "y1234"));
    }
}
