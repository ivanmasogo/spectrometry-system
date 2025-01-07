#include "importantFiles/compilationType.h"
#ifdef UNIX


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include "headers/as7265x.h"

using namespace std;
int check_if_available(int i2c_fd) {
	int value = 0; 
	try {
		value = i2cm_read(i2c_fd, AS7265X_HW_VERSION_HIGH);
		return value;
	}
	catch (int e) {
		return 0;
	}
	//if device is available, value must be !=0

}
/**
 * Write to AS7265x virtual register. Based on code in the AS7265x datasheet.
 */
void as7265x_vreg_write(int i2c_fd, uint8_t virtualReg, uint8_t d)
{
	volatile uint8_t status;
	while (1)
	{
		// Read slave I²C status to see if the write buffer is ready.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0)
			// No inbound TX pending at slave. Okay to write now.
			break;
	}
	// Send the virtual register address (enabling bit 7 to indicate a write).
	i2cm_write(i2c_fd, I2C_AS72XX_SLAVE_WRITE_REG, (virtualReg | 0x80));
	while (1)
	{
		// Read the slave I²C status to see if the write buffer is ready.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0)
			// No inbound TX pending at slave. Okay to write data now.
			break;
	}
	// Send the data to complete the operation.
	i2cm_write(i2c_fd, I2C_AS72XX_SLAVE_WRITE_REG, d);
}
/**
 * Read from AS7265x virtual register. Based on code in the AS7265x datasheet.
 */
uint8_t as7265x_vreg_read(int i2c_fd, uint8_t virtualReg)
{
	volatile uint8_t status, d;
	status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
	if ((status & I2C_AS72XX_SLAVE_RX_VALID) != 0) {
		// data to be read
		d = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_READ_REG);
	}

	// Wait for WRITE flag to clear
	while (1)
	{
		// Read slave I²C status to see if the read buffer is ready.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_TX_VALID) == 0)
			// No inbound TX pending at slave. Okay to write now.
			break;
	}

	// Send the virtual register address (disabling bit 7 to indicate a read).
	i2cm_write(i2c_fd, I2C_AS72XX_SLAVE_WRITE_REG, virtualReg);


	while (1)
	{
		// Read the slave I²C status to see if our read data is available.
		status = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_STATUS_REG);
		if ((status & I2C_AS72XX_SLAVE_RX_VALID) != 0)
			// Read data is ready.
			break;
	}
	// Read the data to complete the operation.
	d = i2cm_read(i2c_fd, I2C_AS72XX_SLAVE_READ_REG);
	return d;
}
/**
 * Test DATA_RDY flag of configuration virtual register (add
 *
 * @return 0 if not set, non-zero if set.
 */
int as7265x_is_data_available(int i2c_fd)
{
	int status = as7265x_vreg_read(i2c_fd, AS7265X_CONFIG);
	return (status & (1 << 1));
}
/**
 * Select device
 *
 * @param device 0=master; 1=first slave; 2=second slave
 */
void as7265x_device_select(int i2c_fd, uint8_t device) {
	as7265x_vreg_write(i2c_fd, AS7265X_DEV_SELECT_CONTROL, device);
}
/**
 * Set amplifier gain
 * @param gain 0= 1x (default), 1=3.7x, 2=16x, 3=64x
 */
void as7265x_set_gain(int i2c_fd, int gain)
{

	int value = as7265x_vreg_read(i2c_fd, AS7265X_CONFIG);
	value &= 0b11001111; // clear gain bits
	value |= (gain & 0b11) << 4;
	as7265x_vreg_write(i2c_fd, AS7265X_CONFIG, value);
}
/**
 * Set ADC integration time.
 *
 * @param time from 1..255. 2.8ms units.
 */
void as7265x_set_integration_time(int i2c_fd, uint8_t time)
{
	as7265x_vreg_write(i2c_fd, AS7265X_INTERGRATION_TIME, time);
}
void as7265x_indicator_enable(int i2c_fd)
{
	as7265x_device_select(i2c_fd, 0);
	uint8_t value = as7265x_vreg_read(i2c_fd, AS7265X_LED_CONFIG);
	value |= (1 << 0);
	as7265x_vreg_write(i2c_fd, AS7265X_LED_CONFIG, value);
}
void as7265x_indicator_disable(int i2c_fd)
{
	as7265x_device_select(i2c_fd, 0);
	uint8_t value = as7265x_vreg_read(i2c_fd, AS7265X_LED_CONFIG);
	value &= ~(1 << 0);
	as7265x_vreg_write(i2c_fd, AS7265X_LED_CONFIG, value);
}
/**
 * Set measurement mode.
 *
 * @param mode 2 = all 6 channels continuous; 3 = one shot all channels
 */
void as7265x_set_measurement_mode(int i2c_fd, uint8_t mode)
{
	uint8_t value = as7265x_vreg_read(i2c_fd, AS7265X_CONFIG);
	value &= 0b11110011;
	value |= (mode & 0b11) << 2;
	as7265x_vreg_write(i2c_fd, AS7265X_CONFIG, value);
}
/**
 * Read calibrated value (IEEE 754 float)
 */
