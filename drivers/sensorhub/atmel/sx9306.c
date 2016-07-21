/*
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wakelock.h>
#include <linux/regulator/consumer.h>
#include "sx9306_reg.h"
#include "ssp.h"

#define VENDOR_NAME              "SEMTECH"
#define MODEL_NAME               "SX9306"
#define MODULE_NAME              "grip_sensor"
#define CALIBRATION_FILE_PATH    "/efs/grip_cal_data"

#define I2C_M_WR                 0 /* for i2c Write */
#define I2c_M_RD                 1 /* for i2c Read */

#define IDLE                     0
#define ACTIVE                   1

#define CAL_RET_ERROR            -1
#define CAL_RET_NONE             0
#define CAL_RET_EXIST            1
#define CAL_RET_SUCCESS          2

#define INIT_TOUCH_MODE          0
#define NORMAL_TOUCH_MODE        1

#define SX9306_MODE_SLEEP        0
#define SX9306_MODE_NORMAL       1

#define MAIN_SENSOR              0
#define REF_SENSOR               1
#define CSX_STATUS_REG           SX9306_TCHCMPSTAT_TCHSTAT0_FLAG

#define LIMIT_PROXOFFSET                3880 /* 45 pF */
#define LIMIT_PROXUSEFUL                10000
#define STANDARD_CAP_MAIN               450000

#define DEFAULT_INIT_TOUCH_THRESHOLD    2000
#define DEFAULT_NORMAL_TOUCH_THRESHOLD  17

#define SX9306_NOR_GATE          0
#define SX9306_OR_GATE           1

#ifdef CONFIG_SENSORS_SX9306_TEMPERATURE_COMPENSATION
#define TOUCH_CHECK_SLOPE        11
#define ENABLE_CSX               ((1 << MAIN_SENSOR) | (1 << REF_SENSOR))
#define CAL_DATA_NUM             4
#else
#define ENABLE_CSX               (1 << MAIN_SENSOR)
#define CAL_DATA_NUM             3
#endif

#define DEFENCE_CODE_FOR_DEVICE_DAMAGE

/* CS0, CS1, CS2, CS3 */
#define TOTAL_BOTTON_COUNT       1

#define IRQ_PROCESS_CONDITION   (SX9306_IRQSTAT_TOUCH_FLAG	\
				| SX9306_IRQSTAT_RELEASE_FLAG	\
				| SX9306_IRQSTAT_COMPDONE_FLAG)
#ifdef CONFIG_MACH_TBLTE_VZW
#include <asm/system_info.h>
extern unsigned int system_rev;
#endif

struct sx9306_p {
	struct i2c_client *client;
	struct input_dev *input;
	struct device *factory_device;
	struct delayed_work init_work;
	struct delayed_work irq_work;
	struct wake_lock grip_wake_lock;
	struct mutex mode_mutex;
	bool calSuccessed;
	bool flagDataSkip;
	u8 touchTh;
	int initTh;
	int calData[CAL_DATA_NUM];
	int touchMode;
	int gatetype;
	int irq;
	int gpioNirq;
	int state;
	atomic_t enable;
#ifdef CONFIG_SENSORS_GRIP_ADJDET
	int gpio_adjdet;
	int irq_adjdet;
	int grip_state;
	int enable_adjdet;
#endif
#ifdef CONFIG_SENSORS_SX9306_REGULATOR_ONOFF
	struct regulator *vdd;
	struct regulator *ldo;
#endif
#ifdef CONFIG_SENSORS_GRIP_SIMDET
	int sim_type;
#endif
};

static int sx9306_get_nirq_state(struct sx9306_p *data)
{
	if (data->gatetype == SX9306_NOR_GATE)
		return !gpio_get_value_cansleep(data->gpioNirq);

	return gpio_get_value_cansleep(data->gpioNirq);
}

static int sx9306_i2c_write(struct sx9306_p *data, u8 reg_addr, u8 buf)
{
	int ret;
	struct i2c_msg msg;
	unsigned char w_buf[2];

	w_buf[0] = reg_addr;
	w_buf[1] = buf;

	msg.addr = data->client->addr;
	msg.flags = I2C_M_WR;
	msg.len = 2;
	msg.buf = (char *)w_buf;

	ret = i2c_transfer(data->client->adapter, &msg, 1);
	if (ret < 0)
		pr_err("[SX9306]: %s - i2c write error %d\n", __func__, ret);

	return ret;
}

static int sx9306_i2c_read(struct sx9306_p *data, u8 reg_addr, u8 *buf)
{
	int ret;
	struct i2c_msg msg[2];

	msg[0].addr = data->client->addr;
	msg[0].flags = I2C_M_WR;
	msg[0].len = 1;
	msg[0].buf = &reg_addr;

	msg[1].addr = data->client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = buf;

	ret = i2c_transfer(data->client->adapter, msg, 2);
	if (ret < 0)
		pr_err("[SX9306]: %s - i2c read error %d\n", __func__, ret);

	return ret;
}

static u8 sx9306_read_irqstate(struct sx9306_p *data)
{
	u8 val = 0;
	u8 ret;

	if (sx9306_i2c_read(data, SX9306_IRQSTAT_REG, &val) >= 0) {
		ret = val & 0x00FF;
		return ret;
	}

	return 0;
}

static void sx9306_initialize_register(struct sx9306_p *data)
{
	u8 val = 0;
	int idx;

	for (idx = 0; idx < (sizeof(setup_reg) >> 1); idx++) {
		sx9306_i2c_write(data, setup_reg[idx].reg, setup_reg[idx].val);
		pr_info("[SX9306]: %s - Write Reg: 0x%x Value: 0x%x\n",
			__func__, setup_reg[idx].reg, setup_reg[idx].val);

		sx9306_i2c_read(data, setup_reg[idx].reg, &val);
		pr_info("[SX9306]: %s - Read Reg: 0x%x Value: 0x%x\n\n",
			__func__, setup_reg[idx].reg, val);
	}
}

