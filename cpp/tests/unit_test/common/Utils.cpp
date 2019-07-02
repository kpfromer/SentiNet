/**
 * Some Kelvin specific source code header describing the use and what license it might be released under.
 */

// Kelvin includes
#include "common/Utils.hpp"
#include "TestUtils.hpp"

/** Some simple tests to validate the system running the unit test.
 */
TEST(UtilTests, DataTypes) {
  EXPECT_EQ(100U, static_cast<unsigned>(100));
  EXPECT_EQ(1234, static_cast<int32_t>(1234.0));
  EXPECT_EQ(1234, static_cast<int32_t>(1234.5));
  EXPECT_NE(1235, static_cast<int32_t>(1234.5));
  EXPECT_EQ(1234, static_cast<int32_t>(1234.999));
}

/** Tests that the to_hex() method is able to take a std::string of raw data, and print out
 *  the proper string in hex.
 */
TEST(UtilTests, HexString) {
  // Load up the vector with some sample data
  std::vector<unsigned char> raw;
  for(auto i = 0U; i < 16; ++i) {
    raw.emplace_back(i);
  }
  raw.emplace_back(255);

  EXPECT_EQ("", utils::to_hex(std::string("")));
  EXPECT_EQ("0x000102030405060708090A0B0C0D0E0FFF", utils::to_hex(std::string(raw.begin(), raw.end())));
  EXPECT_EQ("0x0001\n0x0203\n0x0405\n0x0607\n0x0809\n0x0A0B\n0x0C0D\n0x0E0F\n0xFF",
            utils::to_hex(std::string(raw.begin(), raw.end()), 2));
  EXPECT_EQ("0x000102030405\n0x060708090A0B\n0x0C0D0E0FFF", utils::to_hex(std::string(raw.begin(), raw.end()), 6));
}

// A templated helper method for testing out the edge cases of the RNG generator.
template <typename T>
void static_rng_test() {
  const auto min = utils::get_random<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max() - 2);
  const auto max = utils::get_random<T>(min + 1);
  // Check the edge cases
  ASSERT_LT(min, max);
  EXPECT_EQ(min, utils::get_random<T>(min, min));
  EXPECT_EQ(max, utils::get_random<T>(max, max));
  // Check the seed vale
  const auto seed = utils::get_random<T>();
  EXPECT_EQ(utils::get_random<T>(min, max, seed), utils::get_random<T>(min, max, seed));
  // Swap the arguments
  EXPECT_THROW(utils::get_random<T>(max, min), InvalidArgument);
}

// A templated helper method for testing out a bunch of randomly generated test cases.
template <typename T>
void dynamic_rng_test() {
  const auto min = utils::get_random<T>();
  const auto max = utils::get_random<T>(min);
  {
    const auto rng = utils::get_random<T>(min, max);
    EXPECT_LE(min, rng);
    EXPECT_GE(max, rng);
  }
}

/** Tests that the random number generator is able to generate values within the specified range
 *  and of the correct type.
 */
TEST(UtilTests, RandomValueGenerator) {
  // Test out the edge cases for the various templated types
  static_rng_test<uint8_t>();
  static_rng_test<int8_t>();
  static_rng_test<uint16_t>();
  static_rng_test<int16_t>();
  static_rng_test<uint32_t>();
  static_rng_test<int32_t>();
  static_rng_test<uint64_t>();
  static_rng_test<int64_t>();
  static_rng_test<float>();
  static_rng_test<double>();
  {
    // std::strings are different
    const auto min = utils::get_random<uint32_t>(20, std::numeric_limits<uint8_t>::max() - 2);
    const auto max = utils::get_random<uint32_t>(min + 1, std::numeric_limits<uint8_t>::max());
    // Check the edge cases
    ASSERT_LT(min, max);
    EXPECT_EQ(min, utils::get_random_string(min, min).size());
    EXPECT_EQ(max, utils::get_random_string(max, max).size());
    // Check the seed vale
    const auto seed = utils::get_random<uint32_t>();
    EXPECT_EQ(utils::get_random_string(min, max, seed), utils::get_random_string(min, max, seed));
    // Swap the arguments
    EXPECT_THROW(utils::get_random_string(max, min), InvalidArgument);
  }
  {
    // chars are different .. code coverage
    utils::random_char();
  }
  {
    // Check the UUID
    // xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    // cf7e8ef2-5420-6419-af5f-f21634ddaf74
    const auto uuid = utils::get_random_uuid();
    for(const auto& idx : {8, 13, 18, 23}) {
      EXPECT_TRUE('-' == uuid.at(idx));
    }
  }

  // Run the test a bunch of times just to make sure we're getting a sampling of data.
  for(auto i = 0; i < 1000; i++) {
    dynamic_rng_test<uint8_t>();
    dynamic_rng_test<int8_t>();
    dynamic_rng_test<uint16_t>();
    dynamic_rng_test<int16_t>();
    dynamic_rng_test<uint32_t>();
    dynamic_rng_test<int32_t>();
    dynamic_rng_test<uint64_t>();
    dynamic_rng_test<int64_t>();
    dynamic_rng_test<float>();
    dynamic_rng_test<double>();

    {
      // std::strings are different
      const auto min = utils::get_random<uint32_t>(20, std::numeric_limits<uint8_t>::max() - 2);
      const auto max = utils::get_random<uint32_t>(min + 1, std::numeric_limits<uint8_t>::max());
      const auto rng = utils::get_random_string(min, max);
      EXPECT_LE(min, rng.size());
      EXPECT_GE(max, rng.size());
    }
  }
}

