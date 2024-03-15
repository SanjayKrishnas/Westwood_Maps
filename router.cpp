#include "router.h"
#include "geodb.h"
#include <set>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db)
{
	geoData = &geo_db; //this is the address of geodatabase
}

Router::~Router()
{

}

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	//dijkstra
	std::unordered_map<std::string, double> distance; //geopoints to distances
	unordered_map<string, GeoPoint> previousPoint;
	unordered_map<string, bool> visited; //this will keep track of what Geopoints have been optimized
	priority_queue<pair<GeoPoint, double>, vector<pair<GeoPoint, double>>, Comparator> to_visit;
	
	previousPoint[pt1.to_string()] = pt1;
	distance[pt1.to_string()] = 0; //start point has distance 0
	to_visit.push(make_pair(pt1, 0)); //push start geo onto queue
	
	while(!to_visit.empty())
	{
		pair<GeoPoint, double> curPair = to_visit.top(); //get top pair of priority queue
		double curDistance = curPair.second; //get distance of current geopoint
		const GeoPoint& curPoint = curPair.first; //get geopoint of current pair
		to_visit.pop(); //pop pair out of queue
		
		visited[curPoint.to_string()] = true; //mark top geopoint as visited since it is now optimized

		if (curPoint.to_string() == pt2.to_string()) //this means the destination point is now optimized
		{
			break;
		}

		for (const auto& gp : geoData->get_connected_points(curPoint)) //for each connected point
		{
			if (visited.find(gp.to_string()) == visited.end()) //this means the value hasn't been visited
			{
				if (distance.find(gp.to_string()) == distance.end()) //if distance doesns't exist then set distance to infinity
				{
					distance[gp.to_string()] = 1000000;
				}

				double newDistance = distance_earth_miles(curPoint, gp);
				if (distance[gp.to_string()] > curDistance + newDistance) //if old distance is greater than new distance then change the distance
				{
					distance[gp.to_string()] = curDistance + newDistance;
					previousPoint[gp.to_string()] = curPoint; //set new previous point to cur point 
					to_visit.push(make_pair(gp, distance[gp.to_string()])); //push gp pair onto the queue
				}
			}
		}
	}

	//now construct the vector of geopoints in order from first to last
	vector<GeoPoint> completeRoute;
	
	if (previousPoint.find(pt2.to_string()) != previousPoint.end())
	{
		createPath(previousPoint, completeRoute, pt1, pt2);
	}
	
	return completeRoute;
}

void Router::createPath(const unordered_map<string, GeoPoint>& previousPoint, vector<GeoPoint>& completeRoute, const GeoPoint& gp1, const GeoPoint& gp2) const
{
	string endPoint = gp2.to_string();

	GeoPoint p = previousPoint.find(endPoint)->second;
	if (p.to_string() != gp1.to_string()) //this means that you haven't reached the start
	{
		createPath(previousPoint, completeRoute, gp1, p); //recursive call to find start
	} 
	else 
	{
		completeRoute.push_back(gp1);
	}
	completeRoute.push_back(gp2);

	return;
}