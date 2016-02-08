#include "gtest/gtest.h"
#include "core/StringExt.h"
#include <iostream>

using namespace std::literals::string_literals;

class StringExtensionTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

protected:
};

TEST_F(StringExtensionTest, ReplaceTest)
{
    struct ReplaceTestCase {
        core::String before, after, toReplace, replacement;
    };

    ReplaceTestCase testCases[] = {{""s, ""s, ""s, ""s},
                                   {"test replace"s, "test"s, " replace"s, ""s},
                                   {"aaa"s, ""s, "aaa"s, ""s},
                                   {"aaa"s, "bbb"s, "aaa"s, "bbb"s},
                                   {"aabbcc"s, "aabbe"s, "cc"s, "e"s},
                                   {"aabbcc"s, "aabbeee"s, "cc"s, "eee"s},
                                   {"aabbcc"s, "aaeeecc"s, "bb"s, "eee"s},
                                   {"abcabc"s, "ababcababc"s, "ab"s, "abab"s},
                                   {" "s, " "s, ""s, ""s}};

    for (auto testCase : testCases) {
        ASSERT_EQ(testCase.after,
                  core::string::Replace(testCase.before, testCase.toReplace,
                                        testCase.replacement))
            << "after: \"" << testCase.after << "\"" << std::endl
            << "before: \"" << testCase.before << "\"" << std::endl;
    }
}