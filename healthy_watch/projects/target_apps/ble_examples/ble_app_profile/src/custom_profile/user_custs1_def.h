/**
 ****************************************************************************************
 *
 * @file user_custs1_def.h
 *
 * @brief Custom1 Server (CUSTS1) profile database declarations.
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_DEF_H_
#define _USER_CUSTS1_DEF_H_

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1 Server (CUSTS1) profile database declarations.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm_db_128.h"

/*
 * DEFINES
 ****************************************************************************************
 */


#define DEF_CUST1_SVC_UUID          0XFFF0 //  {0x2F, 0x2A, 0x93, 0xA6, 0xBD, 0xD8, 0x41, 0x52, 0xAC, 0x0B, 0x10, 0x99, 0x2E, 0xC6, 0xFE, 0xED}

#define DEF_CUST1_INDICATEABLE_UUID  0XFFF1 // {0x28, 0xD5, 0xE1, 0xC1, 0xE1, 0xC5, 0x47, 0x29, 0xB5, 0x57, 0x65, 0xC3, 0xBA, 0x47, 0x15, 0x9E}
#define DEF_CUST1_LONG_VALUE_UUID    0XFFF4 // {0x8C, 0x09, 0xE0, 0xD1, 0x81, 0x54, 0x42, 0x40, 0x8E, 0x4F, 0xD2, 0xB3, 0x77, 0xE3, 0x2A, 0x77}

#define DEF_CUST1_INDICATEABLE_CHAR_LEN   20
#define DEF_CUST1_LONG_VALUE_CHAR_LEN     20

#define CUST1_INDICATEABLE_USER_DESC      "Indicateable"
#define CUST1_LONG_VALUE_CHAR_USER_DESC   "Long Value"

/// Custom1 Service Data Base Characteristic enum
enum
{
    CUST1_IDX_SVC = 0,
       
    CUST1_IDX_INDICATEABLE_CHAR,
    CUST1_IDX_INDICATEABLE_VAL,
    CUST1_IDX_INDICATEABLE_IND_CFG,
    CUST1_IDX_INDICATEABLE_USER_DESC,
    
    CUST1_IDX_LONG_VALUE_CHAR,
    CUST1_IDX_LONG_VALUE_VAL,
    CUST1_IDX_LONG_VALUE_NTF_CFG,
    CUST1_IDX_LONG_VALUE_USER_DESC,
    
    CUST1_IDX_NB
};
/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct attm_desc custs1_att_db[CUST1_IDX_NB];

/// @} USER_CONFIG

#endif // _USER_CUSTS1_DEF_H_
