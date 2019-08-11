#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

#define I2C_N_PORTS 4

#define I2C_ERR_INVALID_PORT -1
#define I2C_ERR_BUSY -2
#define I2C_ERR_FAULT -3
#define I2C_ERR_INVALID_LENGTH -4
#define I2C_ERR_BUS_BUSY -5

void i2c_disable(int port);
int i2c_enable(int port, int mode);
void i2c_disable_all(void);

void i2c_init(void);

int i2c_status(int port);
int i2c_start(int port, 
              uint32_t address, 
              uint8_t *data,
              int write_len,
              int read_len);

int i2c_complete( int port, uint8_t *data, uint32_t nbytes);
void i2c_test(void);

#endif