/** Tests that we are able to compare string equality
 */
TEST(UtilTests, StringIsEqual) {
  // Check some of the edge cases
  EXPECT_TRUE(utils::is_equal("", ""));
  EXPECT_FALSE(utils::is_equal("", "hello"));
  EXPECT_FALSE(utils::is_equal("hello", ""));
  EXPECT_TRUE(utils::is_equal("hello", "hello"));
  EXPECT_FALSE(utils::is_equal("hello", "world"));
  EXPECT_FALSE(utils::is_equal("hello", "hello1"));
  EXPECT_FALSE(utils::is_equal(" hello", "hello"));

  // Check that it handles mixed types
  const std::string str1 = "Hello";
  const std::string str2 = "Hello";
  const char* char1 = str1.c_str();
  const char* char2 = str2.c_str();
  EXPECT_TRUE(utils::is_equal(char1, char2));
  EXPECT_TRUE(utils::is_equal(str1, str2));
  EXPECT_TRUE(utils::is_equal(char1, str2));
  EXPECT_TRUE(utils::is_equal(str2, char2));
  EXPECT_FALSE(char1 == char2);
  EXPECT_TRUE(str1 == char2);  // Expectation is that this would be FALSE

  // This is where equality starts to break down and can lead to comparison problems
  // when not hiding the operators behind a method call.
  const char c1[] = "Hello";
  const char c2[] = "Hello";
  EXPECT_FALSE(c1 == c2);
  EXPECT_TRUE(utils::is_equal(c1, c2));

  const char* const1 = "World";
  const char* const2 = "World";
  EXPECT_TRUE(const1 == const2);  // Expectation is that this would be FALSE
  EXPECT_TRUE(utils::is_equal(const1, const2));

  char v3[] = "Sith";
  char v4[] = "Sith";
  EXPECT_FALSE(v3 == v4);
  EXPECT_TRUE(utils::is_equal(v3, v4));

  char* c3 = v3;
  char* c4 = v4;
  EXPECT_FALSE(c3 == c4);  // Because it's comparing the non-const, it's FALSE and not TRUE
  EXPECT_TRUE(utils::is_equal(c3, c4));
}

/** Tests that we are able to find substrings in a string and return a boolean
 */
TEST(UtilTests, StringContains) {
  EXPECT_FALSE(utils::contains("", ""));
  EXPECT_FALSE(utils::contains("Hello", ""));
  EXPECT_FALSE(utils::contains("", "Hello"));
  EXPECT_FALSE(utils::contains("Hello", "World"));

  // Iterator over all substrings in this longer string.
  const std::string str = "This is a really long sentence that we are going to compare against.";
  for(auto i = 1U; i < str.size(); ++i) {
    EXPECT_TRUE(utils::contains(str, str.substr(0, i)));
  }
  for(auto i = 1U; i < str.size(); ++i) {
    EXPECT_TRUE(utils::contains(str, str.substr(str.size() - i, i)));
  }
}

#define STATIC_STRING_0 "Master Jedi"
#define STATIC_STRING_1 "  Master Jedi  "
#define STATIC_STRING_2 "\t\n Master Jedi\v \r "
#define STATIC_STRING_3 "abcdMaster Jedi1234"

