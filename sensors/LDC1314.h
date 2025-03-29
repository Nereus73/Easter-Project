#ifndef DRIVER_SHT4X_H
#define DRIVER_SHT4X_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#define LDC1314_REG_DATA                0x00
#define LDC1314_REG_RCOUNT              0x08
#define LDC1314_REG_OFFSET              0x0C
#define LDC1314_REG_SETTLECOUNT         0x10
#define LDC1314_REG_CLOCK_DIVIDERS      0x14
#define LDC1314_REG_STATUS              0x18
#define LDC1314_REG_ERROR_CONFIG        0x19
#define LDC1314_REG_CONFIG              0x1A
#define LDC1314_REG_MUX_CONFIG          0x1B
#define LDC1314_REG_RESET_DEV           0x1C
#define LDC1314_REG_DRIVE_CURRENT       0x1E
#define LDC1314_REG_MAN_ID              0x7E
#define LDC1314_REG_DEV_ID              0x7F

#define MCU_BIG_ENDIAN                  4321  // define MCU_BIG_ENDIAN in the other case
#define MCU_LITTLE_ENDIAN               1234  // define MCU_BIG_ENDIAN in the other case
#define MCU_BYTE_ORDER MCU_LITTLE_ENDIAN

typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t not_used_01          : 6;
  uint16_t high_current_drive   : 1;
  uint16_t intb_dis             : 1;
  uint16_t not_used_02          : 1;
  uint16_t ref_clk_src          : 1;
  uint16_t aut_amp_dis          : 1;
  uint16_t sensor_activate_sel  : 1;
  uint16_t rp_override_en       : 1;
  uint16_t sleep_mode_en        : 1;
  uint16_t active_chan          : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t active_chan          : 2;
  uint16_t sleep_mode_en        : 1;
  uint16_t rp_override_en       : 1;
  uint16_t sensor_activate_sel  : 1;
  uint16_t aut_amp_dis          : 1;
  uint16_t ref_clk_src          : 1;
  uint16_t not_used_02          : 1;
  uint16_t intb_dis             : 1;
  uint16_t high_current_drive   : 1;
  uint16_t not_used_01          : 6;
#endif /* DRV_BYTE_ORDER */
} ldc1314_config_reg_t;

typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t deglitch             : 3;
  uint16_t                      : 10;
  uint16_t rr_sequence          : 2;
  uint16_t autoscan_en          : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t autoscan_en          : 1;
  uint16_t rr_sequence          : 2;
  uint16_t                      : 10;
  uint16_t deglitch             : 3;
#endif /* DRV_BYTE_ORDER */
} ldc1314_mux_config_reg_t;


typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t                      : 9;
  uint16_t output_gain          : 2;
  uint16_t                      : 4;
  uint16_t reset_dev            : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t reset_dev            : 1;
  uint16_t                      : 4;
  uint16_t output_gain          : 2;
  uint16_t                      : 9;
#endif /* DRV_BYTE_ORDER */
} ldc1314_reset_dev_reg_t;

typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t                      : 6;
  uint16_t init_idrive          : 5;
  uint16_t idrive               : 5;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t idrive               : 5;
  uint16_t init_idrive          : 5;
  uint16_t                      : 6;
#endif /* DRV_BYTE_ORDER */
} ldc1314_drive_cur_reg_t;



typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t fref_divider         : 10;
  uint16_t                      : 2;
  uint16_t fin_divider          : 4;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t fin_divider          : 4;
  uint16_t                      : 2;
  uint16_t fref_divider         : 10;
#endif /* DRV_BYTE_ORDER */
} ldc1314_clk_dividers_reg_t;

typedef struct {
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
  uint16_t unreadconv3          : 1;
  uint16_t unreadconv2          : 1;
  uint16_t unreadconv1          : 1;
  uint16_t unreadconv0          : 1;
  uint16_t                      : 2;
  uint16_t drdy                 : 1;
  uint16_t                      : 1;
  uint16_t err_zc               : 1;
  uint16_t err_ale              : 1;
  uint16_t err_ahe              : 1;
  uint16_t err_wd               : 1;
  uint16_t err_or               : 1;
  uint16_t err_ur               : 1;
  uint16_t err_chan             : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
  uint16_t err_chan             : 2;
  uint16_t err_ur               : 1;
  uint16_t err_or               : 1;
  uint16_t err_wd               : 1;
  uint16_t err_ahe              : 1;
  uint16_t err_ale              : 1;
  uint16_t err_zc               : 1;
  uint16_t                      : 1;
  uint16_t drdy                 : 1;
  uint16_t                      : 2;
  uint16_t unreadconv0          : 1;
  uint16_t unreadconv1          : 1;
  uint16_t unreadconv2          : 1;
  uint16_t unreadconv3          : 1;
#endif /* DRV_BYTE_ORDER */
} ldc1314_status_reg_t;

/**
 * @brief sht4x handle structure definition
 */
typedef struct LDC1314_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t iic_addr;                                                          /**< iic device address */
    uint8_t inited;                                                            /**< inited flag */
} LDC1314_handle_t;


uint8_t LDC1314_enableSleepMode(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_enableAutoscan(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_enableRpOverride(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_disableAutoAmplitude(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_resetDevice(LDC1314_handle_t *handle);
uint8_t LDC1314_selectActiveChannel(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_setConversionTime(LDC1314_handle_t *handle, uint16_t val, uint8_t channel);
uint8_t LDC1314_setDataOffset(LDC1314_handle_t *handle, uint16_t val, uint8_t channel);
uint8_t LDC1314_setSettlingTime(LDC1314_handle_t *handle, uint16_t val, uint8_t channel);
uint8_t LDC1314_setDigitalGain(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_setDeglitch(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_setActivationMode(LDC1314_handle_t *handle, uint8_t val);
uint8_t LDC1314_setDriveCurrent(LDC1314_handle_t *handle, uint8_t val, uint8_t channel);
uint8_t LDC1314_setInputCLK(LDC1314_handle_t *handle, uint16_t val, uint8_t channel);
uint8_t LDC1314_setReferenceCLK(LDC1314_handle_t *handle, uint16_t val, uint8_t channel);
uint8_t LDC1314_getManID(LDC1314_handle_t *handle, uint16_t* val);
uint8_t LDC1314_getDevID(LDC1314_handle_t *handle, uint16_t* val);
uint8_t LDC1314_getSensorData(LDC1314_handle_t *handle, uint16_t* val, uint8_t channel);

static uint8_t ldc1314_read(LDC1314_handle_t *handle, uint8_t reg, uint16_t delay, uint8_t *data, uint16_t len);
static uint8_t ldc1314_write(LDC1314_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif