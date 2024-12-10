#include <Arduino.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define add Ed25519_add
#define sub Ed25519_sub
#define modulo Ed25519_modulo

typedef struct { unsigned char Ed[32]; } field_element;

extern "C"
{
  void sub(field_element *r, const field_element *x, const field_element *y);
  void add(field_element *r, const field_element *x, const field_element *y);
  void modulo(field_element *r, unsigned char *C);
  char Ed_num_sub_prime(unsigned char* r, const unsigned char* a);
  char Ed25519_square(unsigned char* r, const unsigned char* a);
  char Ed_mul(unsigned char* r, const unsigned char* a, const unsigned char* b);
  unsigned char scalar_sub_halforder(unsigned char* r, const unsigned char* a);
  unsigned char scalar_sub_order(unsigned char* r, const unsigned char* a);
  void Ed255_subp_bigint(unsigned char* r, const unsigned char* a);
}

void in_range(field_element *r);
void Ed25519_scalar_sub_order(field_element *r);
void flip_if(field_element *r, const field_element *x, unsigned char b);
void mul(field_element *r, const field_element *x, const field_element *y);
void square(field_element *r, const field_element *x);
void M_Inverse_Z(field_element *r, const field_element *x);

static const field_element PRECOMPUTED_POINTS[17][4] PROGMEM = {
  {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{0x1A, 0xD5, 0x25, 0x8F, 0x60, 0x2D, 0x56, 0xC9, 0xB2, 0xA7, 0x25, 0x95, 0x60, 0xC7, 0x2C, 0x69, 0x5C, 0xDC, 0xD6, 0xFD, 0x31, 0xE2, 0xA4, 0xC0, 0xFE, 0x53, 0x6E, 0xCD, 0xD3, 0x36, 0x69, 0x21},
   {0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66},
   {0xA3, 0xDD, 0xB7, 0xA5, 0xB3, 0x8A, 0xDE, 0x6D, 0xF5, 0x52, 0x51, 0x77, 0x80, 0x9F, 0xF0, 0x20, 0x7D, 0xE3, 0xAB, 0x64, 0x8E, 0x4E, 0xEA, 0x66, 0x65, 0x76, 0x8B, 0xD7, 0x0F, 0x5F, 0x87, 0x67},
   {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {{0x57, 0x0A, 0x30, 0x18, 0x4C, 0x86, 0xBA, 0x7A, 0x5B, 0x62, 0x71, 0xFA, 0xDD, 0xC1, 0x6E, 0x88, 0x09, 0xF9, 0xB0, 0xB7, 0x87, 0x4B, 0x22, 0xEA, 0x4A, 0x25, 0x7C, 0x9A, 0xDB, 0x1D, 0x24, 0x31},
   {0xD7, 0x7F, 0x2D, 0x5E, 0xD9, 0x2C, 0x4B, 0x92, 0x1C, 0x32, 0xBE, 0x2B, 0x66, 0xAA, 0x47, 0x0A, 0xF6, 0x51, 0xC9, 0x3A, 0xFC, 0x82, 0xB7, 0x69, 0xDB, 0x33, 0xC9, 0x6C, 0x4C, 0x98, 0x24, 0x33},
   {0x99, 0x49, 0x4D, 0x1B, 0x07, 0x4E, 0xB3, 0x0E, 0x19, 0xBB, 0xF0, 0xED, 0xBF, 0xDA, 0x62, 0xA3, 0x0F, 0x7E, 0xA8, 0x48, 0x40, 0x56, 0xE4, 0x40, 0x95, 0x9C, 0xB5, 0x74, 0xC9, 0x7D, 0x24, 0x18},
   {0x6B, 0xDE, 0x78, 0x02, 0x3D, 0xA9, 0x74, 0x63, 0xD1, 0x99, 0x92, 0x33, 0x00, 0x61, 0xD3, 0xEB, 0x92, 0x62, 0x42, 0x3B, 0xFA, 0xD1, 0x98, 0x40, 0x57, 0x7C, 0x57, 0x6B, 0x79, 0xC5, 0x86, 0x69}},
  {{0x03, 0xF3, 0xC3, 0x65, 0xCE, 0x01, 0xBC, 0x7F, 0xC2, 0xC3, 0x5B, 0x4B, 0x34, 0xA5, 0xBC, 0xCF, 0x2A, 0xCB, 0x78, 0x8E, 0x75, 0xF9, 0x85, 0xFA, 0x40, 0x69, 0x8B, 0xF1, 0x4E, 0xB9, 0xB9, 0x26},
   {0x37, 0x76, 0x2A, 0xF6, 0x8D, 0xA7, 0xCF, 0x72, 0xDB, 0xE3, 0x2A, 0xE6, 0xD9, 0xB7, 0xD9, 0x4D, 0x7C, 0x18, 0x88, 0xF0, 0xC6, 0x8D, 0x05, 0xD3, 0xAA, 0x4C, 0x6A, 0x12, 0x37, 0x9B, 0x5C, 0x19},
   {0xE0, 0xD8, 0xDE, 0x5F, 0x06, 0xF6, 0x5D, 0xCC, 0x1A, 0x64, 0x14, 0x87, 0x04, 0xFB, 0x2F, 0x3F, 0xB3, 0x07, 0x91, 0x58, 0x08, 0x7F, 0x87, 0x92, 0xE1, 0xCC, 0xF1, 0xBB, 0xB2, 0xB5, 0xBE, 0x3B},
   {0xAF, 0x92, 0xE1, 0xC1, 0xF3, 0xC3, 0x6E, 0x7B, 0x3E, 0x10, 0x7E, 0xB1, 0x76, 0xBF, 0xCD, 0x4A, 0xE3, 0x07, 0xFD, 0x0E, 0x09, 0xC3, 0xC6, 0xF5, 0xB2, 0x86, 0x92, 0xA6, 0xBE, 0xA8, 0xDC, 0x1D}},
  {{0x95, 0x05, 0x02, 0xE9, 0x8E, 0xA7, 0xAD, 0x05, 0xDB, 0x30, 0x01, 0x98, 0x1B, 0xE1, 0x3E, 0xD8, 0xBB, 0x7F, 0xF7, 0x52, 0x34, 0xBF, 0x3F, 0xE9, 0x95, 0x9F, 0xD1, 0x92, 0x90, 0xE2, 0xD6, 0x7A},
   {0xDC, 0x08, 0xB3, 0x80, 0x79, 0x52, 0x3C, 0x4C, 0x0F, 0x29, 0x4C, 0xC4, 0xD6, 0x3D, 0x64, 0xBE, 0x4D, 0x48, 0x9F, 0xEA, 0x85, 0x11, 0xD8, 0x39, 0x35, 0xAD, 0x90, 0x00, 0xAE, 0x83, 0xC9, 0x12},
   {0xB6, 0x92, 0xBD, 0xE3, 0xE9, 0x76, 0xA5, 0x6E, 0x4A, 0x2E, 0xBE, 0x71, 0x10, 0x09, 0xC2, 0x42, 0x4C, 0x27, 0x27, 0x00, 0x75, 0x98, 0x67, 0x1A, 0x26, 0x58, 0x30, 0xB0, 0x19, 0x07, 0xC3, 0x48},
   {0xB8, 0xEB, 0x15, 0x44, 0x11, 0x3C, 0x49, 0xE2, 0xC2, 0xDC, 0x77, 0x4C, 0xD6, 0x9A, 0x50, 0xDC, 0xFF, 0xB8, 0xD8, 0xA0, 0xD8, 0xD0, 0x48, 0x15, 0xB1, 0xDD, 0x0A, 0x32, 0x96, 0x6B, 0x6E, 0x78}},
  {{0x9F, 0x2A, 0xB2, 0xFF, 0x0B, 0xB2, 0x9A, 0x1D, 0xC3, 0x1C, 0xCA, 0x28, 0x03, 0xA1, 0x11, 0x9A, 0xFE, 0xCE, 0xA0, 0xE7, 0xB6, 0xED, 0x54, 0x19, 0x52, 0xE6, 0xAB, 0x00, 0x7F, 0xA2, 0xFD, 0x75},
   {0x9B, 0xE2, 0x3B, 0x76, 0x77, 0x92, 0x59, 0xF6, 0x10, 0x2A, 0xBC, 0x07, 0x0A, 0x79, 0x5D, 0xC4, 0x16, 0x24, 0x57, 0x7E, 0xF8, 0x09, 0xD2, 0x02, 0x55, 0x6B, 0x56, 0x00, 0xD8, 0x8A, 0xB2, 0x17},
   {0x04, 0xF7, 0x4A, 0x1D, 0x55, 0x14, 0x88, 0x99, 0xF3, 0xB7, 0x01, 0x38, 0xCD, 0x83, 0x27, 0x2E, 0xA9, 0xE3, 0x53, 0x25, 0x85, 0x99, 0xC1, 0x7C, 0xA1, 0xCA, 0xA4, 0xE1, 0x59, 0xA8, 0x87, 0x1A},
   {0xC8, 0x01, 0x08, 0xFE, 0x89, 0xB4, 0xFD, 0xB8, 0x1B, 0xE1, 0xFD, 0x6F, 0xEC, 0xC6, 0xA7, 0x6C, 0x6E, 0x44, 0x4D, 0xD6, 0x31, 0x9B, 0x09, 0x55, 0x70, 0x67, 0x07, 0xEA, 0x42, 0xD6, 0x74, 0x65}},
  {{0xB6, 0xF7, 0xFE, 0xBD, 0xBB, 0x1E, 0x33, 0x0B, 0x2B, 0xD9, 0x92, 0x9C, 0x4F, 0x4F, 0x78, 0x5B, 0x44, 0x79, 0x8C, 0xD7, 0x12, 0x6F, 0x36, 0x50, 0x66, 0x49, 0xC3, 0xFB, 0x85, 0xC7, 0xCB, 0x34},
   {0x3C, 0x8C, 0x1C, 0xDA, 0xF8, 0x97, 0x7F, 0xF8, 0x71, 0x5C, 0xB4, 0x80, 0x6C, 0xE4, 0x4C, 0x5E, 0xE5, 0x26, 0x96, 0x76, 0xD2, 0xD3, 0x99, 0x07, 0xF7, 0x1F, 0x91, 0x2D, 0xD4, 0x0C, 0xBE, 0x43},
   {0xA8, 0xA7, 0x11, 0x70, 0xE8, 0x7D, 0xF1, 0xCB, 0x7D, 0xA2, 0x9A, 0xFF, 0x79, 0xE6, 0x1F, 0xB0, 0xAD, 0x5F, 0xD3, 0x54, 0x2A, 0x7A, 0x67, 0x85, 0x31, 0x1C, 0x07, 0x12, 0x13, 0xEF, 0x73, 0x14},
   {0x09, 0x9D, 0x59, 0xB8, 0x1C, 0x48, 0xBA, 0x1D, 0x4D, 0x9F, 0xFC, 0x16, 0x46, 0x20, 0xD2, 0xB5, 0x01, 0x6D, 0x76, 0x59, 0xDA, 0xA8, 0xF6, 0x1C, 0x31, 0xAD, 0x77, 0xB9, 0x4C, 0xCA, 0x26, 0x76}},
  {{0x7E, 0xE1, 0x42, 0x35, 0x39, 0xCE, 0x91, 0x79, 0x73, 0x6C, 0x40, 0x9F, 0xE6, 0x78, 0xD1, 0xBD, 0x2B, 0x82, 0x9F, 0xE8, 0x9A, 0x84, 0x37, 0x6E, 0x32, 0x27, 0xDE, 0x7A, 0x58, 0x52, 0x6F, 0x14},
   {0x95, 0x4C, 0xCC, 0x7B, 0x7F, 0x99, 0x1B, 0xCB, 0xC0, 0xDA, 0x63, 0x5D, 0x7C, 0xC9, 0xE1, 0x95, 0x0D, 0x0A, 0x9C, 0xA5, 0x22, 0x65, 0xAC, 0xBF, 0xA3, 0x88, 0x72, 0x08, 0x5A, 0xF0, 0x3C, 0x6D},
   {0x15, 0x2F, 0x60, 0x86, 0x3B, 0xAD, 0x4A, 0x64, 0xDF, 0x10, 0xAC, 0x8A, 0x6C, 0xED, 0x26, 0xF7, 0x9F, 0xD3, 0xBB, 0xA1, 0xCA, 0x3B, 0x05, 0x02, 0xA6, 0xE7, 0x6B, 0xD6, 0xDB, 0x0E, 0xC4, 0x55},
   {0xED, 0x41, 0xC1, 0xA5, 0x5A, 0xF0, 0xFD, 0x89, 0x7E, 0x18, 0x25, 0xFA, 0xEF, 0xC8, 0xF6, 0x4C, 0x14, 0x95, 0x44, 0xE9, 0xBC, 0xE3, 0x0F, 0xEB, 0x88, 0x44, 0xA5, 0xB6, 0x6B, 0xDB, 0x0F, 0x41}},
  {{0xDA, 0xB5, 0x9D, 0xD9, 0x91, 0x10, 0x80, 0xA0, 0x58, 0xA6, 0xEC, 0xEA, 0x67, 0x15, 0xD5, 0x63, 0x33, 0xF0, 0xFB, 0xC0, 0xE6, 0x64, 0xE2, 0xB8, 0x88, 0x50, 0x8F, 0xAE, 0xDA, 0x89, 0x13, 0x61},
   {0x2F, 0x58, 0x70, 0xEF, 0x27, 0x1F, 0x15, 0xD1, 0x14, 0xE5, 0xE9, 0xFB, 0xA0, 0xAE, 0xE6, 0x05, 0x31, 0xA8, 0x4E, 0xA4, 0x59, 0x41, 0x0F, 0x17, 0xCF, 0xA9, 0x36, 0xF4, 0x72, 0x3B, 0x59, 0x00},
   {0xB4, 0x72, 0xC5, 0xC8, 0x58, 0xD8, 0x35, 0xF7, 0xE3, 0x22, 0xBB, 0x91, 0xBC, 0x4A, 0x6A, 0x34, 0xFB, 0x9C, 0xF4, 0xFD, 0x45, 0x6B, 0xD8, 0xB5, 0x35, 0xD2, 0x75, 0xF1, 0x63, 0xB5, 0x92, 0x5C},
   {0xEF, 0x7D, 0x91, 0x5C, 0xB3, 0x43, 0xF7, 0xB8, 0xBF, 0xBC, 0x6E, 0x5A, 0x87, 0x73, 0xF3, 0xF6, 0x72, 0x75, 0x80, 0x67, 0xB1, 0x8E, 0x70, 0xB0, 0xE5, 0xB8, 0x45, 0x30, 0xC2, 0xE5, 0x21, 0x55}},
  {{0xE8, 0xAB, 0xEA, 0x8B, 0x0F, 0x9A, 0xFA, 0x65, 0xCB, 0x7E, 0x68, 0x1E, 0xF8, 0xDF, 0xF4, 0x5A, 0xF6, 0xA4, 0xBD, 0xDC, 0x74, 0x42, 0x99, 0xE8, 0xF7, 0xA0, 0x54, 0x98, 0x57, 0xED, 0xD3, 0x4D},
   {0x37, 0x6A, 0xC4, 0xF3, 0x03, 0x2A, 0x0C, 0x27, 0xAC, 0x04, 0x0E, 0xF4, 0x70, 0xE0, 0xDB, 0xF7, 0xD3, 0x47, 0x92, 0xA7, 0x87, 0x4A, 0x41, 0xB1, 0x68, 0xF3, 0xAA, 0xF5, 0x98, 0x66, 0x81, 0x0F},
   {0x25, 0xFB, 0x4F, 0xDB, 0x5E, 0xAC, 0x89, 0xB5, 0x10, 0x97, 0x51, 0x7F, 0x65, 0x53, 0x8C, 0xF4, 0x7C, 0x40, 0xEE, 0xE8, 0x81, 0xE5, 0x29, 0xAF, 0x1A, 0xE3, 0xCE, 0xAE, 0xC7, 0x0C, 0x71, 0x5C},
   {0xE9, 0xC3, 0xFC, 0x83, 0x74, 0xF2, 0x5C, 0x30, 0x95, 0xE3, 0x28, 0x5F, 0x96, 0x8F, 0x4D, 0xB1, 0xF0, 0x8F, 0x52, 0x07, 0x29, 0x54, 0x25, 0xBA, 0x47, 0x45, 0x13, 0x41, 0x04, 0xE4, 0x83, 0x68}},
  {{0x3A, 0xA6, 0xB1, 0x19, 0x94, 0xE6, 0x83, 0xFE, 0x66, 0xC7, 0x5B, 0x85, 0x5A, 0x76, 0xAD, 0xC6, 0x0C, 0x30, 0x54, 0x92, 0x8D, 0x45, 0x06, 0x86, 0x84, 0x50, 0x8A, 0x23, 0x22, 0x12, 0xAE, 0x48},
   {0x71, 0xBB, 0xC5, 0x0F, 0x53, 0x94, 0x5C, 0xBF, 0x56, 0x01, 0x93, 0x02, 0xEE, 0xE8, 0x0C, 0x42, 0x54, 0xF8, 0x63, 0x22, 0x82, 0x36, 0x9D, 0x44, 0x1A, 0x95, 0x5E, 0x1F, 0x67, 0x3E, 0xE6, 0x1E},
   {0x7E, 0xE4, 0x79, 0xEE, 0x0B, 0xA5, 0x09, 0x90, 0xCA, 0xB3, 0xA1, 0x20, 0x23, 0xDB, 0x28, 0xA2, 0x0D, 0x7F, 0x11, 0x5D, 0x31, 0x9F, 0x16, 0xA2, 0xF6, 0x87, 0x13, 0xC5, 0x59, 0x93, 0x23, 0x16},
   {0x88, 0xB9, 0x61, 0x5E, 0x7A, 0xA5, 0x17, 0x3D, 0x41, 0xE0, 0xF6, 0x7C, 0xE2, 0x29, 0x5B, 0x69, 0xD2, 0x01, 0xB1, 0x9A, 0x5E, 0x52, 0x0B, 0x4E, 0xE4, 0x83, 0xF4, 0xB9, 0x04, 0xFC, 0xEF, 0x25}},
  {{0x1A, 0x1E, 0x9D, 0x2C, 0xC7, 0x1D, 0x79, 0x54, 0xF1, 0xC0, 0x97, 0x15, 0xDF, 0x75, 0x51, 0xC0, 0x61, 0xB6, 0xDA, 0xE2, 0xCF, 0xF6, 0x41, 0x6E, 0x39, 0xCA, 0x7E, 0x40, 0x4B, 0x16, 0xDC, 0x0B},
   {0x0B, 0x70, 0xC9, 0x22, 0xCD, 0x21, 0x69, 0x0B, 0xA5, 0x1D, 0xDD, 0x75, 0x7E, 0x35, 0xB6, 0x4B, 0x65, 0x7A, 0x34, 0xD1, 0x6B, 0x35, 0x65, 0x1D, 0xFD, 0xD9, 0xB8, 0x6E, 0x85, 0x48, 0xAA, 0x4F},
   {0x1D, 0xF2, 0x70, 0x09, 0x4D, 0x9C, 0x95, 0xDC, 0x46, 0x11, 0xD0, 0xCF, 0x13, 0x35, 0xE4, 0x04, 0xA5, 0xF5, 0xA0, 0x94, 0x52, 0x9B, 0x89, 0xAB, 0x71, 0x4F, 0x65, 0xF9, 0x5F, 0x57, 0xF7, 0x56},
   {0xA9, 0x0A, 0x2C, 0x65, 0xAC, 0x05, 0x81, 0xE5, 0x29, 0xF1, 0x57, 0xEF, 0x44, 0x78, 0x9A, 0x24, 0x5B, 0x93, 0xB2, 0x98, 0x79, 0x68, 0x38, 0x4B, 0xDC, 0xD3, 0xFF, 0x4B, 0x24, 0x33, 0x45, 0x7F}},
  {{0xDB, 0x2A, 0x19, 0xB8, 0xBC, 0x96, 0xA4, 0xB3, 0x31, 0x4F, 0x8A, 0x37, 0xC4, 0x68, 0x8F, 0xC6, 0xEB, 0x0D, 0x35, 0x99, 0x47, 0xEC, 0xC2, 0xE8, 0x67, 0x81, 0xA2, 0x27, 0x64, 0x51, 0xCC, 0x75},
   {0x61, 0xEC, 0xC2, 0xFE, 0x27, 0x86, 0x29, 0x16, 0xBC, 0xEB, 0xD4, 0xD7, 0x41, 0x03, 0x63, 0x33, 0x0D, 0x6D, 0xED, 0xE6, 0x25, 0x34, 0x39, 0x48, 0xDE, 0x98, 0x53, 0x16, 0xDE, 0x98, 0x15, 0x0B},
   {0x0B, 0xC7, 0xBF, 0x9E, 0x7D, 0xFC, 0xA7, 0x81, 0xF9, 0x2C, 0x68, 0xCE, 0xAC, 0xE5, 0xB3, 0x8B, 0x38, 0x4E, 0xEE, 0x9A, 0xE8, 0x7B, 0x21, 0xDD, 0xB9, 0xC0, 0xE4, 0xC5, 0x81, 0xB3, 0x38, 0x2E},
   {0x1C, 0x1A, 0x25, 0x4E, 0x56, 0xA6, 0x00, 0xA4, 0xA5, 0x9F, 0x71, 0xB1, 0x8D, 0x55, 0x49, 0xE8, 0xEE, 0x69, 0x3F, 0x8D, 0x23, 0x50, 0xFD, 0x4A, 0xA5, 0x57, 0x6F, 0xBB, 0x27, 0x92, 0xE7, 0x3A}},
  {{0xF2, 0xC0, 0xEC, 0x74, 0xEB, 0xA0, 0x0F, 0xB9, 0xDE, 0xF9, 0x4A, 0x9F, 0x28, 0xD2, 0x1A, 0x65, 0xC8, 0x45, 0x00, 0x89, 0x09, 0x46, 0x7F, 0xAE, 0x9A, 0xAE, 0x59, 0xCC, 0x57, 0x4E, 0xD7, 0x1A},
   {0x2E, 0xC5, 0x04, 0xF4, 0xB2, 0x5A, 0x89, 0x26, 0xFB, 0x46, 0xF2, 0xDF, 0xAE, 0xC5, 0x91, 0x66, 0xFE, 0x84, 0x6F, 0x51, 0xED, 0x85, 0xD4, 0x7D, 0x6E, 0x96, 0x81, 0xA6, 0xCB, 0x83, 0xDE, 0x49},
   {0x14, 0xE0, 0x8A, 0x4B, 0x57, 0xF7, 0x3C, 0x17, 0xFB, 0xD5, 0xF8, 0x36, 0xEA, 0x84, 0x60, 0x28, 0x4E, 0x8B, 0xA0, 0xBA, 0xDF, 0xBD, 0xD3, 0x6A, 0x30, 0x6B, 0x68, 0x02, 0xCC, 0xC2, 0x8F, 0x59},
   {0xC7, 0x70, 0x17, 0xEB, 0x23, 0x74, 0x50, 0x26, 0xA0, 0xF5, 0xA0, 0x3E, 0xB2, 0x6F, 0xC0, 0x55, 0x75, 0x13, 0xAB, 0xE5, 0x2E, 0x7F, 0x9A, 0x6A, 0x3B, 0xBA, 0x73, 0x78, 0x98, 0xBB, 0x11, 0x3E}},
  {{0x25, 0xED, 0x8C, 0xCB, 0x9F, 0x44, 0x44, 0xFA, 0x9B, 0x91, 0x6A, 0x1B, 0xE8, 0x10, 0x2A, 0x0D, 0x75, 0xA4, 0x85, 0xBF, 0x0B, 0xA8, 0xC8, 0x7E, 0x84, 0x97, 0x2D, 0xA4, 0x29, 0x48, 0x3D, 0x2E},
   {0x64, 0xD4, 0xD9, 0x6B, 0xC5, 0x3F, 0x79, 0x8A, 0x2E, 0x54, 0x32, 0x61, 0x47, 0x3C, 0x84, 0xA4, 0x60, 0x1E, 0xFC, 0x6E, 0x8E, 0xC9, 0x4E, 0x26, 0x26, 0x56, 0x44, 0x37, 0xD4, 0x8B, 0xFE, 0x06},
   {0xC6, 0x15, 0x12, 0x6C, 0x4F, 0x1A, 0xFB, 0xA8, 0x10, 0xE0, 0x28, 0x5B, 0xFD, 0x09, 0x08, 0x90, 0xE4, 0xBD, 0x05, 0x84, 0xC7, 0x5A, 0x4A, 0xB0, 0x66, 0xB2, 0xD9, 0xAA, 0x97, 0x4E, 0x7E, 0x62},
   {0x07, 0x2A, 0xB0, 0xB8, 0x4E, 0xA3, 0x27, 0xEE, 0x6A, 0x6A, 0xC9, 0xE4, 0x2A, 0x58, 0x29, 0xF0, 0x93, 0x01, 0x9B, 0xAE, 0x50, 0x0F, 0xE7, 0x89, 0x34, 0x1D, 0xF2, 0xDA, 0x10, 0x42, 0x7C, 0x60}},
  {{0x06, 0x06, 0x0E, 0x11, 0xDC, 0x29, 0xB0, 0x0E, 0xE6, 0x98, 0xB5, 0x01, 0x11, 0x5C, 0x69, 0xCE, 0x4A, 0xAF, 0xB1, 0x5B, 0x35, 0x03, 0x87, 0x9A, 0x33, 0x61, 0xA2, 0x05, 0x55, 0xD6, 0x24, 0x71},
   {0x4E, 0x08, 0x9C, 0xD3, 0x2F, 0xD9, 0x2E, 0x67, 0x48, 0x95, 0x01, 0xFB, 0x34, 0xE2, 0x36, 0x21, 0x3E, 0x00, 0xCF, 0xD6, 0xB7, 0x68, 0x23, 0xA7, 0x2E, 0xD5, 0xAE, 0x80, 0xFF, 0x37, 0xC9, 0x15},
   {0x3E, 0x10, 0x4F, 0xCA, 0xB3, 0x44, 0x36, 0xBC, 0x93, 0x5B, 0x93, 0xEF, 0x2C, 0x7F, 0xA7, 0x41, 0x9A, 0x89, 0xF3, 0xEF, 0x63, 0x4B, 0xA6, 0x3B, 0x4F, 0xE3, 0xC5, 0x6F, 0x8B, 0xF4, 0x6B, 0x01},
   {0x7B, 0xF4, 0x41, 0x1F, 0xB9, 0xDA, 0x96, 0xF4, 0x45, 0x4A, 0xEB, 0x63, 0x0B, 0x28, 0x7C, 0x06, 0x1C, 0x86, 0xF5, 0x9E, 0x75, 0x90, 0x18, 0x34, 0x01, 0x5B, 0xA6, 0x18, 0xF5, 0xF1, 0xA3, 0x33}},
  {{0x42, 0xF6, 0x3E, 0x16, 0xAD, 0x60, 0x71, 0xE2, 0x67, 0x3E, 0xC4, 0xB0, 0x05, 0xFD, 0x89, 0x6D, 0x31, 0x42, 0x53, 0x7B, 0x4F, 0x9A, 0xCC, 0xE8, 0xA0, 0x0C, 0xA1, 0x9C, 0x96, 0x73, 0xD2, 0x4D},
   {0x50, 0x2F, 0x9D, 0x2B, 0xF2, 0xE6, 0x97, 0x8D, 0x6D, 0xFD, 0xFB, 0x4B, 0xA1, 0x0B, 0x6D, 0x22, 0x5F, 0xCE, 0xC8, 0x7A, 0x15, 0x60, 0xAB, 0xE7, 0x3E, 0x92, 0xE9, 0x1F, 0x3A, 0xF7, 0xE0, 0x6F},
   {0xAB, 0x04, 0x2C, 0xAF, 0x62, 0x1A, 0xD5, 0x6A, 0x74, 0x58, 0x0D, 0xB4, 0x40, 0xD8, 0xD7, 0x06, 0x76, 0x36, 0x2D, 0x87, 0xF4, 0x61, 0x68, 0x60, 0x89, 0x5D, 0xE9, 0x2D, 0xA2, 0x05, 0x69, 0x41},
   {0x82, 0xBA, 0x90, 0xB6, 0x06, 0xC6, 0x60, 0x98, 0x7A, 0x8D, 0xF4, 0xD9, 0x07, 0x6E, 0xF2, 0xF5, 0xD7, 0x38, 0x0E, 0x50, 0x5C, 0x5C, 0x4F, 0x42, 0x3E, 0x37, 0x07, 0x88, 0x35, 0x05, 0xF8, 0x20}},
};

field_element prime = {{0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F}};
field_element xr, yr, tr, zr;

volatile unsigned long overflow_count = 0;  // Global overflow counter
ISR(TIMER1_OVF_vect) {overflow_count++;}  // Increment overflow counter every time Timer 1 overflows}

int main()
{
  Serial.begin(500000);
  Serial.println();

  // Example scalar (saved in 32 hex bytes array using little endian form) = 3618502788666131106986593281521497120428558180689953803000975469142727125494 = 0x80000000000000000000000000000000a6f7cfbf0a86b3b727e1f776e7ae9f6
  unsigned char scalar[32] = {0xF6,	0xE9,	0x7A,	0x6E,	0x77,	0x1F,	0x7E,	0x72,	0x3B,	0x6B,	0xA8,	0xF0,	0xFB,	0x7C,	0x6F,	0x0A,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x08};

  field_element temp_scalar, Gx, Gy, Gt, Gz;

  cli();
  TCCR1A = 0;
  TCCR1B = bit(CS10);
  TCNT1 = 0;
  overflow_count = 0;

  TIMSK1 |= (1 << TOIE1);
  sei();

  scalar[0] &= 248;
  scalar[31] &= 127;
  scalar[31] |= 64;

  for (int i = 0; i < 32; i++){temp_scalar.Ed[i] = scalar[i];}
  for (int i = 0; i < 7; i++){Ed25519_scalar_sub_order(&temp_scalar);}
  for (int i = 0; i < 32; i++){scalar[i] = temp_scalar.Ed[i];}

  windowed_scalar_mul(&Gx, &Gy, &Gt, &Gz, scalar, PRECOMPUTED_POINTS);

  cli();
  unsigned long final_cycles = TCNT1;
  sei();

  unsigned long total_cycles = (overflow_count * 65536UL) + final_cycles;
  Serial.print("Number of cycles = ");
  Serial.println(total_cycles);

  Serial.print("Time requirement = ");
  Serial.print((float)total_cycles / 16);
  Serial.println(" Microseconds");

  Serial.print("Time requirement = ");
  Serial.print((float)total_cycles / 16000);
  Serial.println(" Millisecond");

  Serial.print("Time requirement = ");
  Serial.print((float)total_cycles / 16000000);
  Serial.println(" second");
  
  Serial.print("Affine x-coordinate = ");
  for (int i = 0; i < 32; i++)
  {
    Serial.print(Gx.Ed[i], HEX);
  }
  Serial.println();
}

void windowed_scalar_mul(field_element *xr, field_element *yr, field_element *tr, field_element *zr, unsigned char scalar[32], const field_element precomputed[17][4])
{
  field_element rt;
  unsigned char borrow;

  cli();
  borrow = scalar_sub_halforder(rt.Ed, scalar);
  sei();

  if (borrow == 0)
  {
    for (int i = 0; i < 32; i++){scalar[i] = rt.Ed[i];}

    windowed_scalar_mul(xr, yr, tr, zr, scalar, PRECOMPUTED_POINTS);

    cli();
    Ed255_subp_bigint(xr->Ed, xr->Ed);
    Ed255_subp_bigint(yr->Ed, yr->Ed);
    sei();
    return;
  }
  unsigned char initial_point_window = scalar[31] & 0x0F;
  memcpy_P(xr->Ed, precomputed[initial_point_window][0].Ed, 32);
  memcpy_P(yr->Ed, precomputed[initial_point_window][1].Ed, 32);
  memcpy_P(tr->Ed, precomputed[initial_point_window][2].Ed, 32);
  memcpy_P(zr->Ed, precomputed[initial_point_window][3].Ed, 32);

  for (int i = 30; i >= 0; i--)
  {
    for (int j = 3; j >= 0; j--){double_point(xr, yr, tr, zr);}

    field_element temp_x, temp_y,temp_t, temp_z;
    unsigned char upper_window_value = (scalar[i] >> 4);
    memcpy_P(temp_x.Ed, precomputed[upper_window_value][0].Ed, 32);
    memcpy_P(temp_y.Ed, precomputed[upper_window_value][1].Ed, 32);
    memcpy_P(temp_t.Ed, precomputed[upper_window_value][2].Ed, 32);
    memcpy_P(temp_z.Ed, precomputed[upper_window_value][3].Ed, 32);
    add_points(xr, yr, tr, zr, &temp_x, &temp_y, &temp_t, &temp_z);
    
    for (int j = 3; j >= 0; j--){double_point(xr, yr, tr, zr);}

    unsigned char lower_window_value = (scalar[i] & 0x0F);
    memcpy_P(temp_x.Ed, precomputed[lower_window_value][0].Ed, 32);
    memcpy_P(temp_y.Ed, precomputed[lower_window_value][1].Ed, 32);
    memcpy_P(temp_t.Ed, precomputed[lower_window_value][2].Ed, 32);
    memcpy_P(temp_z.Ed, precomputed[lower_window_value][3].Ed, 32);
    add_points(xr, yr, tr, zr, &temp_x, &temp_y, &temp_t, &temp_z);
  }

  M_Inverse_Z(zr, zr);
  mul(xr, xr, zr);
  in_range(xr);
}

void double_point(field_element *xr, field_element *yr, field_element *tr, field_element *zr)
{
  field_element A, B, C, D, E, F, G, H;
  square(&A, xr);
  square(&B, yr);
  square(&C, zr);
  add(&C, &C, &C);
  sub(&D, &prime, &A);
  add(&E, xr, yr);
  square(&E, &E);
  sub(&E, &E, &A);
  sub(&E, &E, &B);
  add(&G, &D, &B);
  sub(&F, &G, &C);
  sub(&H, &D, &B);
  mul(xr, &E, &F);
  mul(yr, &G, &H);
  mul(tr, &E, &H);
  mul(zr, &F, &G);
}

void add_points(field_element *xr, field_element *yr, field_element *tr, field_element *zr, const field_element *px, const field_element *py, const field_element *tz, const field_element *pz)
{
    field_element A, B, C, D, E, F, G, H;
    sub(&A, yr, xr);
    add(&B, py, px);
    mul(&A, &A, &B);
    add(&B, yr, xr);
    sub(&C, py, px);
    mul(&B, &B, &C);
    mul(&C, zr, tz);
    add(&C, &C, &C);
    mul(&D, tr, pz);
    add(&D, &D, &D);
    add(&E, &D, &C);
    sub(&F, &B, &A);
    add(&G, &B, &A);
    sub(&H, &D, &C);
    mul(xr, &E, &F);
    mul(yr, &G, &H);
    mul(tr, &E, &H);
    mul(zr, &F, &G);
}

void M_Inverse_Z(field_element *r, const field_element *x)
{
  field_element z2, z11, z2_10_0, z2_50_0, z2_100_0, t0, t1;
  unsigned char i;

  square(&z2,x);
  square(&t1,&z2);
  square(&t0,&t1);
  mul(&z2_10_0,&t0,x);
  mul(&z11,&z2_10_0,&z2);
  square(&t0,&z11);
  mul(&z2_10_0,&t0,&z2_10_0);
  square(&t0,&z2_10_0);
  square(&t1,&t0);
  square(&t0,&t1);
  square(&t1,&t0);
  square(&t0,&t1);
  mul(&z2_10_0,&t0,&z2_10_0);
  square(&t0,&z2_10_0);
  square(&t1,&t0);
  for (i = 2;i < 10;i += 2){ square(&t0,&t1); square(&t1,&t0); }
  mul(&z2_50_0,&t1,&z2_10_0);
  square(&t0,&z2_50_0);
  square(&t1,&t0);
  for (i = 2;i < 20;i += 2) { square(&t0,&t1); square(&t1,&t0); }
  mul(&t0,&t1,&z2_50_0);
  square(&t1,&t0);
  square(&t0,&t1);
  for (i = 2;i < 10;i += 2) { square(&t1,&t0); square(&t0,&t1); }
  mul(&z2_50_0,&t0,&z2_10_0);
  square(&t0,&z2_50_0);
  square(&t1,&t0);
  for (i = 2;i < 50;i += 2) { square(&t0,&t1); square(&t1,&t0); }
  mul(&z2_100_0,&t1,&z2_50_0);
  square(&t1,&z2_100_0);
  square(&t0,&t1);
  for (i = 2;i < 100;i += 2) { square(&t1,&t0); square(&t0,&t1); }
  mul(&t1,&t0,&z2_100_0);
  square(&t0,&t1);
  square(&t1,&t0);
  for (i = 2;i < 50;i += 2) { square(&t0,&t1); square(&t1,&t0); }
  mul(&t0,&t1,&z2_50_0);
  square(&t1,&t0);
  square(&t0,&t1);
  square(&t1,&t0);
  square(&t0,&t1);
  square(&t1,&t0);
  mul(r,&t1,&z11);
}

void in_range(field_element *r)
{
  unsigned char c;
  field_element rt;
  c = Ed_num_sub_prime(rt.Ed, r->Ed);
  flip_if(r,&rt,1-c);
}

void flip_if(field_element *r, const field_element *x, unsigned char b)
{
  unsigned char i;
  unsigned long mask = b;
  mask = -mask;
  for(i=0;i<32;i++)
  {
    r->Ed[i] ^= mask & (x->Ed[i] ^ r->Ed[i]);
  }
}

void mul(field_element *r, const field_element *x, const field_element *y)
{
  unsigned char t[64];
  cli();
  Ed_mul(t,x->Ed,y->Ed);
  sei();
  modulo(r,t);
}

void square(field_element *r, const field_element *x)
{
  unsigned char t[64];
  cli();
  Ed25519_square(t,x->Ed);
  sei();
  modulo(r,t);
}

void Ed25519_scalar_sub_order(field_element *r)
{
  unsigned char bwr;
  field_element tempResult;
  cli();
  bwr = scalar_sub_order(tempResult.Ed, r->Ed);
  sei();
  flip_if(r,&tempResult,1-bwr);
}