/** Tests that we are able to take strings with padding on the front or the end
 *  and remove them.  These methods can be used to for strings that are left or right
 *  justified and a bunch of whitespace/hidden character inserted.
 */
TEST(UtilTests, StringTrimming) {
  {
    // Edge cases
    EXPECT_EQ("", utils::ltrim(""));
    EXPECT_EQ("", utils::rtrim(""));
    EXPECT_EQ("", utils::trim(""));

    EXPECT_EQ(STATIC_STRING_0, utils::ltrim(STATIC_STRING_0));
    EXPECT_EQ(STATIC_STRING_0, utils::rtrim(STATIC_STRING_0));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(STATIC_STRING_0));
  }

  {
    // Doing it as #defines and hardcoded strings
    EXPECT_EQ("Master Jedi  ", utils::ltrim(STATIC_STRING_1));
    EXPECT_EQ("  Master Jedi", utils::rtrim(STATIC_STRING_1));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(STATIC_STRING_1));

    EXPECT_EQ("Master Jedi\v \r ", utils::ltrim(STATIC_STRING_2));
    EXPECT_EQ("\t\n Master Jedi", utils::rtrim(STATIC_STRING_2));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(STATIC_STRING_2));

    EXPECT_EQ(utils::trim(STATIC_STRING_1), utils::trim(STATIC_STRING_2));
  }

  {
    // Doing it with variables (verifies that it doesn't modify the original)
    auto str = std::string(STATIC_STRING_1);
    EXPECT_EQ("Master Jedi  ", utils::ltrim(str));
    EXPECT_EQ("  Master Jedi", utils::rtrim(str));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(str));
  }

  {
    // Doing it with consts
    const auto str = std::string(STATIC_STRING_1);
    EXPECT_EQ("Master Jedi  ", utils::ltrim(str));
    EXPECT_EQ("  Master Jedi", utils::rtrim(str));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(str));
  }

  {
    // Verify we can use other delimiters
    const char* delim = "abcd1234";
    EXPECT_EQ("Master Jedi1234", utils::ltrim(STATIC_STRING_3, delim));
    EXPECT_EQ("abcdMaster Jedi", utils::rtrim(STATIC_STRING_3, delim));
    EXPECT_EQ(STATIC_STRING_0, utils::trim(STATIC_STRING_3, delim));
  }

  {
    // Verify we can use other delimiters and get to an empty string
    const char* delim = "abcd1234steri MJ";
    EXPECT_EQ("", utils::ltrim(STATIC_STRING_3, delim));
    EXPECT_EQ("", utils::rtrim(STATIC_STRING_3, delim));
    EXPECT_EQ("", utils::trim(STATIC_STRING_3, delim));
  }
}

/** Tests that we are able to take strings into all upper case.
 */
TEST(UtilTests, StringToUpperToLower) {
  {
    EXPECT_EQ("", utils::to_upper(""));

    std::string str = "ThIs iS a seNTEnce !@#$%^ with rANdoM 123650 NUMbers and =- characteRS";
    std::string str_lower = "this is a sentence !@#$%^ with random 123650 numbers and =- characters";
    std::string str_upper = "THIS IS A SENTENCE !@#$%^ WITH RANDOM 123650 NUMBERS AND =- CHARACTERS";

    // Check a mixture of upper case and lower string
    EXPECT_EQ(str_upper, utils::to_upper(str));
    EXPECT_EQ(str_lower, utils::to_lower(str));

    // Check just a lower case sentence
    EXPECT_EQ(str_upper, utils::to_upper(str_lower));
    EXPECT_EQ(str_lower, utils::to_lower(str_lower));

    // Check just a upper case sentence
    EXPECT_EQ(str_upper, utils::to_upper(str_upper));
    EXPECT_EQ(str_lower, utils::to_lower(str_upper));
  }
}

/** Tests that we are able to take strings and perform substring replacements on them.
 */