static void sx9306_initialize_chip(struct sx9306_p *data)
{
	int cnt = 0;

	while ((sx9306_get_nirq_state(data) == 0) && (cnt++ < 10)) {
		sx9306_read_irqstate(data);
		msleep(20);
	}

	if (cnt >= 10)
		pr_err("[SX9306]: %s - s/w reset fail(%d)\n", __func__, cnt);

	sx9306_initialize_register(data);
}

static int sx9306_set_offset_calibration(struct sx9306_p *data)
{
	int ret = 0;

	ret = sx9306_i2c_write(data, SX9306_IRQSTAT_REG, 0xFF);

	return ret;
}

static void send_event(struct sx9306_p *data, u8 state)
{
	u8 buf = data->touchTh;

	if (state == ACTIVE) {
		data->state = ACTIVE;
		sx9306_i2c_write(data, SX9306_CPS_CTRL6_REG, buf);
		pr_info("[SX9306]: %s - button touched\n", __func__);
	} else {
		data->touchMode = NORMAL_TOUCH_MODE;
		data->state = IDLE;
		sx9306_i2c_write(data, SX9306_CPS_CTRL6_REG, buf);
		pr_info("[SX9306]: %s - button released\n", __func__);
	}

	if (data->flagDataSkip == true)
		return;

#ifdef CONFIG_SENSORS_GRIP_ADJDET
	if (data->enable_adjdet == 1
	   &&
	   gpio_get_value_cansleep(data->gpio_adjdet) == 1) {
		pr_info("[SX9306]: %s : adj detect cable connected," \
			" skip grip sensor\n", __func__);
		return;
	}

	data->grip_state = state;
#endif

	if (state == ACTIVE)
		input_report_rel(data->input, REL_MISC, 1);
	else
		input_report_rel(data->input, REL_MISC, 2);

	input_sync(data->input);
}

static void sx9306_display_data_reg(struct sx9306_p *data)
{
	u8 val, reg;

	sx9306_i2c_write(data, SX9306_REGSENSORSELECT, MAIN_SENSOR);
	for (reg = SX9306_REGUSEMSB; reg <= SX9306_REGOFFSETLSB; reg++) {
		sx9306_i2c_read(data, reg, &val);
		pr_info("[SX9306]: %s - Register(0x%2x) data(0x%2x)\n",
			__func__, reg, val);
	}
}

static s32 sx9306_get_init_threshold(struct sx9306_p *data)
{
	s32 threshold;

	/* Because the STANDARD_CAP_MAIN was 300,000 in the previous patch,
	 * the exception code is added. It will be removed later */
	if (data->calData[0] == 0) {
		threshold = STANDARD_CAP_MAIN + data->initTh;
	} else {
		threshold = data->initTh + data->calData[0];
#ifdef CONFIG_SENSORS_GRIP_SIMDET
		if (data->sim_type == 1)
			threshold = threshold + 880;
#endif
	}
	return threshold;
}

static int sx9306_get_useful(struct sx9306_p *data, u8 channel)
{
	u8 msByte = 0;
	u8 lsByte = 0;
	s16 fullByte = 0;

	sx9306_i2c_write(data, SX9306_REGSENSORSELECT, channel);
	sx9306_i2c_read(data, SX9306_REGUSEMSB, &msByte);
	sx9306_i2c_read(data, SX9306_REGUSELSB, &lsByte);
	fullByte = (s16)((msByte << 8) | lsByte);
	if (fullByte > 32767)
		fullByte -= 65536;

	return (int)fullByte;
}

static int sx9306_get_offset(struct sx9306_p *data, u8 channel)
{
	u8 msByte = 0;
	u8 lsByte = 0;
	u16 fullByte = 0;

	sx9306_i2c_write(data, SX9306_REGSENSORSELECT, channel);
	sx9306_i2c_read(data, SX9306_REGOFFSETMSB, &msByte);
	sx9306_i2c_read(data, SX9306_REGOFFSETLSB, &lsByte);
	fullByte = (u16)((msByte << 8) | lsByte);

	return (int)fullByte;
}

static s32 sx9306_get_capMain(struct sx9306_p *data, u8 channel)
{
	u8 msByte = 0;
	u8 lsByte = 0;
	u16 offset = 0;
	s32 capMain = 0, useful = 0;

	useful = sx9306_get_useful(data, channel);
	offset = sx9306_get_offset(data, channel);

	msByte = (u8)(offset >> 6);
	lsByte = (u8)(offset - (((u16)msByte) << 6));

	capMain = 2 * (((s32)msByte * 3600) + ((s32)lsByte * 225)) +
		(((s32)useful * 50000) / (8 * 65536));

	if (channel == MAIN_SENSOR)
		pr_info("[SX9306]: %s - CapMain: %d,Useful: %d,Offset: %u\n",
			__func__, capMain, useful, offset);

	return capMain;
}

static s32 sx9306_calc_capMain(struct sx9306_p *data)
{
	s32 capMain;
#ifdef CONFIG_SENSORS_SX9306_TEMPERATURE_COMPENSATION
	s32 capRef;

	if (data->calData[0] > 0) {
		capRef = sx9306_get_capMain(data, REF_SENSOR);
		capMain = sx9306_get_capMain(data, MAIN_SENSOR);
		capMain = capMain - ((capRef - data->calData[3]) *
				TOUCH_CHECK_SLOPE);

		pr_info("[SX9306]: %s - After temp comp CapsMain: %d(%d)\n",
			__func__, capMain, capRef);
	} else
#endif
	capMain = sx9306_get_capMain(data, MAIN_SENSOR);

	return capMain;
}

