/*
 * Filename: HR_algorithm.c
 * Path: 
 * Created Date: Thursday, January 31st 2019, 10:05:21 am
 * Author: ldj
 * 
 * Copyright (c) 2019 
 */

#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "HR_algorithm.h"
#include "test_data.h"

#define SAMPLE_PERIOD 10 //ms
#define WSIZE 43 //windows size
#define W_CENTRAL_INDEX (WSIZE / 2 + 1)
#define W_CENTRAL_LEFT_POINT_NUM (WSIZE / 2)
#define TOTAL_PEAK_MAX_NUM ((((SRC_DATA_LEN * 10)/1000) * 3) + 1) //most 3 peak per second 

extern int16_t src_data[SRC_DATA_LEN];

typedef struct
{
    int16_t max_v;
    int16_t padding;
    uint32_t peak_point_time;
} peak_data_t;

typedef struct
{
    uint32_t peak_data_buff_index;
    peak_data_t peak_data[TOTAL_PEAK_MAX_NUM]; //most 3 peak per second 
} peak_data_buff_t;

int16_t find_fifo[WSIZE * 1] = {0};
peak_data_buff_t data_peak_buff;

static peak_data_buff_init(void) 
{
    uint32_t i;

    data_peak_buff.peak_data_buff_index = 0x0; //invalid index
    for (i = 0; i < TOTAL_PEAK_MAX_NUM; i++)
    {
        data_peak_buff.peak_data[i].max_v = 0;
        data_peak_buff.peak_data[i].peak_point_time = 0;
    }
}

static uint32_t find_maxValue_index(const int16_t *src_data,
                                    uint32_t src_data_len,
                                    int16_t *p_max_v,
                                    uint32_t *p_max_index)
{
    int16_t max_data_value = 0;
    uint32_t i,index;

    max_data_value = src_data[0];
    for (i = 0; i < src_data_len; i++)
    {
        if (src_data[i] > max_data_value)
        {
            max_data_value = src_data[i];
            index = i;
        }
    }

    *p_max_v = max_data_value;
    *p_max_index = index;
}

static void find_peak_index(void)
{
    uint32_t i,j;
    int16_t max_v = 0;
    uint32_t max_v_index = 0;

    for (i = 1; i < SRC_DATA_LEN - WSIZE; i++)
    {
        memcpy(find_fifo, &src_data[i], WSIZE * 2); //for in16_t src data

        find_maxValue_index(find_fifo, WSIZE, &max_v, &max_v_index);
        if (W_CENTRAL_INDEX == max_v_index)
        {
            data_peak_buff.peak_data[data_peak_buff.peak_data_buff_index].max_v = max_v;
            data_peak_buff.peak_data[data_peak_buff.peak_data_buff_index].peak_point_time = i + W_CENTRAL_LEFT_POINT_NUM;
            data_peak_buff.peak_data_buff_index++;
        }
    }

    if (data_peak_buff.peak_data_buff_index)
        data_peak_buff.peak_data_buff_index--;
}

static uint32_t get_hr_period(void)
{
    uint32_t hr_period = 0; //between first peak to last peak
    uint32_t i;

    hr_log_debug("data_peak_buff.peak_data_buff_index:%d\n",data_peak_buff.peak_data_buff_index);
    if (0 == data_peak_buff.peak_data_buff_index)
    {
        return 0; 
    }
    hr_period = data_peak_buff.peak_data[data_peak_buff.peak_data_buff_index].peak_point_time - data_peak_buff.peak_data[0].peak_point_time;

    return (hr_period / data_peak_buff.peak_data_buff_index);
}


void HR_algorithm_test(void)
{
    uint32_t i;
    uint32_t hart_rate = 0;

    hr_log_debug("HR_algorithm_test\n");

    peak_data_buff_init();
    find_peak_index();
    //get hart rate use period
    hart_rate = (60 * 1000) / (10 * get_hr_period());
    hr_log_debug("hart rate :%d BPM\n",hart_rate);
}
