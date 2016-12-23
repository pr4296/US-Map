#include <string>
#include <iostream>

using namespace std;

class place {
protected:
	int st, ce, population, rid;
	string state, city;
	double area, latitude, longitude, rdist;
public:
	place(string s, string t, int p, double a, double lat, double lon, int rr, double rd) {
		state = s;
		city = t;
		population = p;
		area = a;
		latitude = lat;
		longitude = lon;
		rid = rr;
		rdist = rd;
	}

	string toLower(string str) { //like this
		for (int i = 0; i<str.length(); i++) {
			if (str[i] >= 'A' && str[i] <= 'Z') 
				str[i] = str[i] + 32;
		}
		return str;				
	}

	string toUpper(string str) { //Like this
		if (str[0] >= 'a' && str[0] <= 'z') 
			str[0] = str[0] - 32;
		for (int i = 1; i<str.length(); i++) {
			if (str[i] >= 'A' && str[i] <= 'Z') 
				str[i] = str[i] + 32;
		}
		return str;			
	}

	string getState() { 
		return state; 
	}
	string getStateLower() { 
		return toLower(state); 
	}
	string getCityLower() { 
		return toLower(city); 
	}
	string getCityUpper() { 
		return city; 
	}
	int getRoad() { 
		return rid; 
	}
	double getLatitude() { 
		return latitude; 
	}
	double getLongitude() { 
		return longitude; 
	}
	void print() {
		cout << "-\t"<<city << ", " << state << ", (" << latitude << " N, " 
			<< longitude << " W) \n";
	}
};



