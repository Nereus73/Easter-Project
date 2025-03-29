#include "sensors/LDC1314.h"





 /**
  * @brief      enter or exit sleep mode depending on val
  * @param      handle to interface the low level topologie
  * @param      val to set sleep mode (0: no sleep. 1: sleep)
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_enableSleepMode(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.sleep_mode_en != val)) {
        reg.sleep_mode_en = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      enable autoscan mode over channels defined in rr_sequence
  * @param      handle to interface the low level topologie
  * @param      val to enable autoscan (0: disable. 1: enable)
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_enableAutoscan(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_mux_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_MUX_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.autoscan_en != val)) {
        reg.autoscan_en = val;
        rslt = ldc1314_write(handle,LDC1314_REG_MUX_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      enable user specific drive current depending on Rp
  * @param      handle to interface the low level topologie
  * @param      val to enable override (0: disable(auto). 1: enable(manual))
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_enableRpOverride(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.rp_override_en != val)) {
        reg.rp_override_en = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      disable automatic amplitude correction
  * @param      handle to interface the low level topologie
  * @param      val to disable override (0: enable(auto). 1: disable(manual))
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_disableAutoAmplitude(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.aut_amp_dis != val)) {
        reg.aut_amp_dis = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      soft reset the device
  * @param      handle to interface the low level topologie
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_resetDevice(LDC1314_handle_t *handle) {
    ldc1314_reset_dev_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_RESET_DEV,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if (rslt == 0) {
        reg.reset_dev = 1;
        rslt = ldc1314_write(handle,LDC1314_REG_RESET_DEV,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      select the channel to convert for no autoscan
  * @param      handle to interface the low level topologie
  * @param      val number of the channel select
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_selectActiveChannel(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.active_chan != val)) {
        reg.active_chan = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      set conversion time for specific channel, less time less accurancy
  * @param      handle to interface the low level topologie
  * @param      val 16bit value to calculate conversion time
  *             - (val*16+4)/fref , fref = 43.4 MHz without divider
  * @param      channel for which the conversion time should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  *             - 2 wrong input value
  */
