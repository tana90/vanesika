/*
 * volume.h - Exports for NTFS volume handling. Part of the Linux-NTFS project.
 *
 * Copyright (c) 2000-2004 Anton Altaparmakov
 * Copyright (c) 2005-2006 Yura Pakhuchiy
 * Copyright (c) 2004-2005 Richard Russon
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Modified 01/2007 by Andy McLaughlin for Visopsys port.
 */

#ifndef _NTFS_VOLUME_H
#define _NTFS_VOLUME_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif
#ifdef HAVE_MNTENT_H
#include <mntent.h>
#endif

#include <sys/progress.h>

/*
 * Under Cygwin, DJGPP and FreeBSD we do not have MS_RDONLY and MS_NOATIME,
 * so we define them ourselves.
 */
#ifndef MS_RDONLY
#define MS_RDONLY 1
#endif
/*
 * Solaris defines MS_RDONLY but not MS_NOATIME thus we need to carefully
 * define MS_NOATIME.
 */
#ifndef MS_NOATIME
#if (MS_RDONLY != 1)
#	define MS_NOATIME 1
#else
#	define MS_NOATIME 2
#endif
#endif

/* Forward declaration */
typedef struct _ntfs_volume ntfs_volume;

#include "types.h"
#include "support.h"
#include "device.h"
#include "inode.h"
#include "attrib.h"

/**
 * enum ntfs_mount_flags -
 *
 * Flags returned by the ntfs_check_if_mounted() function.
 */
typedef enum {
	NTFS_MF_MOUNTED		= 1,	/* Device is mounted. */
	NTFS_MF_ISROOT		= 2,	/* Device is mounted as system root. */
	NTFS_MF_READONLY	= 4,	/* Device is mounted read-only. */
} ntfs_mount_flags;

extern int ntfs_check_if_mounted(const char *filenm, unsigned long *mnt_flags);

/**
 * enum ntfs_volume_state_bits -
 *
 * Defined bits for the state field in the ntfs_volume structure.
 */
typedef enum {
	NV_ReadOnly,		/* 1: Volume is read-only. */
	NV_CaseSensitive,	/* 1: Volume is mounted case-sensitive. */
	NV_LogFileEmpty,	/* 1: $logFile journal is empty. */
	NV_NoATime,		/* 1: Do not update access time. */
} ntfs_volume_state_bits;

#define  test_nvol_flag(nv, flag)	 test_bit(NV_##flag, (nv)->state)
#define   set_nvol_flag(nv, flag)	  set_bit(NV_##flag, (nv)->state)
#define clear_nvol_flag(nv, flag)	clear_bit(NV_##flag, (nv)->state)

#define NVolReadOnly(nv)		 test_nvol_flag(nv, ReadOnly)
#define NVolSetReadOnly(nv)		  set_nvol_flag(nv, ReadOnly)
#define NVolClearReadOnly(nv)		clear_nvol_flag(nv, ReadOnly)

#define NVolCaseSensitive(nv)		 test_nvol_flag(nv, CaseSensitive)
#define NVolSetCaseSensitive(nv)	  set_nvol_flag(nv, CaseSensitive)
#define NVolClearCaseSensitive(nv)	clear_nvol_flag(nv, CaseSensitive)

#define NVolLogFileEmpty(nv)		 test_nvol_flag(nv, LogFileEmpty)
#define NVolSetLogFileEmpty(nv)		  set_nvol_flag(nv, LogFileEmpty)
#define NVolClearLogFileEmpty(nv)	clear_nvol_flag(nv, LogFileEmpty)

#define NVolNoATime(nv)			 test_nvol_flag(nv, NoATime)
#define NVolSetNoATime(nv)		  set_nvol_flag(nv, NoATime)
#define NVolClearNoATime(nv)		clear_nvol_flag(nv, NoATime)

/*
 * NTFS version 1.1 and 1.2 are used by Windows NT4.
 * NTFS version 2.x is used by Windows 2000 Beta
 * NTFS version 3.0 is used by Windows 2000.
 * NTFS version 3.1 is used by Windows XP, 2003 and Vista.
 */

#define NTFS_V1_1(major, minor) ((major) == 1 && (minor) == 1)
#define NTFS_V1_2(major, minor) ((major) == 1 && (minor) == 2)
#define NTFS_V2_X(major, minor) ((major) == 2)
#define NTFS_V3_0(major, minor) ((major) == 3 && (minor) == 0)
#define NTFS_V3_1(major, minor) ((major) == 3 && (minor) == 1)

#define NTFS_BUF_SIZE 8192

/**
 * struct _ntfs_volume - structure describing an open volume in memory.
 */
struct _ntfs_volume {
	union {
		struct ntfs_device *dev;	/* NTFS device associated with
						   the volume. */
		void *sb;	/* For kernel porting compatibility. */
	};
	char *vol_name;		/* Name of the volume. */
	unsigned long state;	/* NTFS specific flags describing this volume.
				   See ntfs_volume_state_bits above. */

