#ifndef OTUS_XOXLOV_ENCODE_H
#define OTUS_XOXLOV_ENCODE_H

#define MAX_FILE_NAME_LEN     255
#define MAX_ENCODING_NAME_LEN 10
#define ASCII_CONSTANT        128
#define LOW_BYTE_MASK         0x3F // b00111111
#define LOW_BYTE_SHIFT        6
#define LOW_BYTE_HEAD         0x80 // b10000000
#define HIGH_BYTE_MASK        0x3F // b00111111
#define HIGH_BYTE_HEAD        0xC0 // b11000000

extern char *encodings[];

int16_t* recognizeEncodingTable(char* );
size_t getEncodingsCount();

#endif //OTUS_XOXLOV_ENCODE_H
