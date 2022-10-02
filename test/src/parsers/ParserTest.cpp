#include <gtest/gtest.h>

#include "Parser.h"

TEST(ParserTest, Digit)
{
    ASSERT_NO_THROW(fp::digit("4").value());
    EXPECT_EQ(fp::digit("4")->first, '4');

    ASSERT_NO_THROW(fp::digit("5c").value());
    EXPECT_EQ(fp::digit("5c")->first, '5');

    ASSERT_THROW(fp::digit("c4").value(), std::bad_optional_access);
}

TEST(ParserTest, Lower)
{
    ASSERT_NO_THROW(fp::lower("a").value());
    EXPECT_EQ(fp::lower("a")->first, 'a');

    ASSERT_NO_THROW(fp::lower("zA").value());
    EXPECT_EQ(fp::lower("zA")->first, 'z');

    ASSERT_THROW(fp::lower("Aa").value(), std::bad_optional_access);
}

TEST(ParserTest, Upper)
{
    ASSERT_NO_THROW(fp::upper("C").value());
    EXPECT_EQ(fp::upper("C")->first, 'C');

    ASSERT_NO_THROW(fp::upper("Fe").value());
    EXPECT_EQ(fp::upper("Fe")->first, 'F');

    ASSERT_THROW(fp::upper("lP").value(), std::bad_optional_access);
}

TEST(ParserTest, Letter)
{
    ASSERT_NO_THROW(fp::letter("rF").value());
    EXPECT_EQ(fp::letter("rF")->first, 'r');

    ASSERT_NO_THROW(fp::letter("Gp").value());
    EXPECT_EQ(fp::letter("Gp")->first, 'G');

    ASSERT_THROW(fp::letter("@2").value(), std::bad_optional_access);
}

TEST(ParserTest, WhitespaceNotEol)
{
    ASSERT_NO_THROW(fp::whitespacesNotEol("\ra").value());
    EXPECT_EQ(fp::whitespacesNotEol(" \t\v\f")->second, "");

    EXPECT_EQ(fp::whitespacesNotEol("\n\t\v\f")->first, "");
}

TEST(ParserTest, Natural)
{
    ASSERT_NO_THROW(fp::natural<int>("11abc").value());
    EXPECT_EQ(fp::natural<int>("11abc")->first, 11);

    ASSERT_NO_THROW(fp::natural<uint64_t>("28376429813462jj234").value());
    EXPECT_EQ(fp::natural<uint64_t>("28376429813462jj234")->first, 28376429813462);

    ASSERT_THROW(fp::natural<int>("aa11").value(), std::bad_optional_access);
    ASSERT_THROW(fp::natural<int8_t>("128").value(), std::bad_optional_access);
}

TEST(ParserTest, Integer)
{
    ASSERT_NO_THROW(fp::integer<int>("11abc").value());
    EXPECT_EQ(fp::integer<int>("11abc")->first, 11);

    ASSERT_NO_THROW(fp::integer<int>("-11abc").value());
    EXPECT_EQ(fp::integer<int>("-11abc")->first, -11);

    ASSERT_NO_THROW(fp::integer<int64_t>("-28376429813462jj234").value());
    EXPECT_EQ(fp::integer<int64_t>("-28376429813462jj234")->first, -28376429813462);

    ASSERT_THROW(fp::integer<int>("aa11").value(), std::bad_optional_access);
    ASSERT_THROW(fp::integer<int8_t>("128").value(), std::bad_optional_access);
}

TEST(ParserTest, Real)
{
    ASSERT_NO_THROW(fp::real<float>("11abc").value());
    EXPECT_FLOAT_EQ(fp::real<float>("11abc")->first, 11.0f);

    ASSERT_NO_THROW(fp::real<float>("-.234abc").value());
    EXPECT_FLOAT_EQ(fp::real<float>("-.234abc")->first, -.234f);

    ASSERT_NO_THROW(fp::real<float>("3453453fg245.234abc").value());
    EXPECT_FLOAT_EQ(fp::real<float>("3453453fg245.234abc")->first, 3453453.0f);

    ASSERT_THROW(fp::real<float>("ab1.1").value(), std::bad_optional_access);
    ASSERT_THROW(fp::real<float>("999999999999999999999999999999999999999999999999999999999999999999999999").value(),
                 std::bad_optional_access);
}