TEST(UtilTests, StringReplace) {
  {
    // Edge cases
    EXPECT_EQ("", utils::replace("", "asda", "123"));
    EXPECT_EQ("asda", utils::replace("asda", "", "123"));
    EXPECT_EQ("asda", utils::replace("asda", "", "123", std::string("asda").size()));
    EXPECT_EQ("aaaa", utils::replace("aaaa", "a", "b", 0, 0));
  }

  {
    // Easy replacements
    EXPECT_EQ("Hello World", utils::replace("HeLLo WorLd", "L", "l"));
    EXPECT_EQ("Hello World", utils::replace("Jello World", "Jello", "Hello"));
    EXPECT_EQ("Eat an apple!", utils::replace("Eat an orange?", "orange?", "apple!"));
  }

  {
    // Indexing replacements, number of occurrences, and multiple occurrences
    EXPECT_EQ("cow cow dog cow", utils::replace("bear bear dog bear", "bear", "cow"));
    EXPECT_EQ("bear cow dog cow", utils::replace("bear bear dog bear", "bear", "cow", 1));
    EXPECT_EQ("bear cow dog bear", utils::replace("bear bear dog bear", "bear", "cow", 1, 1));
    EXPECT_EQ("cow bear dog bear", utils::replace("bear bear dog bear", "bear", "cow", 0, 1));
    EXPECT_EQ("bear bear dog bear", utils::replace("bear bear dog bear", "bear", "cow", 0, 0));
  }
}

/** @todo [KEMB-628] There must be a way to combine the split(), lsplit(), and rsplit() nominal cases into a
 *  test that's able to use std::function() so we can loop over the different implementations.
 */

/** Tests that we are able to take a string and split it into separate parts based on a delimiter.
 */
