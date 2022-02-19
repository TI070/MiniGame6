#pragma once
#ifndef DEBUG_PRIMITIVE_H
#define DEBUG_PRIMITIVE_H

void DebugPrimitive_Initialize();
void DebugPrimitive_Finalize();
void DebugPrimitive_BatchBegin();
void DebugPrimitive_BatchDrawCircle(float x,float y,float radius);
void DebugPrimitive_BatchRun();

#endif
