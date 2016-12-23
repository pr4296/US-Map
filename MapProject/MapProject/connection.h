#include <string>
using namespace std;

class connection {
protected:
	string road, roadType;
	double roadLength;
	int start, end;					
public:
	connection(string r, string t, int s, int e, double l) {
		road = r;
		roadType = t;
		start = s;
		end = e;
		roadLength = l;
	}
	string getRoad() { 
		return road; 
	}
	int getStart() { 
		return start; 
	}
	int getEnd() { 
		return end; 
	}
	double getLength() { 
		return roadLength; 
	}
};

