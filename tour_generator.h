#include "base_classes.h"

class TourGenerator : public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
	virtual ~TourGenerator();
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

private:
	const GeoDatabaseBase* geoData;
	const RouterBase* routData;

	std::string getDirection(const GeoPoint& g1, const GeoPoint& g2) const;
	std::string getTurn(const GeoPoint& g1, const GeoPoint& g2, const GeoPoint& g3) const;
};

	