float as7265x_get_calibrated_value(int i2c_fd, uint8_t device, uint8_t base_addr)
{
	int i;
	uint8_t value;
	uint32_t shift_reg = 0;

	as7265x_device_select(i2c_fd, device);

	for (i = base_addr; i < base_addr + 4; i++) {
		shift_reg <<= 8;
		value = as7265x_vreg_read(i2c_fd, i);
		shift_reg |= value;
	}
	// convert content of shift_reg to floating point
	float ret;
	memcpy(&ret, &shift_reg, sizeof(float));
	return ret;
}
/**
 * Read raw value (16 bit unsigned integer)
 */
int as7265x_get_raw_value(int i2c_fd, uint8_t device, uint8_t base_addr)
{
	as7265x_device_select(i2c_fd, device);
	uint32_t value = (as7265x_vreg_read(i2c_fd, base_addr) << 8);
	value |= as7265x_vreg_read(i2c_fd, base_addr + 1);
	return value;
}
/**
 * Read all 18 channels. Channels AS72651 (vis): channels 0-5, AS72652 (vis+IR): channels 6-11,
 * AS72653 (vis+UV): channels 12-17.
 */
void as7265x_get_all_calibrated_values(int i2c_fd, as7265x_channels_t* channels)
{

	uint8_t base_addr;
	int channel_index = 0;
	uint8_t device;
	float v;

	for (device = 0; device < 3; device++) {
		for (base_addr = 0x14; base_addr < 0x2c; base_addr += 4) {
			v = as7265x_get_calibrated_value(i2c_fd, device, base_addr);
			channels->channel[channel_index] = v;
			channel_index++;
		}
	}

}
/**
 * Read all 18 channels raw ADC. Channels AS72651 (vis): channels 0-5, AS72652 (vis+IR): channels 6-11,
 * AS72653 (vis+UV): channels 12-17
 */
void as7265x_get_all_raw_values(int i2c_fd, as7265x_channels_t* channels)
{
	int base_addr;
	int device;
	int channel_index = 0;
	for (device = 0; device < 3; device++) {
		for (base_addr = 0x8; base_addr < 0x14; base_addr += 2) {
			channels->channel[channel_index] = (uint16_t)as7265x_get_raw_value(i2c_fd, device, base_addr);
			channel_index++;
		}
	}

}
/**
 * Order channels in ascending wavelength.
 */
void as7265x_order_channels(int i2c_fd, as7265x_channels_t* channels)
{
	float buf[18];
	int i;
	for (i = 0; i < 18; i++) {
		buf[i] = channels->channel[as7265x_channel_order_table[i]];
	}
	for (i = 0; i < 18; i++) {
		channels->channel[i] = buf[i];
	}
}


/**
 * Open and return I2C bus file descriptor. Bus opened in RDRW mode.
 */
int i2c_init(const char* i2c_fname) {
    int i2c_fd;
    if ((i2c_fd = open(i2c_fname, O_RDWR)) < 0) {
        return -1; //Error caused by open
    }
    return i2c_fd;
}
/**
 * Close I2C bus file.
 */
void i2c_close(int i2c_fd) {
    close(i2c_fd);
}
/**
 * Write to I2C device register.
 *
 * @return 0 if successful, else -1.
 */
int i2c_register_write(int i2c_fd, uint8_t slave_addr, uint8_t reg, uint8_t data) {
    int retval;
    uint8_t outbuf[2];

    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    outbuf[0] = reg;
    outbuf[1] = data;

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = outbuf;
    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;
    if (ioctl(i2c_fd, I2C_RDWR, &msgset) < 0) {
        return -1; //Error caused by ioctl
    }
    return 0;
}
/**
 * Read I2C device register, returing result in 'result'.
 *
 * @return 0 if successful, else -1
 */
int i2c_register_read(int i2c_fd, uint8_t slave_addr, uint8_t reg, uint8_t* result) {
    int retval;
    uint8_t outbuf[1], inbuf[1];
    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = slave_addr;
    msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
    msgs[1].len = 1;
    msgs[1].buf = inbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    outbuf[0] = reg;

    inbuf[0] = 0;

    *result = 0;
    if (ioctl(i2c_fd, I2C_RDWR, &msgset) < 0) {
        return -1; //Erro by ioctl
    }

    *result = inbuf[0];
    return 0;
}
/**
 * Read a I2C (real) register from AS7265x
 */
int i2cm_read(int i2c_fd, int addr) {
	uint8_t result;
	i2c_register_read(i2c_fd, AS7265X_I2C_ADDR, addr, &result);
	return result;
}
/**
 * Write a I2C (real) register to AS7265x.
 */
int i2cm_write(int i2c_fd, int addr, int value) {
	i2c_register_write(i2c_fd, AS7265X_I2C_ADDR, addr, value);
	return 0; 
}
#endif // 