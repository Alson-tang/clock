#include "main.h"

QRcode *qrcode_init(void)
{
    QRcode *p_qrcode = NULL;
    p_qrcode = QRcode_encodeString(QR_CODE_INFO, QE_CODE_VERSION_DEF, QR_ECLEVEL_L, QR_MODE_8, 1);

    return p_qrcode;
}

void qrcode_deinit(QRcode *p_qrcode)
{
    if (p_qrcode != NULL) {
        QRcode_free(p_qrcode);
    }
}
