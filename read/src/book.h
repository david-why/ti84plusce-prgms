/* book.h: structures */

#pragma pack(push, 1)
struct book_header
{
    uint8_t symbol_height;
    uint8_t symbol_width;
    uint8_t line_space;
    uint8_t page_cols;
    uint8_t page_rows;
    uint16_t symbols;
    uint16_t pages;
};
#pragma pack(pop)

constexpr book_header head{16, 8, 3, 40, 12, 3, 2};

uint8_t symbol_set[head.symbols][head.symbol_height] = {{0, 0, 0, 0, 0, 0, 0, 56, 68, 64, 124, 66, 66, 124, 0, 0}, {16, 16, 16, 136, 136, 76, 44, 10, 137, 136, 72, 72, 40, 8, 8, 8}, {1, 1, 1, 127, 64, 32, 4, 4, 20, 36, 36, 68, 68, 4, 5, 2}};
uint8_t page_set[head.pages][head.page_rows][head.page_cols] = {{{1, 0x80, 2}, {1, 1, 1}}, {{0x80, 2}}};

#define init_book(...) true
#define deinit_book()
#define n_books() (1)
#define get_names() (NULL)
uint8_t get_height()
{
    return head.symbol_height;
}
uint8_t get_width()
{
    return head.symbol_width;
}
uint8_t get_space()
{
    return head.line_space;
}
uint8_t get_cols()
{
    return head.page_cols;
}
uint8_t get_rows()
{
    return head.page_rows;
}
uint16_t get_pages()
{
    return head.pages;
}
uint8_t get_symbol_row(uint16_t sym, uint8_t row)
{
    return symbol_set[sym][row];
}
uint8_t get_char(uint16_t page, uint8_t row, uint8_t col)
{
    return page_set[page][row][col];
}
