/**
 * @file binary.h
 *
 * @date Jul 21, 2010
 * @author stefan
 * @brief (Summary)
 */

#ifndef BINARY_H_
#define BINARY_H_

#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)

/* *** user macros *** */

/* for up to 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

#endif /* BINARY_H_ */
