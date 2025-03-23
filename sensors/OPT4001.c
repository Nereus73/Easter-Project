#include "OPT4001.h"





#define OPT4_COMMAND_READ_SERIAL_NUMBER    0x11        /**< read serial number command */

/**
 * @brief      write and read bytes
 * @param[in]  *handle pointer to an sht4x handle structure
 * @param[in]  cmd sent command
 * @param[in]  delay delay in ms
 * @param[out] *data pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
static uint8_t a_opt4_write_read(opt4_handle_t *handle, uint8_t cmd, uint16_t delay, uint8_t *data, uint16_t len)
{
    if (handle->iic_write_cmd(handle->iic_addr, &cmd, 1) != 0)             /* write command */
    {
        return 1;                                                          /* return error */
    }
    if (delay != 0)                                                        /* if not 0 */
    {
        handle->delay_ms(delay);                                           /* delay */
    }
    if (len != 0)                                                          /* check length */
    {
        if (handle->iic_read_cmd(handle->iic_addr, data, len) != 0)        /* read data */
        {
            return 1;                                                      /* return error */
        }
    }
    
    return 0;                                                              /* success return 0 */
}

static uint8_t a_opt4_write_reg(opt4_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[len+1];
    buf[0] = reg;
    for (uint32_t i = 0; i < len; i++) {
        buf[i+1] = data[i];
    }
    if (handle->iic_write_cmd(handle->iic_addr, &buf, len+1) != 0)             /* write command */
    {
        return 1;                                                          /* return error */
    }
    return 0;                                                              /* success return 0 */
}

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
uint8_t opt4_init(opt4_handle_t *handle)
{
    uint8_t res;
    uint16_t serialNumber;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("OPT4001: iic_init is null.\n");                   /* iic_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("OPT4001: iic_deinit is null.\n");                 /* iic_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                        /* check iic_read_cmd */
    {
        handle->debug_print("OPT4001: iic_read_cmd is null.\n");               /* iic_read_cmd is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                       /* check iic_write_cmd */
    {
        handle->debug_print("OPT4001: iic_write_cmd is null.\n");              /* iic_write_cmd is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("OPT4001: delay_ms is null.\n");                   /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("OPT4001: iic init failed.\n");                    /* iic init failed */
       
        return 1;                                                            /* return error */
    }

    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t opt4_deinit(opt4_handle_t *handle)
{
    uint8_t res;
    
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->iic_deinit() != 0)                                    /* iic deinit */
    {
        handle->debug_print("OPT4001: iic deinit failed.\n");           /* iic deinit failed */
       
        return 1;                                                     /* return error */
    }
    handle->inited = 0;                                               /* flag close */
    
    return 0;                                                         /* success return 0 */
}

uint8_t opt4_getData(opt4_handle_t *handle, uint32_t *lux) {
    uint32_t mantissa;
    uint32_t adc_codes;
    uint8_t exponent;
    uint8_t counter;
    uint8_t crc;
    uint8_t buf[4];

    a_opt4_write_read(handle, 0x00, 10, buf, 4);    
    mantissa = ((buf[0] & 0x0F) << 16) | (buf[1] << 8) | buf[2];  
    exponent = buf[0] >> 4;
    counter = buf[3] >> 4;
    crc = buf[3] & 0x0F;
    adc_codes =  mantissa << exponent;
    *lux = adc_codes;
    return 0;
}

uint8_t setOneShot (opt4_handle_t *handle) {
    uint8_t res;
    uint8_t buf[2];
    res = a_opt4_write_read(handle, 0x0A, 10, buf, 2); 
    if (res != 0) {
        handle->debug_print("OPT4001: One Shot read failed.\n");
        return 1;
    }
    buf[1] &= 0xCF;
    buf[1] |= 0x20;
    res = a_opt4_write_reg(handle, 0x0A, buf, 2);
    if (res != 0) {
        handle->debug_print("OPT4001: One Shot write failed.\n");
        return 1;
    }
    return 0;
}

/**
 * @brief      get serial number
 * @param[in]  *handle pointer to an sht4x handle structure
 * @param[out] *num pointer to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t opt4_get_serial_number(opt4_handle_t *handle, uint16_t *serialNumber)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_opt4_write_read(handle, OPT4_COMMAND_READ_SERIAL_NUMBER, 
                             10, buf, 2);                                     /* read serial number */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("OPT4001: write command failed.\n");                /* write command failed */
        
        return 1;                                                             /* return error */
    }
    *serialNumber = (buf[1] << 8) | buf[0];
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t opt4_write_read(opt4_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    return a_opt4_write_read(handle, cmd, delay_ms, data, len);       /* write and read */
}


