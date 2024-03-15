#include "geodb.h"
#include "HashMap.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "geotools.h"
#include "geopoint.h"

//TO DO go through include statements
//TO DO make all auto auto& (&&&)

using namespace std;

GeoDatabase::GeoDatabase()
{
	//empty?
}

GeoDatabase::~GeoDatabase()
{

}

bool GeoDatabase::load(const std::string& map_data_file)
{
	std::ifstream infile(map_data_file);
	if (!infile)
	{
		return false; //this means the load failed so return false
	}

	std::string street;
	while (getline(infile, street)) //while you can still input new lines of text
	{
		std::string lat1;
		std::string long1;
		std::string lat2;
		std::string long2;

		infile >> lat1 >> long1 >> lat2 >> long2; //read in the lat and long values
		infile.ignore(10000, '\n');

		GeoPoint geo1(lat1, long1);
		GeoPoint geo2(lat2, long2);

		const std::string geo1Name = geo1.to_string();
		const std::string geo2Name = geo2.to_string();

		//add both Geo Points to their respective vectors
		m_connections[geo1Name].push_back(geo2);
		m_connections[geo2Name].push_back(geo1);

		//Add street connection
		m_street[geo1Name + geo2Name] = street;

		//check to see if there are POI near the geo points
		int POI;
		infile >> POI;
		infile.ignore(10000, '\n');

		if (POI > 0)
		{
			GeoPoint geoMid = midpoint(geo1, geo2);

			// add connection for old geo points and midpoints
			m_connections[geo1Name].push_back(geoMid);
			m_connections[geo2Name].push_back(geoMid);

			//add connection between midpoint and all other points 
			string geoMidName = geoMid.to_string();
			m_connections[geoMidName].push_back(geo1);
			m_connections[geoMidName].push_back(geo2);

			//Add street names to midpoint and old points
			m_street[geo1Name + geoMidName] = street;
			m_street[geo2Name + geoMidName] = street;

			for (POI; POI > 0; POI--) //for every POI add a connection between previous points and midpoint as well as midpoint and POI
			{
				//get name of the POI
				string linePOI;
				getline(infile, linePOI);
				size_t index = linePOI.find('|');
				string location = linePOI.substr(0, index); //location of the POI

				//get later half of the line
				string coordPOI = linePOI.substr(index + 1); //gets second half of string
				size_t space = coordPOI.find(' '); 

				//get lat and long of geo point
				string latPOI = coordPOI.substr(0, space);
				string longPOI = coordPOI.substr(space + 1);
				
				GeoPoint geoPOI(latPOI, longPOI);
				string stringPOI = geoPOI.to_string();

				//add connection between midpoint and POI
				m_connections[geoMidName].push_back(geoPOI);
				m_connections[stringPOI].push_back(geoMid);
				
				//add POI into POI hashmap with location
				m_pointsOfInterest[location] = geoPOI; 

				//add street connections between midpoint and POI
				m_street[geoMidName + stringPOI] = "a path";
			}
		}

	}

	return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	const GeoPoint* POI = m_pointsOfInterest.find(poi); //find point of interest

	if (POI == nullptr) //this means the POI isn't found so return immediately
	{
		//make no change to second parameter
		return false;
	}
	else //POI was found
	{
		point = *POI;
		return true;
	}
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
	vector<GeoPoint> connectedPoints;

	const vector<GeoPoint>* points = m_connections.find(pt.to_string());
	if (points == nullptr)
	{
		return connectedPoints; //empty vector
	}
	else
	{
		return (*points); //this has the vector containing all connected geopoints
	}

}


std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	//try first string combo
	string combo = pt1.to_string() + pt2.to_string();
	
	const string* streetName = m_street.find(combo);
	if (streetName != nullptr)
	{
		return *streetName;
	}

	//if first combo fails then try second combo
	string reverseCombo = pt2.to_string() + pt1.to_string();
	streetName = m_street.find(reverseCombo);

	if (streetName != nullptr)
	{
		return *streetName;
	}

	//if both fail then return empty string 
	return "";
}
////////////////////////////////////////////////////////////////////////////////////////