static void sx9306_touchCheckWithRefSensor(struct sx9306_p *data)
{
	s32 capMain, threshold;

	threshold = sx9306_get_init_threshold(data);
	capMain = sx9306_calc_capMain(data);

	if (capMain >= threshold)
		send_event(data, ACTIVE);
	else
		send_event(data, IDLE);
}

static int sx9306_save_caldata(struct sx9306_p *data)
{
	struct file *cal_filp = NULL;
	mm_segment_t old_fs;
	int ret = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	cal_filp = filp_open(CALIBRATION_FILE_PATH,
			O_CREAT | O_TRUNC | O_WRONLY | O_SYNC,
			S_IRUGO | S_IWUSR | S_IWGRP);
	if (IS_ERR(cal_filp)) {
		pr_err("[SX9306]: %s - Can't open calibration file\n",
			__func__);
		set_fs(old_fs);
		ret = PTR_ERR(cal_filp);
		return ret;
	}

	ret = cal_filp->f_op->write(cal_filp, (char *)data->calData,
		sizeof(int) * CAL_DATA_NUM, &cal_filp->f_pos);
	if (ret != (sizeof(int) * CAL_DATA_NUM)) {
		pr_err("[SX9306]: %s - Can't write the cal data to file\n",
			__func__);
		ret = -EIO;
	}

	filp_close(cal_filp, current->files);
	set_fs(old_fs);

	return ret;
}

static void sx9306_open_caldata(struct sx9306_p *data)
{
	struct file *cal_filp = NULL;
	mm_segment_t old_fs;
	int ret;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	cal_filp = filp_open(CALIBRATION_FILE_PATH, O_RDONLY,
			S_IRUGO | S_IWUSR | S_IWGRP);
	if (IS_ERR(cal_filp)) {
		ret = PTR_ERR(cal_filp);
		if (ret != -ENOENT)
			pr_err("[SX9306]: %s - Can't open calibration file.\n",
				__func__);
		else {
			pr_info("[SX9306]: %s - There is no calibration file\n",
				__func__);
			/* calibration status init */
			memset(data->calData, 0, sizeof(int) * CAL_DATA_NUM);
		}
		set_fs(old_fs);
		return;
	}

	ret = cal_filp->f_op->read(cal_filp, (char *)data->calData,
		sizeof(int) * CAL_DATA_NUM, &cal_filp->f_pos);
	if (ret != (sizeof(int) * CAL_DATA_NUM))
		pr_err("[SX9306]: %s - Can't read the cal data from file\n",
			__func__);

	filp_close(cal_filp, current->files);
	set_fs(old_fs);

#ifdef CONFIG_SENSORS_SX9306_TEMPERATURE_COMPENSATION
	pr_info("[SX9306]: %s - (%d, %d, %d, %d)\n", __func__, data->calData[0],
		data->calData[1], data->calData[2], data->calData[3]);
#else
	pr_info("[SX9306]: %s - (%d, %d, %d)\n", __func__,
		data->calData[0], data->calData[1], data->calData[2]);
#endif
}

static int sx9306_set_mode(struct sx9306_p *data, unsigned char mode)
{
	int ret = -EINVAL;

	mutex_lock(&data->mode_mutex);
	if (mode == SX9306_MODE_SLEEP) {
		ret = sx9306_i2c_write(data, SX9306_CPS_CTRL0_REG, 0x20);
		disable_irq(data->irq);
		disable_irq_wake(data->irq);
	} else if (mode == SX9306_MODE_NORMAL) {
		ret = sx9306_i2c_write(data, SX9306_CPS_CTRL0_REG,
			0x20 | ENABLE_CSX);
		msleep(20);

		sx9306_set_offset_calibration(data);
		msleep(400);

		sx9306_touchCheckWithRefSensor(data);
		enable_irq(data->irq);
		enable_irq_wake(data->irq);
	}

	/* make sure no interrupts are pending since enabling irq
	 * will only work on next falling edge */
	sx9306_read_irqstate(data);

	pr_info("[SX9306]: %s - change the mode : %u\n", __func__, mode);

	mutex_unlock(&data->mode_mutex);
	return ret;
}

static int sx9306_do_calibrate(struct sx9306_p *data, bool do_calib)
{
	int ret = 0;

	if (do_calib == false) {
		pr_info("[SX9306]: %s - Erase!\n", __func__);
		goto cal_erase;
	}

	if (atomic_read(&data->enable) == OFF)
		sx9306_set_mode(data, SX9306_MODE_NORMAL);

	data->calData[2] = sx9306_get_offset(data, MAIN_SENSOR);
	if ((data->calData[2] >= LIMIT_PROXOFFSET)
		|| (data->calData[2] == 0)) {
		pr_err("[SX9306]: %s - offset fail(%d)\n", __func__,
			data->calData[2]);
		goto cal_fail;
	}

	data->calData[1] = sx9306_get_useful(data, MAIN_SENSOR);
	if (data->calData[1] >= LIMIT_PROXUSEFUL) {
		pr_err("[SX9306]: %s - useful warning(%d)\n", __func__,
			data->calData[1]);
	}

	data->calData[0] = sx9306_get_capMain(data, MAIN_SENSOR);
#ifdef CONFIG_SENSORS_SX9306_TEMPERATURE_COMPENSATION
	data->calData[3] = sx9306_get_capMain(data, REF_SENSOR);
	pr_info("[SX9306]: %s - ref sensor %d\n", __func__, data->calData[3]);
#endif
	if (atomic_read(&data->enable) == OFF)
		sx9306_set_mode(data, SX9306_MODE_SLEEP);

	goto exit;

cal_fail:
	if (atomic_read(&data->enable) == OFF)
		sx9306_set_mode(data, SX9306_MODE_SLEEP);
	ret = -1;
cal_erase:
	memset(data->calData, 0, sizeof(int) * CAL_DATA_NUM);
exit:
	pr_info("[SX9306]: %s - (%d, %d, %d)\n", __func__,
		data->calData[0], data->calData[1], data->calData[2]);
	return ret;
}

