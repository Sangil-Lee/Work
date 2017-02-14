/******************************************************************************
 *
 *  $Id: reg_request.h,v 2c3ccdde3919 2012/11/14 21:12:57 fp $
 *
 *  Copyright (C) 2012  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT master userspace library.
 *
 *  The IgH EtherCAT master userspace library is free software; you can
 *  redistribute it and/or modify it under the terms of the GNU Lesser General
 *  Public License as published by the Free Software Foundation; version 2.1
 *  of the License.
 *
 *  The IgH EtherCAT master userspace library is distributed in the hope that
 *  it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the IgH EtherCAT master userspace library. If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 *****************************************************************************/

#include "include/ecrt.h"

/*****************************************************************************/

struct ec_reg_request {
    ec_reg_request_t *next; /**< List header. */
    ec_slave_config_t *config; /**< Parent slave configuration. */
    unsigned int index; /**< Request index (identifier). */
    uint8_t *data; /**< Data memory. */
    size_t mem_size; /**< Size of \a data. */
};

/*****************************************************************************/

void ec_reg_request_clear(ec_reg_request_t *);

/*****************************************************************************/
