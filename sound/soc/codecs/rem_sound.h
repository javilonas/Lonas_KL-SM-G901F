/*
 * Author Rem Sound: javilonas, 23.05.2015
 * 
 * Version 1.2 to WCD9330 TomTom codec driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kallsyms.h>

#include <sound/soc.h>
#include <sound/core.h>

#include <linux/mfd/wcd9xxx/core.h>
#include <linux/mfd/wcd9xxx/wcd9xxx_registers.h>
#include <linux/mfd/wcd9xxx/wcd9330_registers.h>
#include <linux/mfd/wcd9xxx/pdata.h>

#include "wcd9330.h"
#include "wcd9xxx-resmgr.h"
#include "wcd9xxx-common.h"

/*****************************************/
// Function declarations
/*****************************************/

// wcd9330 functions
extern unsigned int tomtom_read(struct snd_soc_codec *codec,
				unsigned int reg);
extern int tomtom_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int value);
extern int tomtom_write_no_hook(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int value);

// rem sound functions
void rem_sound_hook_tomtom_codec_probe(struct snd_soc_codec *codec_pointer);
unsigned int rem_sound_hook_tomtom_write(unsigned int reg, unsigned int value);


/*****************************************/
// Definitions
/*****************************************/

// rem sound general
#define REM_SOUND_DEFAULT		1
#define REM_SOUND_VERSION		"1.2"

// Debug mode
#define DEBUG_DEFAULT 			0

// headphone levels
#define HEADPHONE_DEFAULT		0
#define HEADPHONE_REG_OFFSET		0
#define HEADPHONE_MIN 			-30
#define HEADPHONE_MAX 			30

// speaker levels
#define SPEAKER_DEFAULT			0
#define SPEAKER_REG_OFFSET		-4
#define SPEAKER_MIN 			-30
#define SPEAKER_MAX 			30

// Microphone control
#define MICLEVEL_DEFAULT		0
#define MICLEVEL_REG_OFFSET		0
#define MICLEVEL_MIN			-30
#define MICLEVEL_MAX			30