static void sx9306_set_enable(struct sx9306_p *data, int enable)
{
	int pre_enable = atomic_read(&data->enable);

	if (enable) {
		if (pre_enable == OFF) {
			data->touchMode = INIT_TOUCH_MODE;
			data->calSuccessed = false;

			sx9306_open_caldata(data);
			sx9306_set_mode(data, SX9306_MODE_NORMAL);
			atomic_set(&data->enable, ON);
		}
	} else {
		if (pre_enable == ON) {
			sx9306_set_mode(data, SX9306_MODE_SLEEP);
			atomic_set(&data->enable, OFF);
		}
	}
}

static ssize_t sx9306_get_offset_calibration_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u8 val = 0;
	struct sx9306_p *data = dev_get_drvdata(dev);

	sx9306_i2c_read(data, SX9306_IRQSTAT_REG, &val);

	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t sx9306_set_offset_calibration_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long val;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (strict_strtoul(buf, 10, &val)) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return -EINVAL;
	}

	if (val)
		sx9306_set_offset_calibration(data);

	return count;
}

static ssize_t sx9306_register_write_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	int regist = 0, val = 0;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (sscanf(buf, "%d,%d", &regist, &val) != 2) {
		pr_err("[SX9306]: %s - The number of data are wrong\n",
			__func__);
		return -EINVAL;
	}

	sx9306_i2c_write(data, (unsigned char)regist, (unsigned char)val);
	pr_info("[SX9306]: %s - Register(0x%2x) data(0x%2x)\n",
		__func__, regist, val);

	return count;
}

static ssize_t sx9306_register_read_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	int regist = 0;
	unsigned char val = 0;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (sscanf(buf, "%d", &regist) != 1) {
		pr_err("[SX9306]: %s - The number of data are wrong\n",
			__func__);
		return -EINVAL;
	}

	sx9306_i2c_read(data, (unsigned char)regist, &val);
	pr_info("[SX9306]: %s - Register(0x%2x) data(0x%2x)\n",
		__func__, regist, val);

	return count;
}

static ssize_t sx9306_read_data_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	sx9306_display_data_reg(data);

	return snprintf(buf, PAGE_SIZE, "%d\n", 0);
}

static ssize_t sx9306_sw_reset_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int ret = 0;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (atomic_read(&data->enable) == ON)
		sx9306_set_mode(data, SX9306_MODE_SLEEP);

	ret = sx9306_i2c_write(data, SX9306_SOFTRESET_REG, SX9306_SOFTRESET);
	msleep(300);

	sx9306_initialize_chip(data);

	if (atomic_read(&data->enable) == ON)
		sx9306_set_mode(data, SX9306_MODE_NORMAL);

	return snprintf(buf, PAGE_SIZE, "%d\n", ret);
}

static ssize_t sx9306_vendor_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", VENDOR_NAME);
}

static ssize_t sx9306_name_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", MODEL_NAME);
}

static ssize_t sx9306_touch_mode_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", data->touchMode);
}

static ssize_t sx9306_raw_data_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	s16 useful;
	u16 offset;
	s32 capMain;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (atomic_read(&data->enable) == OFF)
		pr_err("[SX9306]: %s - SX9306 was not enabled\n", __func__);

	capMain = sx9306_calc_capMain(data);
	useful = sx9306_get_useful(data, MAIN_SENSOR);
	offset = sx9306_get_offset(data, MAIN_SENSOR);

	return snprintf(buf, PAGE_SIZE, "%d,%d,%u\n", capMain, useful, offset);
}

static ssize_t sx9306_threshold_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	/* It's for init touch */
	return snprintf(buf, PAGE_SIZE, "%d\n",
			sx9306_get_init_threshold(data));
}

static ssize_t sx9306_threshold_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long val;
	struct sx9306_p *data = dev_get_drvdata(dev);

	/* It's for init touch */
	if (strict_strtoul(buf, 10, &val)) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return -EINVAL;
	}

	pr_info("[SX9306]: %s - init threshold %lu\n", __func__, val);
	data->initTh = (int)val;

	return count;
}

static ssize_t sx9306_normal_threshold_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	/* It's for normal touch */
	return snprintf(buf, PAGE_SIZE, "%d\n", data->touchTh);
}

static ssize_t sx9306_normal_threshold_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned long val;
	struct sx9306_p *data = dev_get_drvdata(dev);

	/* It's for normal touch */
	if (strict_strtoul(buf, 10, &val)) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return -EINVAL;
	}

	pr_info("[SX9306]: %s - normal threshold %lu\n", __func__, val);
	data->touchTh = (u8)val;

	return count;
}

static ssize_t sx9306_onoff_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%u\n", !data->flagDataSkip);
}

static ssize_t sx9306_onoff_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	u8 val;
	int ret;
	struct sx9306_p *data = dev_get_drvdata(dev);

	ret = kstrtou8(buf, 2, &val);
	if (ret) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return ret;
	}

	if (val == 0)
		data->flagDataSkip = true;
	else
		data->flagDataSkip = false;

	pr_info("[SX9306]: %s -%u\n", __func__, val);
	return count;
}

static ssize_t sx9306_calibration_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int ret;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if ((data->calSuccessed == false) && (data->calData[0] == 0))
		ret = CAL_RET_NONE;
	else if ((data->calSuccessed == false) && (data->calData[0] != 0))
		ret = CAL_RET_EXIST;
	else if ((data->calSuccessed == true) && (data->calData[0] != 0))
		ret = CAL_RET_SUCCESS;
	else
		ret = CAL_RET_ERROR;

	return snprintf(buf, PAGE_SIZE, "%d,%d,%d\n", ret,
			data->calData[1], data->calData[2]);
}

