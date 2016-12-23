#include <string>
#include <iostream>
using namespace std;

struct StateInfo {
	float area;
	int num;
};
struct Area {
	float longitudeMin;
	float latitudeMin;
	float longitudeMax;
	float latitudeMax;
};
struct Coordinate {
	float longitude;
	float latitude;
};
struct StateBorder {
	StateInfo* info;
	Coordinate** coord;
	StateBorder(StateInfo* s = NULL) {
		info = s;
		if (info != NULL) {
			coord = new Coordinate*[info->num];
			for (int i = 0; i < info->num; i++)
				coord[i] = NULL;
		}
	}
};
struct State {
	StateInfo* info;
	StateBorder** border;
	Area* a;
	State(StateInfo* s = NULL, Area* r = NULL) {
		info = s;
		a = r;
		if (info != NULL) {
			border = new StateBorder*[info->num];
			for (int i = 0; i < info->num; i++)
				border[i] = NULL;
		}
	}
};
struct FileData {
	int latitudeMax, latitudeMin, longitudeMax, longitudeMin;
	string fName;
	FileData(int maxLat, int minLat, int maxLong, int minLong, string f) {
		latitudeMax = maxLat;
		latitudeMin = minLat;
		longitudeMax = maxLong;
		longitudeMin = minLong;
		fName = f;
	}
};
struct priorityQueue {
	vector<intersection*> vi;	
	priorityQueue() {
		for (int i = 0; i < vi.size(); i++)
			vi[i] = NULL;
	}
	void add(intersection* intersect) {	
		for (int i = 0; i < vi.size(); i++) {
			if (intersect->getShortest() < vi[i]->getShortest()) {
				vi.insert(vi.begin() + i, intersect);
				intersect->enq(i);
				refreshQueue();	
				return;
			}
		}
		vi.push_back(intersect);
		intersect->enq(vi.size() - 1);
	} 
	void refreshQueue() {
		for (int i = 0; i < vi.size(); i++)
			vi[i]->setPos(i);
	}
};