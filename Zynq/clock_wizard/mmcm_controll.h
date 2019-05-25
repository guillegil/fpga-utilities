#ifndef __MMCM_CONTROLLER_H__
#define __MMCM_CONTROLLER_H__

#include <stdint.h>
#include <stdbool.h>
#include "mmcm_reg.h"
#include "deviceschip.h"

#if !defined(MMCM_RESOURCES)
#define MMCM_RESOURCES 		XC7Z010CLG400_1
#endif // MMCM_RESOURCES


#define __write_reg(cfg, offset, data) (*((uint32_t *)(cfg + offset)) = data)
#define __read_reg(cfg, offset)		(*((uint32_t *)(cfg) + offset))
#define __get_frac(value, shift) (value >> shift)
#define __program_clock_device(cfg) (*((uint32_t *)(cfg + CLK_CONF_REG23)) = 0x07)

#define mmcm_prop_t   struct __mmcm_info


struct __mmcm_device
{
	void 	*cfg;
	int 	id;
	struct 	__mmcm_info info;
	float 	ideal_m ,m, d;
	bool 	is_active;
};

struct __mmcm_device __mmcm_devices[MMCM_RESOURCES];
static uint8_t __active_devices = 0;

int init_clock_device(int fd, const mmcm_prop_t *mmcm_info);
void close_clock_device(int id);

//int set_frequency(int id, uint8_t output, uint32_t freq);
float get_frequency(int id, uint8_t output);
float set_frequency(int id, uint8_t output, uint32_t freq);


#endif