#ifndef __NUM_H__
#define __NUM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_40_96_MEMBERS               (10)
#define NUM_40_96_MEMBER_ITEMS          (480)
#define NUM_24_48_MEMBERS               (10)
#define NUM_24_48_MEMBER_ITEMS          (144)
#define NUM_12_24_MEMBERS               (95)
#define NUM_12_24_MEMBER_ITEMS          (48)

extern const uint8_t asc2_4096[NUM_40_96_MEMBERS][NUM_40_96_MEMBER_ITEMS];
extern const uint8_t asc2_2448[NUM_24_48_MEMBERS][NUM_24_48_MEMBER_ITEMS];
extern const uint8_t asc2_1224[NUM_12_24_MEMBERS][NUM_12_24_MEMBER_ITEMS];

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* num.h */
