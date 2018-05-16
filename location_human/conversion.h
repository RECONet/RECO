#ifndef CONVERSION_H
#define CONVERSION_H

#define PLMN_T_TO_TBCD(pLMN, tBCDsTRING, mNClENGTH)                 \
do {                                                                \
    tBCDsTRING[0] = (pLMN.mcc_digit2 << 4) | pLMN.mcc_digit1;         \
    /* ambiguous (think about len 2) */                             \
    if (mNClENGTH == 2) {                                      \
        tBCDsTRING[1] = (0x0F << 4) | pLMN.mcc_digit3;               \
        tBCDsTRING[2] = (pLMN.mnc_digit2 << 4) | pLMN.mnc_digit1;     \
    } else {                                                        \
        tBCDsTRING[1] = (pLMN.mnc_digit3 << 4) | pLMN.mcc_digit3;     \
        tBCDsTRING[2] = (pLMN.mnc_digit2 << 4) | pLMN.mnc_digit1;     \
    }                                                               \
} while(0)

#endif
