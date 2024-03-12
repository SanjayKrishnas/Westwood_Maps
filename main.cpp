#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include "HashMap.h"
#include <cassert>

#include "geodb.h"
//#include "router.h"
#include "stops.h"
#include "tourcmd.h"
//#include "tour_generator.h"

using namespace std;

//void print_tour(vector<TourCommand>& tcs)
//{
//    double total_dist = 0;
//    std::string direction;
//    double street_distance = 0;
//
//    cout << "Starting tour...\n";
//
//    for (size_t i = 0; i < tcs.size(); ++i)
//    {
//        if (tcs[i].get_command_type() == TourCommand::commentary)
//        {
//            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
//            cout << tcs[i].get_commentary() << "\n";
//        }
//        else if (tcs[i].get_command_type() == TourCommand::turn)
//        {
//            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
//        }
//        else if (tcs[i].get_command_type() == TourCommand::proceed)
//        {
//            total_dist += tcs[i].get_distance();
//            if (direction.empty())
//                direction = tcs[i].get_direction();
//            street_distance += tcs[i].get_distance();
//            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed
//                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
//            {
//                continue;
//            }
//
//            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
//            street_distance = 0;
//            direction.clear();
//        }
//    }
//
//    cout << "Your tour has finished!\n";
//    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
//}

//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        cout << "usage: BruinTour mapdata.txt stops.txt\n";
//        return 1;
//    }
//
//    GeoDatabase geodb;
//    if (!geodb.load(argv[1]))
//    {
//        cout << "Unable to load map data: " << argv[1] << endl;
//        return 1;
//    }
//
//    Router router(geodb);
//    TourGenerator tg(geodb, router);
//
//    Stops stops;
//    if (!stops.load(argv[2]))
//    {
//        cout << "Unable to load tour data: " << argv[2] << endl;
//        return 1;
//    }
//
//    std::cout << "Routing...\n\n";
//
//    vector<TourCommand> tcs = tg.generate_tour(stops);
//    if (tcs.empty())
//        cout << "Unable to generate tour!\n";
//    else
//        print_tour(tcs);
//}



int main() {
    // basic tests for HashMap
    {
        //test connections
        GeoDatabase g;
        g.load("mapdata.txt");

        string lat = "34.0419265";
        string long1 = "-118.5010322";
        GeoPoint p1 = GeoPoint(lat, long1);
        string p = p1.to_string();

        vector<GeoPoint> points = g.get_connected_points(p1);

        for (auto it : points)
        {
            cout << it.to_string() << endl; //should print geopoint
        }

        //test POI
        std::string poi = "Thalians Mental Health Center";
        GeoPoint point;
        if (g.get_poi_location(poi, point)) {
            std::cout << "Location of " << poi << ": " << point.to_string() << std::endl;
        }
        
        //test street
        GeoPoint g1 = GeoPoint("34.0753246", "-118.3816632");
        GeoPoint g2 = GeoPoint("34.0752704", "-118.3832229");
        string t = g.get_street_name(g2, g1);
        string q = g.get_street_name(g1, g2);
        cout << t << endl << q << endl;
        
        //HashMap<int> map;
        //map.insert("hi", 10);
        //assert(map["hi"] == 10);
        //map.insert("hi", 11);
        //assert(map["hi"] == 11);
        //// this tests general functions as well as rehashing
        //for (int i = 0; i < 50; i++) {
        //    map.insert(to_string(i), i);
        //}
        //for (int i = 0; i < 50; i++) {
        //    const int* p = map.find(to_string(i));
        //    assert(*p == i);
        //}
        //for (int i = 0; i < 50; i++) {
        //    assert(map[to_string(i)] == i);
        //}
        //// making sure the pointers for find work
        //for (int i = 0; i < 50; i++) {
        //    (*map.find(to_string(i)))++;
        //}
        //for (int i = 0; i < 50; i++) {
        //    assert(map[to_string(i)] == i + 1);
        //}
        //// make sure the default works
        //assert(map["50"] == 0);
        //map["51"]++;
        //assert(map["51"] == 1);
    }
    {
        // geodb sanity check
       /* GeoDatabase geodb;
        if (!geodb.load("mapdata.txt"))
        {
            cout << "Unable to load map data: " << endl;
            return 1;
        }
        GeoPoint a1("34.0709602", "-118.4425828");
        vector<GeoPoint> points = geodb.get_connected_points(a1);
        assert(points.size() == 1 && points[0].to_string() == "34.0709629,-118.4428913");
        GeoPoint a2("34.0709629", "-118.4428913");
        assert(geodb.get_street_name(a1, a2) == "Bruin Walk");
        GeoPoint point;
        assert(geodb.get_poi_location("Ackerman Union", point) && point.to_string() == "34.0705851,-118.4439429");
        assert(geodb.get_poi_location("Kerckhoff Hall", point) && point.to_string() == "34.0705849,-118.4437887");
        assert(geodb.get_poi_location("John Wooden Center", point) && point.to_string() == "34.0720093,-118.4458886");*/
    }
    cout << "All Tests Passed" << endl;
}