TEST(UtilTests, StringSplit) {
  // Easy access local variable
  std::vector<std::string> strs;

  // Edge cases
  {
    strs = utils::split("");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::split("", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::split("", "more complex");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::split("hello,world", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);

    strs = utils::split("hello,world", ",", 0);
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  {
    for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
      std::map<std::string, std::vector<std::string>> tests = {
          {"~", {"", ""}},
          {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
          {"hello~world", {"hello", "world"}},
          {"embrace~the dark~ side", {"embrace", "the dark", " side"}},
          {"ThE Empire~DiD NothiNG WRONg!", {"ThE Empire", "DiD NothiNG WRONg!"}},
      };

      // Iterate over the different test cases
      for(const auto& test : tests) {
        // Create a new string by using substitution to replace the deliminter
        const auto new_str = utils::replace(test.first, "~", delim);
        ASSERT_NE(test.first, new_str);
        // Use that new string and delimiter to split it up
        strs = utils::split(utils::replace(new_str, "~", delim), delim);
        ASSERT_EQ(test.second.size(), strs.size());
        for(auto i = 0U; i < test.second.size(); ++i) {
          EXPECT_EQ(test.second[i], strs[i]);
        }
      }
    }
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  // and the number of occurances is set to something less than the actual number of occurances.
  {
    for(auto count : {0U, 1U, 2U, 3U}) {
      for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
        std::map<std::string, std::vector<std::string>> tests = {
            {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
            {"hello~world~need~more", {"hello", "world", "need", "more"}},
            {"embrace~the ~dark~ side", {"embrace", "the ", "dark", " side"}},
            {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE", " Empire", "DiD NothiNG ", "WRONg!"}},
        };

        // Iterate over the different test cases
        for(const auto& test : tests) {
          // Create a new string by using substitution to replace the deliminter
          const auto new_str = utils::replace(test.first, "~", delim);
          ASSERT_NE(test.first, new_str);
          // Use that new string and delimiter to split it up
          strs = utils::split(utils::replace(new_str, "~", delim), delim, count);
          ASSERT_EQ(count + 1, strs.size());

          // @todo [KEMB-628] This should really validate the generated output ... I'm feeling lazy at the moment
        }
      }
    }
  }

  // Split the test strings using a differing number of occurances and validate the results.
  {
    std::map<std::string, std::vector<std::string>> tests = {
        {"A~B~C~D~E~F", {"A", "B", "C~D~E~F"}},
        {"hello~world~need~more", {"hello", "world", "need~more"}},
        {"embrace~the ~dark~ side", {"embrace", "the ~dark~ side"}},
        {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE", " Empire", "DiD NothiNG ~WRONg!"}},
    };

    // Iterate over the different test cases
    for(const auto& test : tests) {
      strs = utils::lsplit(test.first, "~", test.second.size() - 1);
      ASSERT_EQ(test.second.size(), strs.size());
      for(auto i = 0U; i < test.second.size(); ++i) {
        EXPECT_EQ(test.second[i], strs[i]);
      }
    }
  }
}

/** Tests that we are able to take a string and split it into separate parts based on a delimiter
 *  starting from the right side.
 */
TEST(UtilTests, StringRSplit) {
  // Easy access local variable
  std::vector<std::string> strs;

  // Edge cases
  {
    strs = utils::rsplit("");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::rsplit("", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::rsplit("", "more complex");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::rsplit("hello,world", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);

    strs = utils::rsplit("hello,world", ",", 0);
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  {
    for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
      std::map<std::string, std::vector<std::string>> tests = {
          {"~", {"", ""}},
          {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
          {"hello~world", {"hello", "world"}},
          {"embrace~the dark~ side", {"embrace", "the dark", " side"}},
          {"ThE Empire~DiD NothiNG WRONg!", {"ThE Empire", "DiD NothiNG WRONg!"}},
      };

      // Iterate over the different test cases
      for(const auto& test : tests) {
        // Create a new string by using substitution to replace the deliminter
        const auto new_str = utils::replace(test.first, "~", delim);
        ASSERT_NE(test.first, new_str);
        // Use that new string and delimiter to split it up
        strs = utils::rsplit(utils::replace(new_str, "~", delim), delim);
        ASSERT_EQ(test.second.size(), strs.size());
        for(auto i = 0U; i < test.second.size(); ++i) {
          EXPECT_EQ(test.second[i], strs[i]);
        }
      }
    }
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  // and the number of occurances is set to something less than the actual number of occurances.
  {
    for(auto count : {0U, 1U, 2U, 3U}) {
      for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
        std::map<std::string, std::vector<std::string>> tests = {
            {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
            {"hello~world~need~more", {"hello", "world", "need", "more"}},
            {"embrace~the ~dark~ side", {"embrace", "the ", "dark", " side"}},
            {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE", " Empire", "DiD NothiNG ", "WRONg!"}},
        };

        // Iterate over the different test cases
        for(const auto& test : tests) {
          // Create a new string by using substitution to replace the deliminter
          const auto new_str = utils::replace(test.first, "~", delim);
          ASSERT_NE(test.first, new_str);
          // Use that new string and delimiter to split it up
          strs = utils::rsplit(utils::replace(new_str, "~", delim), delim, count);
          ASSERT_EQ(count + 1, strs.size());

          // @todo [KEMB-628] This should really validate the generated output ... I'm feeling lazy at the moment
        }
      }
    }
  }

  // Split the test strings using a differing number of occurances and validate the results.
  {
    std::map<std::string, std::vector<std::string>> tests = {
        {"A~B~C~D~E~F", {"A~B~C~D", "E", "F"}},
        {"hello~world~need~more", {"hello~world", "need", "more"}},
        {"embrace~the ~dark~ side", {"embrace~the ~dark", " side"}},
        {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE~ Empire", "DiD NothiNG ", "WRONg!"}},
    };

    // Iterate over the different test cases
    for(const auto& test : tests) {
      strs = utils::rsplit(test.first, "~", test.second.size() - 1);
      ASSERT_EQ(test.second.size(), strs.size());
      for(auto i = 0U; i < test.second.size(); ++i) {
        EXPECT_EQ(test.second[i], strs[i]);
      }
    }
  }
}

/** Tests that we are able to take a string and split it into separate parts based on a delimiter
 *  starting from the left side.
 */
TEST(UtilTests, StringLSplit) {
  // Easy access local variable
  std::vector<std::string> strs;

  // Edge cases
  {
    strs = utils::lsplit("");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::lsplit("", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::lsplit("", "more complex");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("", strs[0]);

    strs = utils::lsplit("hello,world", "");
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);

    strs = utils::lsplit("hello,world", ",", 0);
    ASSERT_EQ(1U, strs.size());
    EXPECT_EQ("hello,world", strs[0]);
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  {
    for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
      std::map<std::string, std::vector<std::string>> tests = {
          {"~", {"", ""}},
          {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
          {"hello~world", {"hello", "world"}},
          {"embrace~the dark~ side", {"embrace", "the dark", " side"}},
          {"ThE Empire~DiD NothiNG WRONg!", {"ThE Empire", "DiD NothiNG WRONg!"}},
      };

      // Iterate over the different test cases
      for(const auto& test : tests) {
        // Create a new string by using substitution to replace the deliminter
        const auto new_str = utils::replace(test.first, "~", delim);
        ASSERT_NE(test.first, new_str);
        // Use that new string and delimiter to split it up
        strs = utils::lsplit(utils::replace(new_str, "~", delim), delim);
        ASSERT_EQ(test.second.size(), strs.size());
        for(auto i = 0U; i < test.second.size(); ++i) {
          EXPECT_EQ(test.second[i], strs[i]);
        }
      }
    }
  }

  // Test a bunch of cases where the delim is swapped out with a couple of different delims
  // and the number of occurances is set to something less than the actual number of occurances.
  {
    for(auto count : {0U, 1U, 2U, 3U}) {
      for(const auto delim : {",", ";", "my awesome delimiter", "/", "!@#%@#$"}) {
        std::map<std::string, std::vector<std::string>> tests = {
            {"A~B~C~D~E~F", {"A", "B", "C", "D", "E", "F"}},
            {"hello~world~need~more", {"hello", "world", "need", "more"}},
            {"embrace~the ~dark~ side", {"embrace", "the ", "dark", " side"}},
            {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE", " Empire", "DiD NothiNG ", "WRONg!"}},
        };

        // Iterate over the different test cases
        for(const auto& test : tests) {
          // Create a new string by using substitution to replace the deliminter
          const auto new_str = utils::replace(test.first, "~", delim);
          ASSERT_NE(test.first, new_str);
          // Use that new string and delimiter to split it up
          strs = utils::lsplit(utils::replace(new_str, "~", delim), delim, count);
          ASSERT_EQ(count + 1, strs.size());

          // @todo [KEMB-628] This should really validate the generated output ... I'm feeling lazy at the moment
        }
      }
    }
  }

  // Split the test strings using a differing number of occurances and validate the results.
  {
    std::map<std::string, std::vector<std::string>> tests = {
        {"A~B~C~D~E~F", {"A", "B", "C~D~E~F"}},
        {"hello~world~need~more", {"hello", "world", "need~more"}},
        {"embrace~the ~dark~ side", {"embrace", "the ~dark~ side"}},
        {"ThE~ Empire~DiD NothiNG ~WRONg!", {"ThE", " Empire", "DiD NothiNG ~WRONg!"}},
    };

    // Iterate over the different test cases
    for(const auto& test : tests) {
      strs = utils::lsplit(test.first, "~", test.second.size() - 1);
      ASSERT_EQ(test.second.size(), strs.size());
      for(auto i = 0U; i < test.second.size(); ++i) {
        EXPECT_EQ(test.second[i], strs[i]);
      }
    }
  }
}

/** Tests that we are able to extract the path from an incoming string.
 */
TEST(UtilTests, DirectoryAndFilename) {
  // Edge cases
  EXPECT_EQ("", utils::get_filename(""));
  EXPECT_EQ("", utils::get_directory(""));

  // Check when it just includes a filename
  for(const auto& s : {"a", "2323", "!#@*%(@#$", "hello.txt", "love\\the\\windows\\line\\endings"}) {
    EXPECT_EQ(s, utils::get_filename(s));
    EXPECT_EQ("./", utils::get_directory(s));
  }

  // Check when it just includes a directory
  for(const std::string& s : {"/", "/hello/", "why/do/this/", "123/-234234\\ed/123123/"}) {
    EXPECT_EQ("", utils::get_filename(s));
    EXPECT_EQ(s, utils::get_directory(s));
    // Check when it includes a directory and a filename
    for(const std::string& f : {"file", "another.txt", "\\because windows", "  123 @#$*@# sdfS"}) {
      EXPECT_EQ(f, utils::get_filename(s + f));
      EXPECT_EQ(s, utils::get_directory(s + f, "/", true));
      EXPECT_EQ(s.substr(0, s.size() - 1), utils::get_directory(s + f));
    }
  }
}

/** Tests that we are able to take strings and join them together into an output string.
 */
TEST(UtilTests, StringJoin) {
  // Edge cases
  EXPECT_EQ("", utils::join({}));
  EXPECT_EQ("", utils::join({}, ","));
  EXPECT_EQ("", utils::join({""}));
  EXPECT_EQ("", utils::join({""}, ","));
  EXPECT_EQ("hello", utils::join({"hello"}));
  EXPECT_EQ("hello", utils::join({"hello"}, ","));

  // Join some strings togther
  EXPECT_EQ("hello/world/this/is/you", utils::join({"hello", "world", "this", "is", "you"}));
  EXPECT_EQ("hello,world,this,is,you", utils::join({"hello", "world", "this", "is", "you"}, ","));
  EXPECT_EQ("hello123world123this123is123you", utils::join({"hello", "world", "this", "is", "you"}, "123"));
  EXPECT_EQ("hello\tworld\tthis\tis\tyou", utils::join({"hello", "world", "this", "is", "you"}, "\t"));

  EXPECT_EQ("/hello/world/this/is/you", utils::join({"", "hello", "world", "this", "is", "you"}, "/"));
}

/** Tests that we are able to convert times to strings
 */
TEST(UtilTests, ConvertTime) {
  auto then = static_cast<time_t>(1545256222);
  auto now = std::time(nullptr);

  // Sanity check the parameters
  EXPECT_EQ("", utils::convert_time(nullptr));
  EXPECT_EQ("", utils::convert_time(std::gmtime(&now), ""));

  const std::vector<std::string> formats = {"%Y-%m-%d %H:%M:%S", "hello", "%Y", "%M some %m other %H"};

  // Validate using a precomputed time (statis)
  const std::vector<std::string> const_times = {"2018-12-19 21:50:22", "hello", "2018", "50 some 12 other 21"};
  for(auto i = 0U; i < formats.size(); ++i) {
    EXPECT_EQ(const_times[i], utils::convert_time(std::gmtime(&then), formats[i]));
  }

  // Validate using the current system time (dynamic)
  for(const auto& format : formats) {
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&now), format.c_str());
    EXPECT_EQ(ss.str(), utils::convert_time(std::gmtime(&now), format));
  }
}

/** Tests that we are able to erase elements from a map.
 */
TEST(UtilTests, MapErase) {
  const std::map<std::string, int> sample = {
      {"hello", 1}, {"world", 2}, {"the Empire", 100}, {"did", 22}, {"nothing", -123}, {"wrong", 20},
  };

  // Check edge cases
  {
    auto copy = sample;
    EXPECT_EQ(sample.size(), copy.size());

    // Remove nothing
    utils::erase_if(copy, [](decltype(sample)::value_type&) -> bool { return false; });
    EXPECT_EQ(sample.size(), copy.size());
    // Remove everything
    utils::erase_if(copy, [](decltype(sample)::value_type&) -> bool { return true; });
    EXPECT_EQ(0U, copy.size());
  }

  // Check that we can remove items based on the key
  {
    auto copy = sample;
    EXPECT_EQ(sample.size(), copy.size());

    // Single element erase
    utils::erase_if(
        copy, [](decltype(sample)::value_type& pair) -> bool { return utils::is_equal("the Empire", pair.first); });
    EXPECT_EQ(sample.size() - 1U, copy.size());

    // Multi-element erase
    utils::erase_if(copy, [](decltype(sample)::value_type& pair) -> bool { return utils::contains(pair.first, "l"); });
    EXPECT_EQ(sample.size() - 3U, copy.size());
  }

  // Check that we can remove items based on the value
  {
    auto copy = sample;
    EXPECT_EQ(sample.size(), copy.size());

    // Single element erase
    utils::erase_if(copy, [](decltype(sample)::value_type& pair) -> bool { return pair.second == 100; });
    EXPECT_EQ(sample.size() - 1U, copy.size());

    // Multi-element erase
    utils::erase_if(copy, [](decltype(sample)::value_type& pair) -> bool { return pair.second >= 22; });
    EXPECT_EQ(sample.size() - 2U, copy.size());
  }

  // Check that we can remove items based on the key and value
  {
    auto copy = sample;
    EXPECT_EQ(sample.size(), copy.size());

    // Single element erase
    utils::erase_if(copy, [](decltype(sample)::value_type& pair) -> bool {
      return utils::contains(pair.first, "the") && pair.second == 100;
    });
    EXPECT_EQ(sample.size() - 1U, copy.size());

    // Multi-element erase
    utils::erase_if(copy, [](decltype(sample)::value_type& pair) -> bool {
      return utils::contains(pair.first, "d") || pair.second == 20;
    });
    EXPECT_EQ(sample.size() - 4U, copy.size());
  }
}
