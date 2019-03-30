/*****************************************************************************
 *
 *  $Id: CommandSlaves.h,v bc2d4bf9cbe5 2012/09/06 18:22:24 fp $
 *
 *  Copyright (C) 2006-2009  Florian Pose, Ingenieurgemeinschaft IgH
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
 ****************************************************************************/

#ifndef __COMMANDSLAVES_H__
#define __COMMANDSLAVES_H__

#include "Command.h"

/****************************************************************************/

class CommandSlaves:
    public Command
{
    public:
        CommandSlaves();

        string helpString(const string &) const;
        void execute(const StringVector &);

    protected:
        struct Info {
            string pos;
            string alias;
            string relPos;
            string state;
            string flag;
            string name;
            unsigned int device;
        };

        void listSlaves(MasterDevice &, const SlaveList &, bool);
        void showSlaves(MasterDevice &, const SlaveList &);

        static bool slaveInList( const ec_ioctl_slave_t &, const SlaveList &);
};

/****************************************************************************/

#endif
