#ifndef __ENCODER_H__
#define __ENCODER_H__

void laps_and_distance_algorithm(void);
void reset(void);
void Yreset(void);
void disreset(void);
void disresetY(void);

extern float AD_Value[12];
#include "main.h"

typedef struct Positioning_system_TypeDef
{
	float X_laps;
	float X_laps_decimal;
	float X_distance_mm;
	float X_distance_cm;
	
	
	float Y_laps;
	float Y_laps_decimal;
	float Y_distance_mm;
	float Y_distance_cm;
	
}Positioning_system_TypeDef;

extern Positioning_system_TypeDef Positioning;
#endif 