static ssize_t sx9306_calibration_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	bool do_calib;
	int ret;
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (sysfs_streq(buf, "1"))
		do_calib = true;
	else if (sysfs_streq(buf, "0"))
		do_calib = false;
	else {
		pr_info("[SX9306]: %s - invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}

	ret = sx9306_do_calibrate(data, do_calib);
	if (ret < 0) {
		pr_err("[SX9306]: %s - sx9306_do_calibrate fail(%d)\n",
			__func__, ret);
		goto exit;
	}

	ret = sx9306_save_caldata(data);
	if (ret < 0) {
		pr_err("[SX9306]: %s - sx9306_save_caldata fail(%d)\n",
			__func__, ret);
		memset(data->calData, 0, sizeof(int) * 3);
		goto exit;
	}

	pr_info("[SX9306]: %s - %u success!\n", __func__, do_calib);

exit:

	if ((data->calData[0] != 0) && (ret >= 0))
		data->calSuccessed = true;
	else
		data->calSuccessed = false;

	return count;
}

#ifdef CONFIG_SENSORS_GRIP_SIMDET
static ssize_t sx9306_sim_type_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	pr_info("[SX9306]: %s - sim type = %d!\n", __func__, data->sim_type);

	return snprintf(buf, PAGE_SIZE, "%d\n", data->sim_type);
}

static ssize_t sx9306_sim_type_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (sysfs_streq(buf, "1"))
		data->sim_type = 1;
	else
		data->sim_type = 0;

	pr_info("[SX9306]: %s - sim type = %d!\n", __func__, data->sim_type);

	return count;
}
#endif

static DEVICE_ATTR(menual_calibrate, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_get_offset_calibration_show,
		sx9306_set_offset_calibration_store);
static DEVICE_ATTR(register_write, S_IWUSR | S_IWGRP,
		NULL, sx9306_register_write_store);
static DEVICE_ATTR(register_read, S_IWUSR | S_IWGRP,
		NULL, sx9306_register_read_store);
static DEVICE_ATTR(readback, S_IRUGO, sx9306_read_data_show, NULL);
static DEVICE_ATTR(reset, S_IRUGO, sx9306_sw_reset_show, NULL);

static DEVICE_ATTR(name, S_IRUGO, sx9306_name_show, NULL);
static DEVICE_ATTR(vendor, S_IRUGO, sx9306_vendor_show, NULL);
static DEVICE_ATTR(mode, S_IRUGO, sx9306_touch_mode_show, NULL);
static DEVICE_ATTR(raw_data, S_IRUGO, sx9306_raw_data_show, NULL);
static DEVICE_ATTR(calibration, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_calibration_show, sx9306_calibration_store);
static DEVICE_ATTR(onoff, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_onoff_show, sx9306_onoff_store);
static DEVICE_ATTR(threshold, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_threshold_show, sx9306_threshold_store);
static DEVICE_ATTR(normal_threshold, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_normal_threshold_show, sx9306_normal_threshold_store);
#ifdef CONFIG_SENSORS_GRIP_SIMDET
static DEVICE_ATTR(sim_type, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_sim_type_show, sx9306_sim_type_store);
#endif

static struct device_attribute *sensor_attrs[] = {
	&dev_attr_menual_calibrate,
	&dev_attr_register_write,
	&dev_attr_register_read,
	&dev_attr_readback,
	&dev_attr_reset,
	&dev_attr_name,
	&dev_attr_vendor,
	&dev_attr_mode,
	&dev_attr_raw_data,
	&dev_attr_threshold,
	&dev_attr_normal_threshold,
	&dev_attr_onoff,
	&dev_attr_calibration,
#ifdef CONFIG_SENSORS_GRIP_SIMDET
	&dev_attr_sim_type,
#endif
	NULL,
};

/*****************************************************************************/
static ssize_t sx9306_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	u8 enable;
	int ret;
	struct sx9306_p *data = dev_get_drvdata(dev);

	ret = kstrtou8(buf, 2, &enable);
	if (ret) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return ret;
	}

	pr_info("[SX9306]: %s - new_value = %u\n", __func__, enable);
	if ((enable == 0) || (enable == 1))
		sx9306_set_enable(data, (int)enable);

	return size;
}

static ssize_t sx9306_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&data->enable));
}

static ssize_t sx9306_flush_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	u8 enable;
	int ret;
	struct sx9306_p *data = dev_get_drvdata(dev);

	ret = kstrtou8(buf, 2, &enable);
	if (ret) {
		pr_err("[SX9306]: %s - Invalid Argument\n", __func__);
		return ret;
	}

	if (enable == 1) {
		mutex_lock(&data->mode_mutex);
		input_report_rel(data->input, REL_MAX, 1);
		input_sync(data->input);
		mutex_unlock(&data->mode_mutex);
	}

	return size;
}

static DEVICE_ATTR(enable, S_IRUGO | S_IWUSR | S_IWGRP,
		sx9306_enable_show, sx9306_enable_store);
static DEVICE_ATTR(flush, S_IWUSR | S_IWGRP,
		NULL, sx9306_flush_store);

static struct attribute *sx9306_attributes[] = {
	&dev_attr_enable.attr,
	&dev_attr_flush.attr,
	NULL
};

static struct attribute_group sx9306_attribute_group = {
	.attrs = sx9306_attributes
};

