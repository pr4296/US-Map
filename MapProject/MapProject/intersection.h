#include <string>
#include <iostream>
#include <vector>
#include "connection.h"
using namespace std;

class intersection {
	protected:
	double longitude, latitude, distance, sDist;
	string State, city;
	bool qx, v;
	int pos;

	connection* prevcon;
	intersection* previnter;
	vector<connection*> connections;
	
public:
	intersection(double lon, double lat, double d, string s, string c) {
		sDist = 2147000000;
		prevcon = NULL;
		previnter = NULL;
		longitude = lon;
		latitude = lat;
		distance = d;
		State = s;
		city = c;
		qx = false;
		v = false;
	}
	void addConnection(connection* x) { 
		connections.push_back(x); 
	}
	
	vector<connection*> getConnections() { 
		return connections; 
	}

	double getDistance() { 
		return distance; 
	}
	string getCity() { 
		return city; 
	}
	string getState() { 
		return State; 
	}
	double getLongitude() { 
		return longitude; 
	}
	double getLatitude() { 
		return latitude; 
	}
	bool inqueue() { 
		return qx; 
	}
	void enq(int p) { 
		qx = true; 
		pos = p; 
	}
	void enq() { 
		qx = true; 
	}
	void unq() { 
		qx = false; 
		pos = -1; 
	}
	bool hasBeenVisited() { 
		return v; 
	}
	void visit() { 
		v = true; 
	}
	double getShortest() { 
		return sDist; 
	}
	void setShortest(double x) { 
		sDist = x; 
	}
	int getPos() { 
		return pos; 
	}
	void setPos(int p) { 
		pos = p; 
	}
	void setPrevious(connection* c, intersection* i) { 
		prevcon = c; 
		previnter = i; 
	}
	connection* getPreviousC() { 
		return prevcon; 
	}
	intersection* getPreviousI() { 
		return previnter; 
	}
};





