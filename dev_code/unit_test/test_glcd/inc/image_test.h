/*
 * Title : image_test.h
 * Copyright : HCL
 * Author : < hoang.thong >
 * Creation Date : 06/03/2023
 * ------- ---------- --------
 */

#ifndef _IMAGE_TEST_H_
#define _IMAGE_TEST_H_

#ifdef __cplusplus
extern "C"
{
#endif
    /* Include --------------------------------------------------------------------------------*/

    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

    /* Export Function Declarations -----------------------------------------------------------*/

    /* Export Variables -----------------------------------------------------------------------*/
    uint8_t acu8BitMap128x64[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x0f, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 0xff,
        0xff, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xff, 0xff,
        0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff,
        0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x01, 0xff, 0xff,
        0xff, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xff,
        0xff, 0xf8, 0x01, 0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0x80, 0x1f, 0xff,
        0xff, 0xf0, 0x03, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x1f, 0xc0, 0x0f, 0xff,
        0xff, 0xe0, 0x0f, 0xf8, 0x00, 0x00, 0x20, 0x00, 0x00, 0x04, 0x00, 0x00, 0x1f, 0xf0, 0x07, 0xff,
        0xff, 0x80, 0x3f, 0xf0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0f, 0xfc, 0x01, 0xff,
        0xff, 0x00, 0x7f, 0xf0, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x0f, 0xfe, 0x00, 0xff,
        0xfe, 0x00, 0xff, 0xf0, 0x1f, 0xfc, 0x00, 0x01, 0xc0, 0x00, 0x3f, 0xf8, 0x0f, 0xff, 0x00, 0x7f,
        0xfc, 0x01, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0x80, 0x3f,
        0xf8, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xc0, 0x1f,
        0xf0, 0x07, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xe0, 0x0f,
        0xf0, 0x0f, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xf0, 0x0f,
        0xe0, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xf8, 0x07,
        0xc0, 0x3f, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xfc, 0x03,
        0xc0, 0x3f, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xfc, 0x03,
        0x80, 0x7f, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xfe, 0x01,
        0x80, 0x7f, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xfe, 0x01,
        0x80, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0x01,
        0x00, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0x00,
        0x00, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0x00,
        0x00, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0x00,
        0x00, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0x00,
        0x00, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0x00,
        0x00, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0x00,
        0x00, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0x00,
        0x00, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0x00,
        0x80, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0x01,
        0x80, 0x7f, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x01, 0xff, 0xfe, 0x01,
        0x80, 0x7f, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xfe, 0x01,
        0xc0, 0x3f, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xfc, 0x03,
        0xc0, 0x3f, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xfc, 0x03,
        0xe0, 0x1f, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xf8, 0x07,
        0xf0, 0x0f, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xf0, 0x07,
        0xf0, 0x0f, 0xfb, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x0f,
        0xf8, 0x07, 0xf8, 0x0f, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x1f,
        0xfc, 0x03, 0xf8, 0x01, 0xff, 0xe0, 0x00, 0x3f, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x3f,
        0xfe, 0x00, 0xf8, 0x00, 0x7f, 0xfe, 0x00, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x7f,
        0xff, 0x00, 0x7f, 0x00, 0x3f, 0xfc, 0x01, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0xff,
        0xff, 0x80, 0x3f, 0xc0, 0x1f, 0xfc, 0x03, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xfc, 0x01, 0xff,
        0xff, 0xc0, 0x0f, 0xf0, 0x03, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf0, 0x03, 0xff,
        0xff, 0xf0, 0x07, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xe0, 0x0f, 0xff,
        0xff, 0xf8, 0x01, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0x80, 0x1f, 0xff,
        0xff, 0xfe, 0x00, 0x7f, 0x80, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xfe, 0x00, 0x7f, 0xff,
        0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xf0, 0x01, 0xff, 0xff,
        0xff, 0xff, 0xc0, 0x03, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xc0, 0x03, 0xff, 0xff,
        0xff, 0xff, 0xf8, 0x00, 0x7f, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xfe, 0x00, 0x1f, 0xff, 0xff,
        0xff, 0xff, 0xfe, 0x00, 0x0f, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x7f, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x80, 0x00, 0xf8, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0x00, 0x01, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    uint8_t acu8Logo32x32[] = {
        0xff, 0xf0, 0x0f, 0xff, 0xff, 0x80, 0x01, 0xff, 0xfe, 0x0f, 0xf0, 0x7f, 0xfc, 0x7f, 0xfe, 0x3f,
        0xf8, 0xff, 0xff, 0x1f, 0xf1, 0xff, 0xff, 0x8f, 0xe2, 0x1f, 0xf8, 0x47, 0xc6, 0x00, 0x00, 0x63,
        0xce, 0x60, 0x06, 0x73, 0x8e, 0x7f, 0xfe, 0x71, 0x9e, 0x7f, 0xfe, 0x79, 0x9e, 0x7f, 0xfe, 0x79,
        0x3c, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3d, 0xff, 0xff, 0xbc,
        0x3c, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3c, 0xff, 0xff, 0x3c, 0x3e, 0x7f, 0xfe, 0x7c,
        0x9e, 0x3f, 0xfc, 0x79, 0x9f, 0x07, 0xe0, 0xf9, 0x99, 0xc1, 0x83, 0xf9, 0xcc, 0xf3, 0xcf, 0xf3,
        0xc6, 0x67, 0xe7, 0xe3, 0xe2, 0x07, 0xe7, 0xc7, 0xf1, 0x87, 0xe7, 0x8f, 0xf8, 0xe7, 0xe7, 0x1f,
        0xfc, 0x27, 0xe4, 0x3f, 0xfe, 0x07, 0xe0, 0x7f, 0xff, 0x80, 0x01, 0xff, 0xff, 0xf0, 0x0f, 0xff};

    uint8_t acu8Logo64x64[] = {

        0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff,
        0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff,
        0xff, 0xff, 0x00, 0x3f, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xc0, 0x3f, 0xff,
        0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xfc, 0x0f, 0xff,
        0xff, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff,
        0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff,
        0xfe, 0x0e, 0x07, 0xff, 0xff, 0xe0, 0x70, 0x7f, 0xfc, 0x1e, 0x01, 0xff, 0xff, 0x80, 0x78, 0x3f,
        0xf8, 0x3c, 0x00, 0x40, 0x02, 0x00, 0x3c, 0x1f, 0xf8, 0x7c, 0x20, 0x00, 0x00, 0x04, 0x3e, 0x1f,
        0xf0, 0xfc, 0x38, 0x00, 0x00, 0x1c, 0x3f, 0x0f, 0xf0, 0xfc, 0x3e, 0x01, 0x80, 0x7c, 0x3f, 0x0f,
        0xe1, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0x87, 0xe1, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0x87,
        0xc3, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0xc3, 0xc3, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0xc3,
        0x87, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xe1, 0x87, 0xf8, 0x7f, 0xff, 0xff, 0xfe, 0x1f, 0xe1,
        0x87, 0xf0, 0x7f, 0xff, 0xff, 0xfe, 0x0f, 0xe1, 0x87, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe1,
        0x8f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf1, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0,
        0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0,
        0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0,
        0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0,
        0x0f, 0xf1, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xf0, 0x0f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf0,
        0x0f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf0, 0x0f, 0xf8, 0x7f, 0xff, 0xff, 0xfe, 0x1f, 0xf0,
        0x87, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xe1, 0x87, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xe1,
        0x87, 0xfc, 0x0f, 0xff, 0xff, 0xf0, 0x3f, 0xe1, 0x87, 0xfe, 0x03, 0xff, 0xff, 0xc0, 0x7f, 0xe1,
        0xc3, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xc3, 0xc3, 0xff, 0xc0, 0x07, 0xe0, 0x03, 0xff, 0xc3,
        0xe1, 0xc3, 0xe0, 0x07, 0xe0, 0x07, 0xff, 0x87, 0xe1, 0xc1, 0xf8, 0x07, 0xe0, 0x1f, 0xff, 0x87,
        0xe0, 0xe0, 0xfe, 0x0f, 0xf0, 0x7f, 0xff, 0x07, 0xf0, 0xf0, 0x7e, 0x1f, 0xf8, 0x7f, 0xff, 0x0f,
        0xf8, 0x78, 0x3e, 0x1f, 0xf8, 0x7f, 0xfe, 0x1f, 0xf8, 0x3c, 0x1c, 0x1f, 0xf8, 0x7f, 0xfc, 0x1f,
        0xfc, 0x1e, 0x00, 0x3f, 0xfc, 0x7f, 0xf8, 0x3f, 0xfe, 0x0e, 0x00, 0x3f, 0xfc, 0x7f, 0xf0, 0x7f,
        0xfe, 0x07, 0x80, 0x3f, 0xfc, 0x7f, 0xe0, 0x7f, 0xff, 0x03, 0xe0, 0x3f, 0xfc, 0x7f, 0xc0, 0xff,
        0xff, 0x81, 0xfe, 0x3f, 0xfc, 0x7f, 0x81, 0xff, 0xff, 0xc0, 0xfe, 0x3f, 0xfc, 0x7f, 0x03, 0xff,
        0xff, 0xe0, 0x3e, 0x3f, 0xfc, 0x7c, 0x07, 0xff, 0xff, 0xf8, 0x0e, 0x3f, 0xfc, 0x70, 0x1f, 0xff,
        0xff, 0xfc, 0x00, 0x3f, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xf8, 0x00, 0xff, 0xff,
        0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff,
        0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff};

    uint8_t acu8Logo64x32[] = {
        0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff,
        0xff, 0xfe, 0x00, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xff,
        0xff, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff,
        0xfc, 0x1e, 0x03, 0xff, 0xff, 0xc0, 0x78, 0x3f, 0xf8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1f,
        0xf0, 0xfc, 0x3c, 0x00, 0x00, 0x3c, 0x3f, 0x0f, 0xe1, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0x87,
        0xc3, 0xfc, 0x3f, 0xff, 0xff, 0xfc, 0x3f, 0xc3, 0x87, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xe1,
        0x87, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe1, 0x0f, 0xf1, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xf0,
        0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0,
        0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf0, 0x0f, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf0,
        0x0f, 0xf0, 0x7f, 0xff, 0xff, 0xfe, 0x0f, 0xf0, 0x87, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xe1,
        0x87, 0xfe, 0x07, 0xff, 0xff, 0xe0, 0x7f, 0xe1, 0xc3, 0xff, 0x80, 0x3f, 0xfc, 0x01, 0xff, 0xc3,
        0xe1, 0xc1, 0xf0, 0x07, 0xe0, 0x0f, 0xff, 0x87, 0xf0, 0xe0, 0x7e, 0x0f, 0xf0, 0x7f, 0xff, 0x0f,
        0xf8, 0x78, 0x3e, 0x1f, 0xf8, 0x7f, 0xfe, 0x1f, 0xfc, 0x1e, 0x00, 0x3f, 0xfc, 0x7f, 0xf8, 0x3f,
        0xff, 0x07, 0x80, 0x3f, 0xfc, 0x7f, 0xe0, 0xff, 0xff, 0xc0, 0xfe, 0x3f, 0xfc, 0x7f, 0x03, 0xff,
        0xff, 0xf0, 0x1e, 0x3f, 0xfc, 0x78, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xfc, 0x00, 0x7f, 0xff,
        0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff};

    uint8_t acu8Setting16x16[] = {
        0x01, 0x80, 0x03, 0xc0, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x7c, 0x3e, 0xfc, 0x3f,
        0xfc, 0x3f, 0x7c, 0x3e, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x03, 0xc0, 0x01, 0x80};

#ifdef __cplusplus
}
#endif
#endif