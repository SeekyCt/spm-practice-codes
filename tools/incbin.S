/*
    Based on https://github.com/stblr/mkw-sp/blob/main/Incbin.S
*/

#define STRINGIZE2(a) #a
#define STRINGIZE(a) STRINGIZE2(a)

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)

#define END_SYMBOL CONCAT(CONCAT(.L, NAME), _end)
#define SIZE_SYMBOL CONCAT(NAME, _size)

// Asset data
    .section SECTION.NAME
    .balign ALIGN
    .global NAME
    .type NAME, @object
NAME:
    .incbin STRINGIZE(PATH)
END_SYMBOL:
    .size NAME, . - NAME

// Asset size
    .section SECTION.SIZE_SYMBOL
    .balign 0x4
    .global SIZE_SYMBOL
SIZE_SYMBOL:
    .4byte END_SYMBOL - NAME
    .size CONCAT(NAME, Size), 0x4
