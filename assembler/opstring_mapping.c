/* C code produced by gperf version 3.1 */
/* Command-line: gperf --output-file opstring_mapping.c gperf_input  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 8 "gperf_input"
struct opstring {char * name; unsigned char opcode; };
#include <string.h>

#define TOTAL_KEYWORDS 15
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 24
/* maximum key range = 22, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
opstring_hash (str, len)
     register const char *str;
     register size_t len;
{
  static const unsigned char asso_values[] =
    {
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 10, 25, 10,
       9,  0, 25, 25,  5,  5,  5, 25, 25, 15,
      10,  0,  5, 25,  0, 25, 25,  0, 25,  0,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

static const struct opstring opstrings[] =
  {
    {""}, {""}, {""},
#line 18 "gperf_input"
    {"ret",    8},
#line 19 "gperf_input"
    {"read",   9},
#line 20 "gperf_input"
    {"write",  10},
    {""},
#line 21 "gperf_input"
    {"op",     11},
#line 10 "gperf_input"
    {"pop",    0},
#line 12 "gperf_input"
    {"push",   2},
#line 14 "gperf_input"
    {"push#",  4},
    {""},
#line 23 "gperf_input"
    {"dup",    13},
#line 22 "gperf_input"
    {"rnd",    12},
#line 11 "gperf_input"
    {"ipop",   1},
#line 13 "gperf_input"
    {"ipush",  3},
    {""}, {""},
#line 16 "gperf_input"
    {"jnz",    6},
#line 24 "gperf_input"
    {"halt",   99},
    {""}, {""}, {""},
#line 15 "gperf_input"
    {"jmp",    5},
#line 17 "gperf_input"
    {"call",   7}
  };

const struct opstring *
in_word_set (str, len)
     register const char *str;
     register size_t len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = opstring_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = opstrings[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &opstrings[key];
        }
    }
  return 0;
}


// return 255 if not in set and corresponding opcode if in set
unsigned char opstring_to_opcode(const char * string, size_t len){

  unsigned char res;
  const struct opstring * p = in_word_set(string, len);

  res = ( ! p ) ? 255 : p->opcode;

  return res;
}
