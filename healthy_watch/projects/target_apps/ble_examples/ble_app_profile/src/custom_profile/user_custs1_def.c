/**
 ****************************************************************************************
 *
 * @file user_custs1_def.c
 *
 * @brief Custom1 Server (CUSTS1) profile database definitions.
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

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1 Server (CUSTS1) profile database definitions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "prf_types.h"
#include "attm_db_128.h"
#include "user_custs1_def.h"

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Custom Server Attributes Values Definition

static const att_svc_desc_t custs1_svc                        = DEF_CUST1_SVC_UUID;

static const struct att_char_desc custs1_indicateable_char     = ATT_CHAR(ATT_CHAR_PROP_RD |  ATT_CHAR_PROP_WR_NO_RESP | ATT_CHAR_PROP_NTF,
                                                                     0,
                                                                     DEF_CUST1_INDICATEABLE_UUID);

static const struct att_char_desc custs1_long_value_char       = ATT_CHAR(ATT_CHAR_PROP_RD |  ATT_CHAR_PROP_WR_NO_RESP | ATT_CHAR_PROP_NTF,
                                                                     0,
                                                                     DEF_CUST1_LONG_VALUE_UUID);

static const uint8_t custs1_indicateable_desc[] = CUST1_INDICATEABLE_USER_DESC;

static const uint8_t custs1_long_value_desc[] = CUST1_LONG_VALUE_CHAR_USER_DESC;

/*
 * CUSTS1 ATTRIBUTES
 ****************************************************************************************
 */

static const uint16_t att_decl_svc       = ATT_DECL_PRIMARY_SERVICE;
static const uint16_t att_decl_char      = ATT_DECL_CHARACTERISTIC;
static const uint16_t att_decl_cfg       = ATT_DESC_CLIENT_CHAR_CFG;
static const uint16_t att_decl_user_desc = ATT_DESC_CHAR_USER_DESCRIPTION;

/// Full CUSTOM1 Database Description - Used to add attributes into the database
const struct attm_desc custs1_att_db[CUST1_IDX_NB] =
{
    // CUSTOM1 Service Declaration
    [CUST1_IDX_SVC]                     = {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE),
                                            sizeof(custs1_svc), sizeof(custs1_svc), (uint8_t*)&custs1_svc},

    // Indicateable Characteristic Declaration
    [CUST1_IDX_INDICATEABLE_CHAR]       = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE),
                                            sizeof(custs1_indicateable_char), sizeof(custs1_indicateable_char), (uint8_t*)&custs1_indicateable_char},
    // Indicateable Characteristic Value
    [CUST1_IDX_INDICATEABLE_VAL]        = {DEF_CUST1_INDICATEABLE_UUID, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE),
                                            DEF_CUST1_INDICATEABLE_CHAR_LEN, 0, NULL},
    // Indicateable Client Characteristic Configuration Descriptor
    [CUST1_IDX_INDICATEABLE_IND_CFG]    = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                            sizeof(uint16_t), 0, NULL},
    // Indicateable Characteristic User Description
    [CUST1_IDX_INDICATEABLE_USER_DESC]  = {ATT_DESC_CHAR_USER_DESCRIPTION, PERM(RD, ENABLE),
                                            sizeof(CUST1_INDICATEABLE_USER_DESC) - 1, sizeof(CUST1_INDICATEABLE_USER_DESC) - 1, (uint8_t*)&custs1_indicateable_desc},

    // Long Value Characteristic Declaration
    [CUST1_IDX_LONG_VALUE_CHAR]         = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE),
                                            sizeof(custs1_long_value_char), sizeof(custs1_long_value_char), (uint8_t*)&custs1_long_value_char},
    // Long Value Characteristic Value
    [CUST1_IDX_LONG_VALUE_VAL]          = {DEF_CUST1_LONG_VALUE_UUID, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE),
                                            DEF_CUST1_LONG_VALUE_CHAR_LEN, 0, NULL},
    // Long Value Client Characteristic Configuration Descriptor
    [CUST1_IDX_LONG_VALUE_NTF_CFG]      = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                            sizeof(uint16_t), 0, NULL},
    // Long Value Characteristic User Description
    [CUST1_IDX_LONG_VALUE_USER_DESC]    = {ATT_DESC_CHAR_USER_DESCRIPTION, PERM(RD, ENABLE),
                                            sizeof(CUST1_IDX_LONG_VALUE_USER_DESC) - 1, sizeof(CUST1_IDX_LONG_VALUE_USER_DESC) - 1, (uint8_t*)&custs1_long_value_desc},
};


/// @} USER_CONFIG
