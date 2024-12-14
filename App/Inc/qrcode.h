#ifndef __QRCODE_H__
#define __QRCODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* 公众号（乐鑫信息科技）二维码信息 */
#define QR_CODE_INFO            ("http://weixin.qq.com/r/mp/S0O-u0TEvIzAKQDYbxbP")

/* 二维码默认版本，版本越高，可携带的信息就越多 */
#define QE_CODE_VERSION_DEF     (3)

QRcode *qrcode_init(void);
void qrcode_deinit(QRcode *p_qrcode);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* qr_code.h */

