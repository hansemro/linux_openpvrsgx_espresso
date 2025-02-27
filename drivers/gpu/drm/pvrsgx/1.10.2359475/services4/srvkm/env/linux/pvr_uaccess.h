/*************************************************************************/ /*!
@Title          Utility functions for user space access
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/
#ifndef __PVR_UACCESS_H__
#define __PVR_UACCESS_H__

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,38))
#ifndef AUTOCONF_INCLUDED
#include <linux/config.h>
#endif
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif

static inline unsigned long pvr_copy_to_user(void __user *pvTo, const void *pvFrom, unsigned long ulBytes)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33))
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0))
    if (access_ok(pvTo, ulBytes))
#else
    if (access_ok(VERIFY_WRITE, pvTo, ulBytes))
#endif
    {
	return __copy_to_user(pvTo, pvFrom, ulBytes);
    }
    return ulBytes;
#else
    return copy_to_user(pvTo, pvFrom, ulBytes);
#endif
}

static inline unsigned long pvr_copy_from_user(void *pvTo, const void __user *pvFrom, unsigned long ulBytes)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33))
    /*
     * The compile time correctness checking introduced for copy_from_user in
     * Linux 2.6.33 isn't fully comaptible with our usage of the function.
     */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0))
   if (access_ok(pvFrom, ulBytes))
#else
   if (access_ok(VERIFY_READ, pvFrom, ulBytes))
#endif
    {
	return __copy_from_user(pvTo, pvFrom, ulBytes);
    }
    return ulBytes;
#else
    return copy_from_user(pvTo, pvFrom, ulBytes);
#endif
}

#define	pvr_put_user	put_user
#define	pvr_get_user	get_user

#endif /* __PVR_UACCESS_H__ */

