#pragma once


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sht4x_driver sht4x driver function
 * @brief    sht4x driver modules
 * @{
 */

/**
 * @addtogroup sht4x_base_driver
 * @{
 */

/**
 * @brief sht4x address enumeration definition
 */


typedef struct opt4_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t iic_addr;                                                          /**< iic device address */
    uint8_t inited;                                                            /**< inited flag */
} opt4_handle_t;



/**
 * @defgroup sht4x_link_driver sht4x link driver function
 * @brief    sht4x link driver modules
 * @ingroup  sht4x_driver
 * @{
 */

/**
 * @brief     initialize sht4x_handle_t structure
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] STRUCTURE sht4x_handle_t
 * @note      none
 */
#define DRIVER_SHT4X_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_IIC_INIT(HANDLE, FUC)              (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_IIC_DEINIT(HANDLE, FUC)            (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_IIC_READ_COMMAND(HANDLE, FUC)      (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)     (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_DELAY_MS(HANDLE, FUC)              (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an sht4x handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_SHT4X_LINK_DEBUG_PRINT(HANDLE, FUC)           (HANDLE)->debug_print = FUC


/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an sht4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t opt4_init(opt4_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an sht4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 soft reset failed
 * @note      none
 */
uint8_t opt4_deinit(opt4_handle_t *handle);


/**
 * @brief      get serial number
 * @param[in]  *handle pointer to an sht4x handle structure
 * @param[out] *num pointer to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t opt4_get_serial_number(opt4_handle_t *handle, uint16_t *serialNumber);

/**
 * @}
 */

/**
 * @defgroup sht4x_extend_driver sht4x extend driver function
 * @brief    sht4x extend driver modules
 * @ingroup  sht4x_driver
 * @{
 */

/**
 * @brief      write and read bytes
 * @param[in]  *handle pointer to an sht4x handle structure
 * @param[in]  cmd sent command
 * @param[in]  delay_ms delay in ms
 * @param[out] *data pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */

uint8_t opt4_getData(opt4_handle_t *handle, uint32_t *lux);
uint8_t setOneShot (opt4_handle_t *handle);

uint8_t opt4_write_read(opt4_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
