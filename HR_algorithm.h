/*
 * Filename: HR_algorithm.h
 * Path: 
 * Created Date: Thursday, January 31st 2019, 10:06:29 am
 * Author: ldj
 * 
 * Copyright (c) 2019 
 */

#ifndef __HR_ALGORITHM_H__
#define __HR_ALGORITHM_H__

#define HR_DEBUG 1

#if HR_DEBUG
#define hr_log_debug printf
#else
#define hr_log_debug(...) /**< Disables detailed logs. */
#endif

void HR_algorithm_test(void);

#endif
