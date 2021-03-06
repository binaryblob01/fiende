/* $Id: prockernel.c,v 1.6 1999/02/23 11:45:29 martin Exp $ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/prockernel.h>

const char *glibtop_names_proc_kernel [GLIBTOP_MAX_PROC_KERNEL] =
{
	"k_flags", "min_flt", "maj_flt", "cmin_flt", "cmaj_flt",
	"kstk_esp", "kstk_eip", "nwchan", "wchan"
};

const unsigned glibtop_types_proc_kernel [GLIBTOP_MAX_PROC_KERNEL] =
{
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_STRING
};

const char *glibtop_labels_proc_kernel [GLIBTOP_MAX_PROC_KERNEL] =
{
	N_("K_Flags"), N_("Min_Flt"), N_("Maj_Flt"), N_("CMin_Flt"),
	N_("CMaj_Flt"), N_("KStk_ESP"), N_("KStk_EIP"), N_("NWChan"),
	N_("WChan")
};

const char *glibtop_descriptions_proc_kernel [GLIBTOP_MAX_PROC_KERNEL] =
{
	/* K_Flags */
	N_("Kernel flags of the process.\n\n"
	   "On Linux, currently every flag has the math bit set, because "
	   "crt0.s checks for math emulation, so this is not included in "
	   "the output.\n\n"
	   "This is probably a bug, as not every process is a compiled C "
	   "program.\n\n"
	   "The math bit should be a decimal 4, and the traced bit is "
	   "decimal 10."),
	/* Min_Flt */
	N_("The number of minor faults the process has made, those which "
	   "have not required loading a memory page from disk."),
	/* Maj_Flt */
	N_("The number of major faults the process has made, those which "
	   "have required loading a memory page from disk."),
	/* CMin_Flt */
	N_("The number of minor faults that the process and its children "
	   "have made."),
	/* CMaj_Flt */
	N_("The number of major faults that the process and its children "
	   "have made."),
	/* KStk_ESP */
	N_("The current value of esp (32-bit stack pointer), as found in "
	   "the kernel stack page for the process."),
	/* KStk_EIP */
	N_("The current EIP (32-bit instruction pointer)."),
	/* NWChan */
	N_("This is the \"channel\" in which the process is waiting.  This "
	   "is the address of a system call, and can be looked up in a "
	   "namelist if you need a textual name.  (If you have an up-to-date "
	   "/etc/psdatabase, then try ps -l to see the WCHAN field in action)"),
	/* WChan */
	N_("This is the textual name of the `nwchan' field.")
};