uint8_t LDC1314_setConversionTime(LDC1314_handle_t *handle, uint16_t val, uint8_t channel) {
    if (channel > 3) {
        return 2;
    }
    uint16_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_RCOUNT+channel,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg != val)) {
        reg = val;
        rslt = ldc1314_write(handle,LDC1314_REG_RCOUNT+channel,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      set data offset for specific channel
  * @param      handle to interface the low level topologie
  * @param      val offset frquency to be used
  * @param      channel for which the data offset should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  *             - 2 wrong input value
  */
uint8_t LDC1314_setDataOffset(LDC1314_handle_t *handle, uint16_t val, uint8_t channel) {
    if (channel > 3) {
        return 2;
    }
    uint16_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_OFFSET+channel,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg != val)) {
        reg = val;
        rslt = ldc1314_write(handle,LDC1314_REG_OFFSET+channel,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      set settling time for specific channel
  * @param      handle to interface the low level topologie
  * @param      val register value corresponding to the settling time
  *             calculated: val > (Qsensor*fref)/(16*fsensor)
  *             Q = Rp*sqrt(C/L)
  * @param      channel for which the settling time should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  *             - 2 wrong input value
  */
uint8_t LDC1314_setSettlingTime(LDC1314_handle_t *handle, uint16_t val, uint8_t channel) {
    if (channel > 3) {
        return 2;
    }
    uint16_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_SETTLECOUNT+channel,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg != val)) {
        reg = val;
        rslt = ldc1314_write(handle,LDC1314_REG_SETTLECOUNT+channel,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      set digital signal gain to increase resolution or range
  * @param      handle to interface the low level topologie
  * @param      val value to define resolution/range
  *             - 0 resolution: 12bit, range 100%
  *             - 1 resolution: 14bit, range 25%
  *             - 2 resolution: 15bit, range 12.5%
  *             - 3 resolution: 16bit, range 6.25%
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_setDigitalGain(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_reset_dev_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_RESET_DEV,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.output_gain != val)) {
        reg.output_gain = val;
        rslt = ldc1314_write(handle,LDC1314_REG_RESET_DEV,(uint8_t *) &reg,2);
    }
    return rslt;
}

 /**
  * @brief      set input deglitch filter bandwidth
  * @param      handle to interface the low level topologie
  * @param      val value to define upper cut off frequency
  *             - 1 Frequency 1 MHz
  *             - 4 Frequency 3.3 MHz
  *             - 5 Frequency 10 MHz
  *             - 7 Frequency 33 MHz
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  *             - 2 wrong input value
  */
uint8_t LDC1314_setDeglitch(LDC1314_handle_t *handle, uint8_t val) {
    if ((val != 1) && (val != 4) && (val != 5) && (val != 7)) {
        return 2;
    }
    ldc1314_mux_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_MUX_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.deglitch != val)) {
        reg.deglitch = val;
        rslt = ldc1314_write(handle,LDC1314_REG_MUX_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

/**
  * @brief      set sensor activation mode, current consumption during activation
  * @param      handle to interface the low level topologie
  * @param      val value to define upper cut off frequency
  *             - 0 full current activation mode ~1.5mA
  *             - 1 low power activation - DRIVE_CURRENT used
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_setActivationMode(LDC1314_handle_t *handle, uint8_t val) {
    ldc1314_config_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CONFIG,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.sensor_activate_sel != val)) {
        reg.sensor_activate_sel = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CONFIG,(uint8_t *) &reg,2);
    }
    return rslt;
}

/**
  * @brief      set sensor drive current for given channel
  * @param      handle to interface the low level topologie
  * @param      val 5 bit drive current value, select according to Rp
  * @param      channel for which sensor drive current should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_setDriveCurrent(LDC1314_handle_t *handle, uint8_t val, uint8_t channel) {
    ldc1314_drive_cur_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_DRIVE_CURRENT,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.idrive != val)) {
        reg.idrive = val;
        rslt = ldc1314_write(handle,LDC1314_REG_DRIVE_CURRENT,(uint8_t *) &reg,2);
    }
    return rslt;
}

/**
  * @brief      set input clock for given channel
  * @param      handle to interface the low level topologie
  * @param      val 4 bit clock divider, fin = fclk/val
  * @param      channel for which the divider should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_setInputCLK(LDC1314_handle_t *handle, uint16_t val, uint8_t channel) {
    ldc1314_clk_dividers_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CLOCK_DIVIDERS,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.fin_divider != val)) {
        reg.fin_divider = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CLOCK_DIVIDERS,(uint8_t *) &reg,2);
    }
    return rslt;
}

/**
  * @brief      set reference clock for given channel
  * @param      handle to interface the low level topologie
  * @param      val 10 bit reference clock divider, fref = fclk/val
  * @param      channel for which the divider should be set
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_setReferenceCLK(LDC1314_handle_t *handle, uint16_t val, uint8_t channel) {
    ldc1314_clk_dividers_reg_t reg;
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_CLOCK_DIVIDERS,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if ((rslt == 0) && (reg.fref_divider != val)) {
        reg.fref_divider = val;
        rslt = ldc1314_write(handle,LDC1314_REG_CLOCK_DIVIDERS,(uint8_t *) &reg,2);
    }
    return rslt;
}

/**
  * @brief      get manufacturer ID 0x5449
  * @param      handle to interface the low level topologie
  * @param      val pointer to the variable to be written
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_getManID(LDC1314_handle_t *handle, uint16_t* val) {
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_MAN_ID,0,(uint8_t *) val,2);
    return rslt;
}

/**
  * @brief      get device ID 0x3054
  * @param      handle to interface the low level topologie
  * @param      val pointer to the variable to be written
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_getDevID(LDC1314_handle_t *handle, uint16_t* val) {
    uint8_t rslt = 0;
    rslt = ldc1314_read(handle,LDC1314_REG_DEV_ID,0,(uint8_t *) val,2);
    return rslt;
}

/**
  * @brief      get Sensor Data
  * @param      handle to interface the low level topologie
  * @param      val pointer to the variable to be written
  * @param      channel on which to scan for new data
  * @return     status code
  *             - 0 succes
  *             - 1 i2c communication failed
  */
uint8_t LDC1314_getSensorData(LDC1314_handle_t *handle, uint16_t* val, uint8_t channel) {
    ldc1314_status_reg_t reg;
    uint8_t rslt = 0;
    uint8_t dataChannel = LDC1314_REG_DATA+(channel << 1);
    rslt = ldc1314_read(handle,LDC1314_REG_STATUS,0,(uint8_t *) &reg,2);
    if (rslt != 0) {
        return 1;
    }
    if (reg.drdy) {
        if (channel < 4) {
            rslt = ldc1314_read(handle,dataChannel,0,(uint8_t *) &val,2);        
        }         
    }
    if (rslt != 0) {
        return 1;
    }
    *val &= 0x0FFF;
    return rslt;
}

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
static uint8_t ldc1314_read(LDC1314_handle_t *handle, uint8_t reg, uint16_t delay, uint8_t *data, uint16_t len)
{
    uint8_t temp[len];
    if (handle->iic_write_cmd(handle->iic_addr, &reg, 1) != 0)             /* write command */
    {
        return 1;                                                          /* return error */
    }
    if (delay != 0)                                                        /* if not 0 */
    {
        handle->delay_ms(delay);                                           /* delay */
    }
    
    if (len != 0)                                                          /* check length */
    {
        if (handle->iic_read_cmd(handle->iic_addr, temp, len) != 0)        /* read data */
        {
            return 1;                                                      /* return error */
        }
    #if MCU_BYTE_ORDER == MCU_LITTLE_ENDIAN
    for (uint16_t i = 0; i < len; i++) {
        data[i] = temp[len-i-1];
    }
    #elif MCU_BYTE_ORDER == MCU_BIG_ENDIAN
    for (uint16_t i = 0; i < len; i++) {
        data[i] = temp[i];
    }
    #endif
    }
    
    return 0;                                                              /* success return 0 */
}

static uint8_t ldc1314_write(LDC1314_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint8_t temp[len+1];
    temp[0] = reg;
    #if MCU_BYTE_ORDER == MCU_LITTLE_ENDIAN
    for (uint16_t i = 0; i < len; i++) {
        temp[i+1] = data[len-i-1];
    }
    #elif MCU_BYTE_ORDER == MCU_BIG_ENDIAN
    for (uint16_t i = 0; i < len; i++) {
        temp[i+1] = data[i];
    }
    #endif
    if (handle->iic_write_cmd(handle->iic_addr, temp, len+1) != 0)             /* write command */
    {
        return 1;                                                          /* return error */
    }   
    return 0;                                                              /* success return 0 */
}