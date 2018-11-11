/**********************
 * 
 * File name: ltc145_14.h
 * 
 * Description: Library to handle the ADC LTC2145-14 
 * with a Xilinx Zynq FPGA through SPI.
 * 
 * Author: Guillermo Gil Rodriguez
 * Date: 11-11-2018
 * 
 * Tested in: 
 * 
**********************/



#ifndef __LTC2145_14_H__
#define __LTC2145_14_H__

#include <stdint.h>

#define RESET_REG_ADDR                          0x00       // Write only
#define POWER_DOWN_REG_ADDR                     0x01       
#define TIMING_REG_ADDR                         0x02
#define OUTPUT_MODE_REG_ADDR                    0x03
#define DATA_FORMAT_REG_ADDR                    0x04

/***** RESET REGISTER MODE *****/

#define RESET_ALL                               0x10


/***** POWER DOWN REGISTER MODES *****/

#define PWDN_MODE0                              0x00       // Normal Operation.
#define PWDN_MODE1                              0x01       // Channel 1 in normal operation. Channel 2 in nap mode.
#define PWDN_MODE2                              0x02       // Channel 2 in normal operation. Channel 1 in nap mode.
#define PWDN_MODE3                              0x03       // Sleep mode.

/***** POWER DOWN REGISTER CANONICAL MODES *****/

#define NORMAL_OPERTAION                        PWDN_MODE0
#define CHANNEL1_NORMAL_OPERATION               PWDN_MODE1
#define CHANNEL2_NORMAL_OPERATION               PWDN_MODE2
#define ADC_SLEEP_MODE                          PWDN_MODE3


/***** TIMING REGISTER MASKS *****/

#define CLKOUT_NORMAL                           0x00
#define CLKOUT_INVERTED                         0x08        

#define CLKOUT_NO_DELAY                         0x00
#define CLKOUT_45DELAY                          0x02
#define CLKOUT_90DELAY                          0x04
#define CLKOUT_135DELAY                         0x06

#define CLK_DUTY_STABILIZER_OFF                 0x00
#define CLK_DUTY_STABILIZER_ON                  0x01

/***** TIMING REGISTER MODES *****/

#define TIMING_MODE0                            CLKOUT_NORMAL | CLKOUT_NO_DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_MODE1                            CLKOUT_NORMAL | CLKOUT_45DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_MODE2                            CLKOUT_NORMAL | CLKOUT_90DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_MODE3                            CLKOUT_NORMAL | CLKOUT_135DELAY | CLK_DUTY_STABILIZER_OFF

#define TIMING_MODE4                            CLKOUT_NORMAL | CLKOUT_NO_DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_MODE5                            CLKOUT_NORMAL | CLKOUT_45DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_MODE6                            CLKOUT_NORMAL | CLKOUT_90DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_MODE7                            CLKOUT_NORMAL | CLKOUT_135DELAY | CLK_DUTY_STABILIZER_ON

#define TIMING_INV_MODE0                        CLKOUT_INVERTED | CLKOUT_NO_DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_INV_MODE1                        CLKOUT_INVERTED | CLKOUT_45DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_INV_MODE2                        CLKOUT_INVERTED | CLKOUT_90DELAY | CLK_DUTY_STABILIZER_OFF
#define TIMING_INV_MODE3                        CLKOUT_INVERTED | CLKOUT_135DELAY | CLK_DUTY_STABILIZER_OFF

#define TIMING_INV_MODE4                        CLKOUT_INVERTED | CLKOUT_NO_DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_INV_MODE5                        CLKOUT_INVERTED | CLKOUT_45DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_INV_MODE6                        CLKOUT_INVERTED | CLKOUT_90DELAY | CLK_DUTY_STABILIZER_ON
#define TIMING_INV_MODE7                        CLKOUT_INVERTED | CLKOUT_135DELAY | CLK_DUTY_STABILIZER_ON


/***** OUTPUT MODE REGISTER MASKS *****/

#define LVDS_OC_3500UA                          0x00        // Output current set to 3.5mA
#define LVDS_OC_4000UA                          0x10        // Output current set to 4.0mA
#define LVDS_OC_4500UA                          0x20        // Output current set to 4.5mA
#define LVDS_OC_3000UA                          0x40        // Output current set to 3.0mA
#define LVDS_OC_2500UA                          0x50        // Output current set to 2.5mA
#define LVDS_OC_2100UA                          0x60        // Output current set to 2.1mA
#define LVDS_OC_1750UA                          0x70        // Output current set to 1.75mA

