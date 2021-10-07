struct page
{
    const char *title;
    const char *content;
};

#include "pages_data.hpp"

unsigned int n_pages = sizeof(pages) / sizeof(pages[0]);
const unsigned int n_bigpages = (n_pages + 7) / 8;
