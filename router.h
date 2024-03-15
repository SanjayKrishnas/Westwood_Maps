#ifndef ROUTER
#define ROUTER

#include "base_classes.h"
#include "HashMap.h"
#include "GeoPoint.h"
#include <unordered_map>
#include <vector>

class Router : public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db);
	virtual ~Router();
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;

private:
	//can use stl map set vector list queue etc
	const GeoDatabaseBase* geoData;
	void createPath(const std::unordered_map<std::string, GeoPoint>& previousPoints, std::vector<GeoPoint>& completeRoutes, const GeoPoint& gp1, const GeoPoint& gp2) const;
	
	struct Comparator 
	{
		bool operator()(const std::pair<GeoPoint, double>& g1, const std::pair<GeoPoint, double>& g2) const 
		{
			return g1.second > g2.second;
		}
	};
};

#endif