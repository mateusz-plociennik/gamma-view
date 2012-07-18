/////////////////////////////////////////////////////////////////////////////
// Name:        data_types.h
// Purpose:     gives data types for gamma-view
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

struct gammaPoint 					// 64 bits
{
	bool isPoint			: 1; 	// 1 bit
	unsigned char reserved1	: 7; 	// 7 bits
	unsigned char reserved2	: 8; 	// 8 bits
	
	unsigned char x;				// 8 bits
	unsigned char y; 				// 8 bits
	float z;						// 32 bits
};

union gammaDataItem			// 8 bytes
{
	unsigned long long dt;	// 8 bytes
	gammaPoint gp;			// 8 bytes
};

struct gammaData
{
	int length;
	gammaDataItem *data_p;
};

#endif // _DATA_TYPES_H_