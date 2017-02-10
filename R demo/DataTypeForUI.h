#pragma once
//enums here
typedef enum
{
	VERTICAL,
	HORIZONTAL
}ScrollBarStyle, Dimension;
#define Y VERTICAL
#define X HORIZONTAL

typedef enum
{
	PIXEL,
	DIALOG_UNIT
}UnitType;

typedef enum
{
	LEFT,
	RIGHT,
	CENTER,

	TOP_LEFT,
	TOP_RIGHT,
	TOP_CENTER,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_CENTER
}Align;