static void sx9306_touch_process(struct sx9306_p *data, u8 flag)
{
	u8 status = 0;
	s32 capMain, threshold;

	threshold = sx9306_get_init_threshold(data);
	capMain = sx9306_calc_capMain(data);
	sx9306_i2c_read(data, SX9306_TCHCMPSTAT_REG, &status);

	if (flag & SX9306_IRQSTAT_COMPDONE_FLAG) {
		if (data->state == IDLE) {
			if (status & (CSX_STATUS_REG << MAIN_SENSOR))
				send_event(data, ACTIVE);
			else if (capMain > threshold)
				send_event(data, ACTIVE);
			else
				pr_info("[SX9306]: %s - already released.\n",
					__func__);
		} else { /* User released button */
			if (status & (CSX_STATUS_REG << MAIN_SENSOR))
				pr_info("[SX9306]: %s - still touched.\n",
					__func__);
			else if (capMain <= threshold)
				send_event(data, IDLE);
			else
				pr_info("[SX9306]: %s - still touched.\n",
					__func__);
		}

		return;
	}

	if (data->state == IDLE) {
		if (status & (CSX_STATUS_REG << MAIN_SENSOR)) {
#ifdef DEFENCE_CODE_FOR_DEVICE_DAMAGE
			if ((data->calData[0] - 5000) > capMain) {
				sx9306_set_offset_calibration(data);
				msleep(400);
				sx9306_touchCheckWithRefSensor(data);
				pr_err("[SX9306]: %s - Defence code for"
					" device damage\n", __func__);
				return;
			}
#endif
			send_event(data, ACTIVE);
		} else {
			pr_info("[SX9306]: %s - already released.\n", __func__);
		}
	} else { /* User released button */
		if (!(status & (CSX_STATUS_REG << MAIN_SENSOR))) {
			if ((data->touchMode == INIT_TOUCH_MODE)
				&& (capMain >= threshold))
				pr_info("[SX9306]: %s - IDLE SKIP\n",
					__func__);
			else
				send_event(data , IDLE);
		} else {
			pr_info("[SX9306]: %s - still touched\n", __func__);
		}
	}
}

static void sx9306_process_interrupt(struct sx9306_p *data)
{
	u8 flag = 0;

	/* since we are not in an interrupt don't need to disable irq. */
	flag = sx9306_read_irqstate(data);

	if (flag & IRQ_PROCESS_CONDITION)
		sx9306_touch_process(data, flag);
}

static void sx9306_init_work_func(struct work_struct *work)
{
	struct sx9306_p *data = container_of((struct delayed_work *)work,
		struct sx9306_p, init_work);

	sx9306_initialize_chip(data);

	/* make sure no interrupts are pending since enabling irq
	 * will only work on next falling edge */
	sx9306_read_irqstate(data);
}

static void sx9306_irq_work_func(struct work_struct *work)
{
	struct sx9306_p *data = container_of((struct delayed_work *)work,
		struct sx9306_p, irq_work);

	sx9306_process_interrupt(data);
}

#ifdef CONFIG_SENSORS_GRIP_ADJDET
static irqreturn_t sx9306_adjdet_interrupt_thread(int irq, void *pdata)
{
	struct sx9306_p *data = pdata;

	if (gpio_get_value_cansleep(data->gpio_adjdet) == 0)
		pr_info("[SX9306]: %s : adj detect cable disconnect!" \
				" grip sensor enable\n", __func__);
	else {
		pr_info("[SX9306]: %s : adj detect cable connect!" \
				" grip sensordisable\n", __func__);
		if (data->grip_state == ACTIVE) {
			pr_info("[SX9306]: %s : Send FAR(IDLE)\n", __func__);
			input_report_rel(data->input, REL_MISC, 2);
			input_sync(data->input);
			data->grip_state = IDLE;
		}
	}

	return IRQ_HANDLED;
}
#endif

static irqreturn_t sx9306_interrupt_thread(int irq, void *pdata)
{
	struct sx9306_p *data = pdata;

	if (sx9306_get_nirq_state(data) == 1) {
		pr_err("[SX9306]: %s - nirq read high\n", __func__);
	} else {
		wake_lock_timeout(&data->grip_wake_lock, HZ / 8);
		schedule_delayed_work(&data->irq_work, msecs_to_jiffies(100));
	}

	return IRQ_HANDLED;
}

static int sx9306_input_init(struct sx9306_p *data)
{
	int ret = 0;
	struct input_dev *dev = NULL;

	/* Create the input device */
	dev = input_allocate_device();
	if (!dev)
		return -ENOMEM;

	dev->name = MODULE_NAME;
	dev->id.bustype = BUS_I2C;

	input_set_capability(dev, EV_REL, REL_MISC);
	input_set_capability(dev, EV_REL, REL_MAX);
	input_set_drvdata(dev, data);

	ret = input_register_device(dev);
	if (ret < 0) {
		input_free_device(dev);
		return ret;
	}

	ret = sensors_create_symlink(dev);
	if (ret < 0) {
		input_unregister_device(dev);
		return ret;
	}

	ret = sysfs_create_group(&dev->dev.kobj, &sx9306_attribute_group);
	if (ret < 0) {
		sensors_remove_symlink(data->input);
		input_unregister_device(dev);
		return ret;
	}

	/* save the input pointer and finish initialization */
	data->input = dev;

	return 0;
}

static int sx9306_setup_pin(struct sx9306_p *data)
{
	int ret;

	ret = gpio_request(data->gpioNirq, "SX9306_nIRQ");
	if (ret < 0) {
		pr_err("[SX9306]: %s - gpio %d request failed (%d)\n",
			__func__, data->gpioNirq, ret);
		return ret;
	}

	ret = gpio_direction_input(data->gpioNirq);
	if (ret < 0) {
		pr_err("[SX9306]: %s - failed to set gpio %d as input (%d)\n",
			__func__, data->gpioNirq, ret);
		gpio_free(data->gpioNirq);
		return ret;
	}

#ifdef CONFIG_SENSORS_GRIP_ADJDET
	if (data->enable_adjdet == 1) {
		ret = gpio_request(data->gpio_adjdet, "SX9306_ADJDETIRQ");
		if (ret < 0) {
			pr_err("[SX9306]: %s - gpio %d request failed (%d)\n",
				__func__, data->gpio_adjdet, ret);
			return ret;
		}

		ret = gpio_direction_input(data->gpio_adjdet);

		if (ret < 0) {
			pr_err("[SX9306]: %s - failed to set" \
				" gpio %d as input (%d)\n",
				__func__, data->gpio_adjdet, ret);
			gpio_free(data->gpio_adjdet);
			return ret;
		}
	}
#endif
	return 0;
}

