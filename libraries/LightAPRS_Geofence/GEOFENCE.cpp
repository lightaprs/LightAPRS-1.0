/*
 * GEOFENCE.c
 *
 * Created: 5.11.2016 22:04:58
 *  Author: Tomy2
 */ 

 
#include "GEOFENCE.h"


/*
	Adapted version of pointInPolygon() function from:	http://alienryderflex.com/polygon/
	
	Returns '0' if the point is outside of the polygon and '1' if it's inside.
	
	Expects input DEGREES * 100000 for latitude and longitude. Eg 4961070 for 49.61070 N.
	The reason is to make sure all calculations fit inside int32_t.
	
	However, this function is not very accurate due to rounding within the computation.
*/
int32_t pointInPolygon(int32_t polyCorners, int32_t * polygon, int32_t latitude, int32_t longitude)
{
	int32_t i;
	int32_t j = polyCorners * 2 - 2;
	int32_t oddNodes = 0;

	for(i = 0; i < polyCorners * 2; i += 2)
	{
		if((polygon[i + 1] < latitude && polygon[j + 1] >= latitude
			|| polygon[j + 1] < latitude && polygon[i + 1] >= latitude)
			&& (polygon[i] <= longitude || polygon[j] <= longitude))
		{
			oddNodes ^= (polygon[i] + (latitude - polygon[i + 1])
			/ (polygon[j + 1] - polygon[i + 1]) * (polygon[j] - polygon[i]) < longitude);
		}

		j = i;
	}

	return oddNodes;
}


/*
	Adapted version of pointInPolygon() function from:	http://alienryderflex.com/polygon/
	
	Returns '0' if the point is outside of the polygon and '1' if it's inside.
	
	Uses FLOAT input for better accuracy.
*/
int32_t pointInPolygonF(int32_t polyCorners, float * polygon, float latitude, float longitude)
{
	int32_t i;
	int32_t j = polyCorners * 2 - 2;
	int32_t oddNodes = 0;

	for(i = 0; i < polyCorners * 2; i += 2)
	{
		if((polygon[i + 1] < latitude && polygon[j + 1] >= latitude
		|| polygon[j + 1] < latitude && polygon[i + 1] >= latitude)
		&& (polygon[i] <= longitude || polygon[j] <= longitude))
		{
			oddNodes ^= (polygon[i] + (latitude - polygon[i + 1])
			/ (polygon[j + 1] - polygon[i + 1]) * (polygon[j] - polygon[i]) < longitude);
		}

		j = i;
	}

	return oddNodes;
}


/*
	Changes GEOFENCE_APRS_frequency and GEOFENCE_no_tx global variables based on the input coordinates.
	
	FREQUENCIES:
						Africa				144.800
						Europe				144.800
						Russia				144.800
						Canada				144.390
						Mexico				144.390
						USA					144.390
						Costa Rica			145.010
						Nicaragua			145.010
						Panama				145.010
						Venezuela			145.010
						Brazil				145.570
						Colombia			144.390
						Chile				144.390
						Argentina			144.930
						Paraguay			144.930
						Uruguay				144.930
						China				144.640
						Japan				144.660
						South Korea			144.620
						Thailand			145.525
						Australia			145.175
						New Zealand			144.575
						Indonesia			144.390
						Malaysia			144.390
		
	NO AIRBORNE APRS:											
						Latvia
						North Korea
						United Kingdom
						Yemen
	
	Expected input FLOAT for latitude and longitude as in GPS_UBX_latitude_Float and GPS_UBX_longitude_Float.
*/
void GEOFENCE_position(float latitude, float longitude)
{

	// SECTOR 1
	if(longitude > -38.0 && longitude < 73.0)
	{
		
		// S 1/2
		if(latitude > 0.0)
		{
			if(pointInPolygonF(9, UKF, latitude, longitude) == 1)				{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else if(pointInPolygonF(10, LatviaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else if(pointInPolygonF(5, YemenF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 1;GEOFENCE_APRS_frequency = 144800000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(9, BrazilF, latitude, longitude) == 1)			{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145570000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
	}
	
	// SECTOR 2
	else if(longitude <= -38.0)
	{
		
		// S 1/2
		if(latitude > 12.5)
		{
																				{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(8, ArgParUruF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144930000;}
			else if(pointInPolygonF(9, BrazilF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145570000;}
			else if(pointInPolygonF(7, VenezuelaF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145010000;}
			else if(pointInPolygonF(5, CostNicPanF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145010000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
	}
	
	// SECTOR 3
	else if(longitude >= 73.0)
	{
		
		// S 1/2
		if(latitude > 19.2)
		{	
			if(pointInPolygonF(6, North_KoreaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 1; GEOFENCE_APRS_frequency = 144620000;}	
			else if(pointInPolygonF(12, ChinaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144640000;}
			else if(pointInPolygonF(7, JapanF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144660000;}		
			else if(pointInPolygonF(5, South_KoreaF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144620000;}
			else if(pointInPolygonF(5, ThailandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145525000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
		}
		
		// S 2/2
		else
		{
			if(pointInPolygonF(6, AustraliaF, latitude, longitude) == 1)		{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145175000;}
			else if(pointInPolygonF(5, New_ZealandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144575000;}
			else if(pointInPolygonF(5, ThailandF, latitude, longitude) == 1)	{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 145525000;}
			else																{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144390000;}
		}
	}
	
	// shouldn't get here
	else
	{
																				{GEOFENCE_no_tx = 0; GEOFENCE_APRS_frequency = 144800000;}
	}
}


