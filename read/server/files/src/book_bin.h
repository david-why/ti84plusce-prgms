/* book_bin.h: NO structures, just data[] */

// #pragma pack(push, 1)
// struct book_header
// {
//     uint8_t symbol_height;
//     uint8_t symbol_width;
//     uint8_t line_space;
//     uint8_t page_rows;
//     uint8_t page_cols;
//     uint16_t symbols;
//     uint16_t pages;
// };
// #pragma pack(pop)
// uint8_t symbol_set[head.symbols][head.symbol_height];
// uint8_t page_set[head.pages][head.page_rows][head.page_cols];

// uint8_t data[] = {16, 8, 3, 3, 2, 0, 3, 0, 2,
//                   // Symbol set
//                   0, 0, 0, 0, 0, 0, 0, 56, 68, 64, 124, 66, 66, 124, 0, 0,
//                   16, 16, 16, 136, 136, 76, 44, 10, 137, 136, 72, 72, 40, 8, 8, 8,
//                   1, 1, 1, 127, 64, 32, 4, 4, 20, 36, 36, 68, 68, 4, 5, 2,
//                   // Page set
//                   1, 0x80, 2,
//                   1, 1, 1,
//                   0x80, 2, 0,
//                   0, 0, 0};
uint8_t data[] = {
#include "data.h"
};

#define init_book() true
#define deinit_book()
uint8_t get_height()
{
    return data[0];
}
uint8_t get_width()
{
    return data[1];
}
uint8_t get_space()
{
    return data[2];
}
uint8_t get_cols()
{
    return data[4];
}
uint8_t get_rows()
{
    return data[3];
}
uint16_t get_symbols()
{
    return (((uint16_t)data[5]) << 8) + data[6];
}
uint16_t get_pages()
{
    return (((uint16_t)data[7]) << 8) + data[8];
}
uint8_t get_symbol_row(uint16_t sym, uint8_t row)
{
    return data[9 + sym * get_height() + row];
}
uint8_t get_char(uint32_t page, uint8_t row, uint8_t col)
{
    return data[9 + (uint32_t)get_symbols() * get_height() + page * get_rows() * get_cols() + row * get_cols() + col];
}