static void sx9306_initialize_variable(struct sx9306_p *data)
{
	data->state = IDLE;
	data->touchMode = INIT_TOUCH_MODE;
	data->flagDataSkip = false;
	data->calSuccessed = false;
	memset(data->calData, 0, sizeof(int) * 3);
	atomic_set(&data->enable, OFF);

	data->initTh = (int)CONFIG_SENSORS_SX9306_INIT_TOUCH_THRESHOLD;
	pr_info("[SX9306]: %s - Init Touch Threshold : %d\n",
		__func__, data->initTh);

#ifdef CONFIG_SENSORS_GRIP_SIMDET
	data->sim_type = 0;
#endif

#ifdef CONFIG_MACH_TBLTE_VZW
	if (system_rev < 15)
		data->touchTh = 30;
	else
		data->touchTh = (u8)CONFIG_SENSORS_SX9306_NORMAL_TOUCH_THRESHOLD;
#else
	data->touchTh = (u8)CONFIG_SENSORS_SX9306_NORMAL_TOUCH_THRESHOLD;
#endif
	pr_info("[SX9306]: %s - Normal Touch Threshold : %u\n",
		__func__, data->touchTh);
}

static int sx9306_parse_dt(struct sx9306_p *data, struct device *dev)
{
	struct device_node *dNode = dev->of_node;
	enum of_gpio_flags flags;

	if (dNode == NULL)
		return -ENODEV;

	data->gpioNirq = of_get_named_gpio_flags(dNode,
		"sx9306-i2c,nirq-gpio", 0, &flags);
	if (data->gpioNirq < 0) {
		pr_err("[SX9306]: %s - get gpioNirq error\n", __func__);
		return -ENODEV;
	}

#ifdef CONFIG_SENSORS_GRIP_ADJDET
	data->gpio_adjdet = of_get_named_gpio_flags(dNode,
		"sx9306-i2c,adjdet-gpio", 0, &flags);
	if (data->gpio_adjdet < 0) {
		pr_err("[SX9306]: %s - get gpio_adjdet error\n", __func__);
		data->enable_adjdet = 0;
	} else {
		pr_info("[SX9306]: %s - get gpio_adjdet success\n", __func__);
		data->enable_adjdet = 1;
	}
#endif

	if (of_property_read_u32(dNode, "sx9306-i2c,gate-type",
		&data->gatetype))
		data->gatetype = SX9306_OR_GATE;

	return 0;
}

#if defined(CONFIG_SENSORS_SX9306_REGULATOR_ONOFF)
int sx9306_regulator_on(struct sx9306_p *data, bool onoff)
{
	int ret = -1;
	if (!data->vdd) {
		data->vdd = devm_regulator_get(&data->client->dev, "vdd");
		if (!data->vdd) {
			pr_err("%s: regulator pointer null vdd, rc=%d\n",
				__func__, ret);
			return ret;
		}
		ret = regulator_set_voltage(data->vdd, 2850000, 2850000);
		if (ret) {
			pr_err("%s: set voltage failed on vdd, rc=%d\n",
				__func__, ret);
			return ret;
		}
	}


	if (!data->ldo) {
		data->ldo = devm_regulator_get(&data->client->dev, "vdd_ldo");
		if (!data->ldo) {
			pr_err("%s: devm_regulator_get for vdd_ldo failed\n",
				__func__);
			return 0;
		}
	}

	if (onoff) {
		ret = regulator_enable(data->vdd);
		if (ret) {
			pr_err("%s: Failed to enable regulator vdd.\n",
				__func__);
			return ret;
		}

		ret = regulator_enable(data->ldo);
		if (ret) {
			pr_err("%s: Failed to enable regulator ldo.\n",
				__func__);
			return ret;
		}
	} else {
		ret = regulator_disable(data->vdd);
		if (ret) {
			pr_err("%s: Failed to disable regulator vdd.\n",
				__func__);
			return ret;
		}

		ret = regulator_disable(data->ldo);
		if (ret) {
			pr_err("%s: Failed to disable regulator ldo.\n",
				__func__);
			return ret;
		}

	}
	/*Delay added for wakeup of chip, before i2c-transactions */
	msleep(30);
	return 0;
}
#endif

static int sx9306_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret = -ENODEV;
	unsigned long irqflags = 0;
	struct sx9306_p *data = NULL;

	pr_info("[SX9306]: %s - Probe Start!\n", __func__);
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("[SX9306]: %s - i2c_check_functionality error\n",
			__func__);
		goto exit;
	}

	/* create memory for main struct */
	data = kzalloc(sizeof(struct sx9306_p), GFP_KERNEL);
	if (data == NULL) {
		pr_err("[SX9306]: %s - kzalloc error\n", __func__);
		ret = -ENOMEM;
		goto exit_kzalloc;
	}

	ret = sx9306_parse_dt(data, &client->dev);
	if (ret < 0) {
		pr_err("[SX9306]: %s - of_node error\n", __func__);
		ret = -ENODEV;
		goto exit_of_node;
	}

	ret = sx9306_setup_pin(data);
	if (ret) {
		pr_err("[SX9306]: %s - could not setup pin\n", __func__);
		goto exit_setup_pin;
	}

	i2c_set_clientdata(client, data);
	data->client = client;
	data->factory_device = &client->dev;
#if defined(CONFIG_SENSORS_SX9306_REGULATOR_ONOFF)
	sx9306_regulator_on(data, 1);
