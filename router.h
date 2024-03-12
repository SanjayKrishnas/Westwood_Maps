#ifndef ROUTER
#define ROUTER

#include "base_classes.h"

class Router : public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db);
	virtual ~Router();
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;

private:
	//can use stl map set vector list queue etc
};

#endif