#define LVDS_OC_2X                              0x08

#define DIG_OUT_EN                              0x00        // Enable outputs
#define DIG_OUT_HIZ                             0x04        // Disable outputs: set them to HI-Z

#define SIMPLE_CMOS_OUTPUT                      0x00
#define DDR_LVDS_OUTPUT                         0x01
#define DDR_CMOS_OUTPUT                         0x02

/***** OUTPUT MODE REGISTER MASKS *****/

#define SIMPLE_CMOS                             DIG_OUT_EN | SIMPLE_CMOS_OUTPUT
#define DDR_LVSD                                DIG_OUT_EN | DDR_LVDS_OUTPUT
#define DDR_CMOS                                DIG_OUT_EN | DDR_CMOS_OUTPUT

#define DISABLE_OUTPUTS                         DIG_OUT_HIZ

/***** DATA FORMAT REGISTER MASKS *****/

#define DIG_OUT_TEST_OFF                        0x00    // Output test off
#define DIG_OUT_TEST_ZERO                       0x08    // Set digital outputs to 0's
#define DIG_OUT_TEST_ONES                       0x18    // Set digital outputs to 1's
#define DIG_OUT_TEST_ALTERNATE1                 0x28    // Alternate outputs between 1 01 0101 0101 0101 and 0 10 1010 1010 1010
#define DIG_OUT_TEST_ALTERNATE2                 0x38    // Alternate outputs between 0 00 0000 0000 0000 and 1 11 1111 1111 1111

#define BITPOL_ALT_OFF                          0x00
#define BITPOL_ALT_ON                           0x04

#define DATA_OUT_RAND_OFF                       0x00    
#define DATA_OUT_RAND_ON                        0x02    

#define OFFSET_BINARY_DATA_FORMAT               0x00    // Normal format
#define COMP2_DATA_FORMAT                       0x01    // 2's complement format

/***** DATA FORMAT REGISTER MODES *****/

#define DATA_NORMAL_FORMAT_MODE0                OFFSET_BINARY_DATA_FORMAT | BITPOL_ALT_OFF | DATA_OUT_RAND_OFF
#define DATA_NORMAL_FORMAT_MODE1                OFFSET_BINARY_DATA_FORMAT | BITPOL_ALT_OFF | DATA_OUT_RAND_ON
#define DATA_NORMAL_FORMAT_MODE2                OFFSET_BINARY_DATA_FORMAT | BITPOL_ALT_ON | DATA_OUT_RAND_OFF
#define DATA_NORMAL_FORMAT_MODE3                OFFSET_BINARY_DATA_FORMAT | BITPOL_ALT_ON | DATA_OUT_RAND_ON

#define DATA_COMP2_FORMAT_MODE0                 COMP2_DATA_FORMAT | BITPOL_ALT_OFF | DATA_OUT_RAND_OFF
#define DATA_COMP2_FORMAT_MODE1                 COMP2_DATA_FORMAT | BITPOL_ALT_OFF | DATA_OUT_RAND_ON
#define DATA_COMP2_FORMAT_MODE2                 COMP2_DATA_FORMAT | BITPOL_ALT_ON | DATA_OUT_RAND_OFF
#define DATA_COMP2_FORMAT_MODE3                 COMP2_DATA_FORMAT | BITPOL_ALT_ON | DATA_OUT_RAND_ON                        


/***** FUNCTIONS *****/

uint8_t ADCInitSPI();     // TODO: Parameters

uint8_t ADCWrite(uint8_t addr, uint8_t data);       // Write <data> in <addr> through SPI 
uint8_t ADCRead(uint8_t addr);                      // Read from <addr> through SPI 

void ADCReset();                                    // Reset all registers

void ADCPowerMode(uint8_t mode);                    // Config ADC power mode
void ADCTimingMode(uint8_t mode);                   // Config ADC timing mode
void ADCOutMode(uint8_t mode);                      // Config ADC output mode
void ADCDataFormatMode(uint8_t mode);               // Config ADC data format mode

void ADCSetOutputCurrent(uint8_t oc);               // Config ADC output current

uint8_t ADCTest(uint8_t mode);                      // Test ADC outputs



#endif // __LTC2145_14_H__