#endif

	/* read chip id */
	ret = sx9306_i2c_write(data, SX9306_SOFTRESET_REG, SX9306_SOFTRESET);
	if (ret < 0) {
		pr_err("[SX9306]: %s - chip reset failed %d\n", __func__, ret);
		goto exit_chip_reset;
	}

	ret = sx9306_input_init(data);
	if (ret < 0)
		goto exit_input_init;

	wake_lock_init(&data->grip_wake_lock,
		WAKE_LOCK_SUSPEND, "grip_wake_lock");

	ret = sensors_register(data->factory_device,
		data, sensor_attrs, MODULE_NAME);
	if (ret) {
		pr_err("[SENSOR] %s - cound not register grip_sensor(%d).\n",
			__func__, ret);
		goto grip_sensor_register_failed;
	}

	sx9306_initialize_variable(data);

	INIT_DELAYED_WORK(&data->init_work, sx9306_init_work_func);
	INIT_DELAYED_WORK(&data->irq_work, sx9306_irq_work_func);

	mutex_init(&data->mode_mutex);

	data->irq = gpio_to_irq(data->gpioNirq);

	/* initailize interrupt reporting */
	if (data->gatetype == SX9306_NOR_GATE)
		irqflags = IRQF_TRIGGER_RISING | IRQF_ONESHOT;
	else
		irqflags = IRQF_TRIGGER_FALLING | IRQF_ONESHOT;
	ret = request_threaded_irq(data->irq, NULL, sx9306_interrupt_thread,
			irqflags , "sx9306_irq", data);
	if (ret < 0) {
		pr_err("[SX9306]: %s - failed to set request_threaded_irq %d" \
			" as returning (%d)\n", __func__, data->irq, ret);
		goto exit_request_threaded_irq;
	}

#ifdef CONFIG_SENSORS_GRIP_ADJDET
	if (data->enable_adjdet == 1) {
		data->irq_adjdet = gpio_to_irq(data->gpio_adjdet);
		/* initailize interrupt reporting */
		ret = request_threaded_irq(data->irq_adjdet, NULL,
				sx9306_adjdet_interrupt_thread,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"sx9306_adjdet_irq", data);
		if (ret < 0) {
			pr_err("[SX9306]: %s - failed to set" \
				" request_threaded_adjdet_irq %d" \
				" as returning (%d)\n",
				__func__, data->irq_adjdet, ret);
			goto exit_request_threaded_adjdet_irq;
		}
		data->grip_state = IDLE;
	}
#endif

	disable_irq(data->irq);

	schedule_delayed_work(&data->init_work, msecs_to_jiffies(300));
	pr_info("[SX9306]: %s - Probe done!\n", __func__);

	return 0;
#ifdef CONFIG_SENSORS_GRIP_ADJDET
exit_request_threaded_adjdet_irq:
	free_irq(data->irq, data);
#endif
exit_request_threaded_irq:
	mutex_destroy(&data->mode_mutex);
	sensors_unregister(data->factory_device, sensor_attrs);
	sensors_remove_symlink(data->input);
	wake_lock_destroy(&data->grip_wake_lock);
	sysfs_remove_group(&data->input->dev.kobj, &sx9306_attribute_group);
grip_sensor_register_failed:
	input_unregister_device(data->input);
exit_input_init:
exit_chip_reset:
	gpio_free(data->gpioNirq);
exit_setup_pin:
exit_of_node:
	kfree(data);
exit_kzalloc:
exit:
	pr_err("[SX9306]: %s - Probe fail!\n", __func__);
	return ret;
}

static int sx9306_remove(struct i2c_client *client)
{
	struct sx9306_p *data = (struct sx9306_p *)i2c_get_clientdata(client);

	if (atomic_read(&data->enable) == ON)
		sx9306_set_mode(data, SX9306_MODE_SLEEP);

	cancel_delayed_work_sync(&data->init_work);
	cancel_delayed_work_sync(&data->irq_work);
	free_irq(data->irq, data);
#ifdef CONFIG_SENSORS_GRIP_ADJDET
	if (data->enable_adjdet == 1)
		free_irq(data->irq_adjdet, data);
#endif
	gpio_free(data->gpioNirq);

	wake_lock_destroy(&data->grip_wake_lock);
	sensors_unregister(data->factory_device, sensor_attrs);
	sensors_remove_symlink(data->input);
	sysfs_remove_group(&data->input->dev.kobj, &sx9306_attribute_group);
	input_unregister_device(data->input);
	mutex_destroy(&data->mode_mutex);

	kfree(data);

	return 0;
}

static int sx9306_suspend(struct device *dev)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (atomic_read(&data->enable) == ON)
		pr_info("[SX9306]: %s\n", __func__);

	return 0;
}

static int sx9306_resume(struct device *dev)
{
	struct sx9306_p *data = dev_get_drvdata(dev);

	if (atomic_read(&data->enable) == ON)
		pr_info("[SX9306]: %s\n", __func__);

	return 0;
}

static struct of_device_id sx9306_match_table[] = {
	{ .compatible = "sx9306-i2c",},
	{},
};

static const struct i2c_device_id sx9306_id[] = {
	{ "sx9306_match_table", 0 },
	{ }
};

static const struct dev_pm_ops sx9306_pm_ops = {
	.suspend = sx9306_suspend,
	.resume = sx9306_resume,
};

static struct i2c_driver sx9306_driver = {
	.driver = {
		.name	= MODEL_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = sx9306_match_table,
		.pm = &sx9306_pm_ops
	},
	.probe		= sx9306_probe,
	.remove		= sx9306_remove,
	.id_table	= sx9306_id,
};

static int __init sx9306_init(void)
{
	return i2c_add_driver(&sx9306_driver);
}

static void __exit sx9306_exit(void)
{
	i2c_del_driver(&sx9306_driver);
}

module_init(sx9306_init);
module_exit(sx9306_exit);

MODULE_DESCRIPTION("Semtech Corp. SX9306 Capacitive Touch Controller Driver");
MODULE_AUTHOR("Samsung Electronics");
MODULE_LICENSE("GPL");
