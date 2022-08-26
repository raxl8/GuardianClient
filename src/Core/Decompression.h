#pragma once

unsigned int stb_decompress_length(const unsigned char* input);
unsigned int stb_decompress(unsigned char* output, const unsigned char* i, unsigned int /*length*/);