	ntfs_inode *vol_ni;	/* ntfs_inode structure for FILE_Volume. */
	u8 major_ver;		/* Ntfs major version of volume. */
	u8 minor_ver;		/* Ntfs minor version of volume. */
	u16 flags;		/* Bit array of VOLUME_* flags. */

	u16 sector_size;	/* Byte size of a sector. */
	u8 sector_size_bits;	/* Log(2) of the byte size of a sector. */
	u32 cluster_size;	/* Byte size of a cluster. */
	u32 mft_record_size;	/* Byte size of a mft record. */
	u32 indx_record_size;	/* Byte size of a INDX record. */
	u8 cluster_size_bits;	/* Log(2) of the byte size of a cluster. */
	u8 mft_record_size_bits;/* Log(2) of the byte size of a mft record. */
	u8 indx_record_size_bits;/* Log(2) of the byte size of a INDX record. */

	/* Variables used by the cluster and mft allocators. */
	u8 mft_zone_multiplier;	/* Initial mft zone multiplier. */
	s64 mft_data_pos;	/* Mft record number at which to allocate the
				   next mft record. */
	LCN mft_zone_start;	/* First cluster of the mft zone. */
	LCN mft_zone_end;	/* First cluster beyond the mft zone. */
	LCN mft_zone_pos;	/* Current position in the mft zone. */
	LCN data1_zone_pos;	/* Current position in the first data zone. */
	LCN data2_zone_pos;	/* Current position in the second data zone. */

	s64 nr_clusters;	/* Volume size in clusters, hence also the
				   number of bits in lcn_bitmap. */
	ntfs_inode *lcnbmp_ni;	/* ntfs_inode structure for FILE_Bitmap. */
	ntfs_attr *lcnbmp_na;	/* ntfs_attr structure for the data attribute
				   of FILE_Bitmap. Each bit represents a
				   cluster on the volume, bit 0 representing
				   lcn 0 and so on. A set bit means that the
				   cluster and vice versa. */

	LCN mft_lcn;		/* Logical cluster number of the data attribute
				   for FILE_MFT. */
	ntfs_inode *mft_ni;	/* ntfs_inode structure for FILE_MFT. */
	ntfs_attr *mft_na;	/* ntfs_attr structure for the data attribute
				   of FILE_MFT. */
	ntfs_attr *mftbmp_na;	/* ntfs_attr structure for the bitmap attribute
				   of FILE_MFT. Each bit represents an mft
				   record in the $DATA attribute, bit 0
				   representing mft record 0 and so on. A set
				   bit means that the mft record is in use and
				   vice versa. */

	int mftmirr_size;	/* Size of the FILE_MFTMirr in mft records. */
	LCN mftmirr_lcn;	/* Logical cluster number of the data attribute
				   for FILE_MFTMirr. */
	ntfs_inode *mftmirr_ni;	/* ntfs_inode structure for FILE_MFTMirr. */
	ntfs_attr *mftmirr_na;	/* ntfs_attr structure for the data attribute
				   of FILE_MFTMirr. */

	ntfschar *upcase;	/* Upper case equivalents of all 65536 2-byte
				   Unicode characters. Obtained from
				   FILE_UpCase. */
	u32 upcase_len;		/* Length in Unicode characters of the upcase
				   table. */

	ATTR_DEF *attrdef;	/* Attribute definitions. Obtained from
				   FILE_AttrDef. */
	s32 attrdef_len;	/* Size of the attribute definition table in
				   bytes. */

	/* Temp: for directory handling */
	void *private_data;	/* ntfs_dir for . */
	void *private_bmp1;	/* ntfs_bmp for $MFT/$BITMAP */
	void *private_bmp2;	/* ntfs_bmp for $Bitmap */
};

extern ntfs_volume *ntfs_volume_alloc(void);

extern ntfs_volume *ntfs_volume_startup(struct ntfs_device *dev,
		unsigned long flags);

extern ntfs_volume *ntfs_device_mount(struct ntfs_device *dev,
		unsigned long flags);
extern int ntfs_device_umount(ntfs_volume *vol, const BOOL force);

extern ntfs_volume *ntfs_mount(const char *name, unsigned long flags);
extern int ntfs_umount(ntfs_volume *vol, const BOOL force);

extern int ntfs_version_is_supported(ntfs_volume *vol);
extern int ntfs_logfile_reset(ntfs_volume *vol, progress *, int);

extern int ntfs_volume_write_flags(ntfs_volume *vol, const u16 flags);
extern void progress_update(progress *, int, u64, u64);

#ifdef NTFS_RICH

int ntfs_volume_commit(ntfs_volume *vol);
int ntfs_volume_rollback(ntfs_volume *vol);
int ntfs_volume_umount2(ntfs_volume *vol, const BOOL force);
ntfs_volume * ntfs_volume_mount2(const char *device, unsigned long flags, BOOL force);
int utils_valid_device(const char *name, int force);
ntfs_volume * utils_mount_volume(const char *device, unsigned long flags, BOOL force);

#endif /* NTFS_RICH */

#endif /* defined _NTFS_VOLUME_H */

