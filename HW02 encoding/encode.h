#ifndef OTUS_XOXLOV_ENCODE_H
#define OTUS_XOXLOV_ENCODE_H

#define ASCII_CONSTANT        128

extern char *encodings[];

int16_t* recognizeEncodingTable(char* );
size_t getEncodingsCount();

#endif //OTUS_XOXLOV_ENCODE_H
