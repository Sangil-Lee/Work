/******************************************************************************
 *
 *  $Id: soe_request.c,v ec403cf308eb 2013/02/12 14:46:43 fp $
 *
 *  Copyright (C) 2006-2008  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT Master.
 *
 *  The IgH EtherCAT Master is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version 2, as
 *  published by the Free Software Foundation.
 *
 *  The IgH EtherCAT Master is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the IgH EtherCAT Master; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 *****************************************************************************/

/** \file
 * Sercos-over-EtherCAT request functions.
 */

/*****************************************************************************/

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/slab.h>

#include "soe_request.h"

/*****************************************************************************/

/** Default timeout in ms to wait for SoE responses.
 */
#define EC_SOE_REQUEST_RESPONSE_TIMEOUT EC_SOE_REQUEST_RESPONSE_TIMEOUT_OVERRIDE

/*****************************************************************************/

void ec_soe_request_clear_data(ec_soe_request_t *);

/*****************************************************************************/

/** SoE request constructor.
 */
void ec_soe_request_init(
        ec_soe_request_t *req /**< SoE request. */
        )
{
    INIT_LIST_HEAD(&req->list);
    req->drive_no = 0x00;
    req->idn = 0x0000;
    req->al_state = EC_AL_STATE_INIT;
    req->data = NULL;
    req->mem_size = 0;
    req->data_size = 0;
    req->dir = EC_DIR_INVALID;
    req->state = EC_INT_REQUEST_INIT;
    req->jiffies_sent = 0U;
    req->error_code = 0x0000;
}

/*****************************************************************************/

/** SoE request destructor.
 */
void ec_soe_request_clear(
        ec_soe_request_t *req /**< SoE request. */
        )
{
    ec_soe_request_clear_data(req);
}

/*****************************************************************************/

/** Copy another SoE request.
 *
 * \return Zero on success, otherwise a negative error code.
 */
int ec_soe_request_copy(
        ec_soe_request_t *req, /**< SoE request. */
        const ec_soe_request_t *other /**< Other SoE request to copy from. */
        )
{
    req->drive_no = other->drive_no;
    req->idn = other->idn;
    req->al_state = other->al_state;
    return ec_soe_request_copy_data(req, other->data, other->data_size);
}

/*****************************************************************************/

/** Set drive number.
 */
void ec_soe_request_set_drive_no(
        ec_soe_request_t *req, /**< SoE request. */
        uint8_t drive_no /** Drive Number. */
        )
{
    req->drive_no = drive_no;
}

/*****************************************************************************/

/** Set IDN.
 */
void ec_soe_request_set_idn(
        ec_soe_request_t *req, /**< SoE request. */
        uint16_t idn /** IDN. */
        )
{
    req->idn = idn;
}

/*****************************************************************************/

/** Free allocated memory.
 */
void ec_soe_request_clear_data(
        ec_soe_request_t *req /**< SoE request. */
        )
{
    if (req->data) {
        kfree(req->data);
        req->data = NULL;
    }

    req->mem_size = 0;
    req->data_size = 0;
}

/*****************************************************************************/

/** Pre-allocates the data memory.
 *
 * If the \a mem_size is already bigger than \a size, nothing is done.
 *
 * \return 0 on success, otherwise -ENOMEM.
 */
int ec_soe_request_alloc(
        ec_soe_request_t *req, /**< SoE request. */
        size_t size /**< Data size to allocate. */
        )
{
    if (size <= req->mem_size)
        return 0;

    ec_soe_request_clear_data(req);

    if (!(req->data = (uint8_t *) kmalloc(size, GFP_KERNEL))) {
        EC_ERR("Failed to allocate %zu bytes of SoE memory.\n", size);
        return -ENOMEM;
    }

    req->mem_size = size;
    req->data_size = 0;
    return 0;
}

/*****************************************************************************/

/** Copies SoE data from an external source.
 *
 * If the \a mem_size is to small, new memory is allocated.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_soe_request_copy_data(
        ec_soe_request_t *req, /**< SoE request. */
        const uint8_t *source, /**< Source data. */
        size_t size /**< Number of bytes in \a source. */
        )
{
    int ret = ec_soe_request_alloc(req, size);
    if (ret < 0)
        return ret;

    memcpy(req->data, source, size);
    req->data_size = size;
    return 0;
}

/*****************************************************************************/

/** Copies SoE data from an external source.
 *
 * If the \a mem_size is to small, new memory is allocated.
 *
 * \retval  0 Success.
 * \retval <0 Error code.
 */
int ec_soe_request_append_data(
        ec_soe_request_t *req, /**< SoE request. */
        const uint8_t *source, /**< Source data. */
        size_t size /**< Number of bytes in \a source. */
        )
{
    if (req->data_size + size > req->mem_size) {
        size_t new_size = req->mem_size ? req->mem_size * 2 : size;
        uint8_t *new_data = (uint8_t *) kmalloc(new_size, GFP_KERNEL);
        if (!new_data) {
            EC_ERR("Failed to allocate %zu bytes of SoE memory.\n",
                    new_size);
            return -ENOMEM;
        }
        memcpy(new_data, req->data, req->data_size);
        kfree(req->data);
        req->data = new_data;
        req->mem_size = new_size;
    }

    memcpy(req->data + req->data_size, source, size);
    req->data_size += size;
    return 0;
}

/*****************************************************************************/

/** Request a read operation.
 */
void ec_soe_request_read(
        ec_soe_request_t *req /**< SoE request. */
       )
{
    req->dir = EC_DIR_INPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->error_code = 0x0000;
}

/*****************************************************************************/

/** Request a write operation.
 */
void ec_soe_request_write(
        ec_soe_request_t *req /**< SoE request. */
        )
{
    req->dir = EC_DIR_OUTPUT;
    req->state = EC_INT_REQUEST_QUEUED;
    req->error_code = 0x0000;
}

/*****************************************************************************/
