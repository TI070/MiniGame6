#pragma once
#ifndef GAUGE_H
#define GAUGE_H

#define GAUGE_HEIGHT (20)
#define GAUGE_WIDTH (100)

void Gauge_Initialize(void);
void Gauge_Draw(float posx, float posy, float size_x, float size_y, float gauge_max, float current_value);
void Gauge_Draw(float posx, float posy, float gauge_max, float current_value);
void Gauge_Draw2(float posx, float posy, float gauge_max, float current_value,float chargemax,float charge_value);
#endif