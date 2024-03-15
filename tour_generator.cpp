#include "tour_generator.h"
#include "stops.h"
#include "tourcmd.h"
#include <vector>
#include "geotools.h"
#include "base_classes.h"
#include <string>
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
{
	//get pointers to router and geodatabase
	geoData = &geodb;
	routData = &router;
}

TourGenerator::~TourGenerator()
{
	
}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
	vector<TourCommand> commands;

	//create 2 geopoints and 
	string currPOI;
	string nextPOI;
	string talking_points;

	//current and next POI
	GeoPoint point1;
	GeoPoint point2;
	
	//get names of both streets along 3 geopoints
	string Street1;
	string Street2;

	for (int k = 0; k < stops.size() - 1; k++)
	{
		TourCommand commentary;
		stops.get_poi_data(k, currPOI, talking_points);
		
		//push current POI commentary
		commentary.init_commentary(currPOI, talking_points);
		commands.push_back(commentary);

		stops.get_poi_data(k + 1, nextPOI, talking_points);

		//Find current and next geopoints
		geoData->get_poi_location(currPOI, point1);
		geoData->get_poi_location(nextPOI, point2);

		vector<GeoPoint> path = routData->route(point1, point2); //generate a route between both geopoints

		for (int i = 0; i < path.size() - 1; i++)
		{
			GeoPoint& start = path[i];
			GeoPoint& middle = path[i + 1];

			Street1 = geoData->get_street_name(start, middle);

			//always proceed along the first street
			TourCommand proceed;
			proceed.init_proceed(getDirection(start, middle), Street1, distance_earth_miles(start, middle), start, middle);
			commands.push_back(proceed);

			if(i < path.size() - 2) //this means you might have to make a turn
			{
				GeoPoint& end = path[i + 2];
				string Street2 = geoData->get_street_name(middle, end);

				if (Street1 != Street2) //this means you need to make a turn 
				{
					TourCommand turn;
					string toTurn = getTurn(start, middle, end); //get the value left right or error
					if (toTurn != "ERROR")
					{
						turn.init_turn(getTurn(start, middle, end), Street2); //turn onto street 2
						commands.push_back(turn);
					}
				}
			}
		}
	}

	//push back last POI command
	TourCommand commentary;
	string lastPOI;
	stops.get_poi_data(stops.size() - 1, lastPOI, talking_points);

	//push current POI commentary
	commentary.init_commentary(lastPOI, talking_points);
	commands.push_back(commentary);

	return commands;
}

string TourGenerator::getDirection(const GeoPoint& g1, const GeoPoint& g2) const
{
	double lineAngle = angle_of_line(g1, g2); //gives the direction of travel as a double
	if (lineAngle >= 0 && lineAngle < 22.5)
	{
		return "east";
	}
	else if (lineAngle >= 22.5 && lineAngle < 67.5)
	{
		return "northeast";
	}
	else if (lineAngle >= 67.5 && lineAngle < 112.5)
	{
		return "north";
	}
	else if (lineAngle >= 112.5 && lineAngle < 157.5)
	{
		return "northwest";
	}
	else if (lineAngle >= 157.5 && lineAngle < 202.5)
	{
		return "west";
	}
	else if (lineAngle >= 202.5 && lineAngle < 247.5)
	{
		return "southwest";
	}
	else if (lineAngle >= 247.5 && lineAngle < 292.5)
	{
		return "south";
	}
	else if (lineAngle >= 292.5 && lineAngle < 337.5)
	{
		return "southeast";
	}
	else if (lineAngle >= 337.5)
	{
		return "east";
	}
	//should never get to this point
	return "ERROR";
}

string TourGenerator::getTurn(const GeoPoint& g1, const GeoPoint& g2, const GeoPoint& g3) const
{
	double turnAngle = angle_of_turn(g1, g2, g3);

	if (turnAngle >= 1 && turnAngle < 180)
	{
		return "left";
	}
	else if (turnAngle >= 181 && turnAngle <= 359)
	{
		return "right";
	}

	//if it gets to this point that means angle is less than 1 or greater than 359
	//Should never get to this point
	return "ERROR";
}