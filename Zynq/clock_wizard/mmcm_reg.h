#ifndef __MMCM_AXI_DEF_H__
#define __MMCM_AXI_DEF_H__

#include <stdint.h>
#include <stdbool.h>


#define CLK_SRR                  (0x00)   // Software Reset Register                  (W)
#define CLK_SR                   (0x04)   // Status Register                          (R)
#define CLK_MONITOR_SR           (0x08)   // Clock Monitor Error Status Register      (R)
#define CLK_IS                   (0x0C)   // Interrupt Status                         (R/W)
#define CLK_IEN                  (0x10)   // Interrupt Enable                         (R/W)

#define CLK_CONF_REG0            (0x200)  // Clock Configuration Register 0           (R/W)
#define CLK_CONF_REG1            (0x204)  // Clock Configuration Register 1           (R/W)
#define CLK_CONF_REG2            (0x208)  // Clock Configuration Register 2           (R/W)
#define CLK_CONF_REG3            (0x20C)  // Clock Configuration Register 3           (R/W)
#define CLK_CONF_REG4            (0x210)  // Clock Configuration Register 4           (R/W)
#define CLK_CONF_REG5            (0x214)  // Clock Configuration Register 5           (R/W)
#define CLK_CONF_REG6            (0x218)  // Clock Configuration Register 6           (R/W)
#define CLK_CONF_REG7            (0x21C)  // Clock Configuration Register 7           (R/W)
#define CLK_CONF_REG8            (0x220)  // Clock Configuration Register 8           (R/W)
#define CLK_CONF_REG9            (0x224)  // Clock Configuration Register 9           (R/W)
#define CLK_CONF_REG10           (0x228)  // Clock Configuration Register 10          (R/W)
#define CLK_CONF_REG11           (0x22C)  // Clock Configuration Register 11          (R/W)
#define CLK_CONF_REG12           (0x230)  // Clock Configuration Register 12          (R/W)
#define CLK_CONF_REG13           (0x234)  // Clock Configuration Register 13          (R/W)
#define CLK_CONF_REG14           (0x238)  // Clock Configuration Register 14          (R/W)
#define CLK_CONF_REG15           (0x23C)  // Clock Configuration Register 15          (R/W)
#define CLK_CONF_REG16           (0x240)  // Clock Configuration Register 16          (R/W)
#define CLK_CONF_REG17           (0x244)  // Clock Configuration Register 17          (R/W)
#define CLK_CONF_REG18           (0x248)  // Clock Configuration Register 18          (R/W)
#define CLK_CONF_REG19           (0x24C)  // Clock Configuration Register 19          (R/W)
#define CLK_CONF_REG20           (0x250)  // Clock Configuration Register 20          (R/W)
#define CLK_CONF_REG21           (0x254)  // Clock Configuration Register 21          (R/W)
#define CLK_CONF_REG22           (0x258)  // Clock Configuration Register 22          (R/W)
#define CLK_CONF_REG23           (0x25C)  // Clock Configuration Register 23          (R/W)


#define MMCM_PRIMITIVE		     (false)
#define PLL_PRIMITIVE	         (true)


typedef union {					                    
    uint16_t all;			               

	struct {
		unsigned char frequency_synthesis :1;		
		unsigned char phase_aligment      :1;		  
		unsigned char dynamic_reconfig    :1;		    
		unsigned char safe_clock_startup  :1;		               
		unsigned char minimize_power      :1;		                
		unsigned char spread_spectrum     :1;		              
		unsigned char dynamic_phase_shift :1;		              
		unsigned char reset               :1;	
        unsigned char reset_active        :1;
        unsigned char locked              :1;
        unsigned char power_down          :1;          
	};

} __mmcm_features;


struct __mmcm_info
{
    uint32_t         addr;
    bool             primitive;
    float         	 fin; 
    uint8_t          active_outputs;    
    __mmcm_features  features;
};

void set_mmcm_addr(struct __mmcm_info *info, uint32_t addr);
void set_mmcm_primitive(struct __mmcm_info *info, bool primitive);
void set_mmcm_fin(struct __mmcm_info *info, uint32_t fin);
void set_mmcm_active_outputs(struct __mmcm_info *info, uint8_t active_outputs);
void set_mmcm_features(struct __mmcm_info *info, __mmcm_features features);


#endif // __MMCM_AXI_DEF_H__