/**
  ******************************************************************************
  * @file    system.h
  * @author  Alson Tang
  * @version V1.0.0
  * @date    01-July-2024
  * @brief   This file contains all the functions prototypes for system
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  * Change History
  *  <Date>     | <Version> | <Author>       | <Description>
  *  2024/07/01 | 1.0.0     | Alson.Tang     | Create file
  ******************************************************************************
  */
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BIT(n)                                          (1u << n)

#define APPLICATION_NETWORK_RECV_OK                     BIT(0)

/*
 *********************************************************************************************************
 *                                         Application
 *********************************************************************************************************
 */
#define APPLICATION_NETWORK_QUEUE_SIZE                  (8)

typedef struct message {
    uint32_t recv_len;
    uint8_t buf_index;
} message_t;

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_H__ */
