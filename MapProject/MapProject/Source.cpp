#include "../MapProject/library.h"
#include "intersection.h"
#include "LinkedList.h"
#include "MapObjects.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <ctime>
using namespace std;

//converts str to lowercase
string toLower(string str) {
	for (int i = 0; i<str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
	}
	return str;
}

//converts str to uppercase
string toUpper(string str) {
	if (str[0] >= 'a' && str[0] <= 'z')
		str[0] = str[0] - 32;
	for (int i = 1; i<str.length(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] + 32;
	}
	return str;
}

//trims the spaces before and after string s
string trim(string s) { 
	char *str = new char[s.length() + 1];		             
	strcpy(str, s.c_str());  	    
	int i, len; 	 	    
	len = strlen(str); 	    
	for (i = 0; i<len; i++) { 
		if (str[i] != ' ') break; 
	} 	    
	memmove(str, str + i, len - i + 1); 	 	    
	len = strlen(str); 	    
	for (i = len - 1; i >= 0; i--) { 
		if (str[i] != ' ') break; 
	} 	    
	str[i + 1] = 0; 	    
	string toReturn(str); 	    
	return toReturn; 
}

//hash function
int Hash(string str) {
	int h = 1357593;
	int mult = 1257;
	for (int i = 0; i < str.length(); i++)
		h = str[i] + mult*h;
	if (h < 0)
		h = -h;
	if (h < 0)
		h = 0;
	h = h % 1000;
	return h;
}

//this function reads the named-places.txt file and stores it into the hashtable
void readFileNamedPlaces(string fName, List** &hashtable) {
	ifstream fin(fName.c_str());
	if (fin.fail()) {
		cout << "Error opening "<<fName<<".\n";
		exit(1);
	}

	int hashval;
	place* temp = NULL;
	string line, s_numcode, s_abbrev, s_name, s_dist, s_repcode,
		s_longitude, s_latitude, s_area, s_population;
	int i_numcode, i_population, i_repcode, length;
	double d_area, d_latitude, d_longitude, d_dist;
	while (!fin.eof()) {
		line = "";
		getline(fin, line);
		if (fin.fail()) break;
		
		s_numcode = trim(line.substr(0, 8));
		s_abbrev = trim(line.substr(8, 2));
		s_name = trim(line.substr(10, 50));
		length = line.length();
		s_dist = trim(line.substr(length - 8, 8));
		s_repcode = trim(line.substr(length - 13, 5));
		s_longitude = trim(line.substr(length - 24, 11));
		s_latitude = trim(line.substr(length - 34, 10));
		s_area = trim(line.substr(length - 48, 14));
		s_population = trim(line.substr(length - 55, 7));
		i_numcode = atoi(s_numcode.c_str());
		i_population = atoi(s_population.c_str());
		i_repcode = atoi(s_repcode.c_str());
		d_area = atof(s_area.c_str());
		d_latitude = atof(s_latitude.c_str());
		d_longitude = atof(s_longitude.c_str());
		d_dist = atof(s_dist.c_str());
		temp = new place(s_abbrev, s_name, i_population, d_area, d_latitude, d_longitude, i_repcode, d_dist);
		hashval = Hash(temp->getCityLower());	
		if (hashtable[hashval]->getFirst() != NULL)	
			hashtable[hashval]->addEnd(temp);
		else 
			hashtable[hashval]->addFront(temp);
	}
	fin.close();
}

//this function reads the intersections.txt file
void readFileIntersections(string fName, vector<intersection*> &vi) {
	ifstream fin(fName.c_str());
	if (fin.fail()) {
		cout << "Error opening " << fName << ".\n";
		exit(1);
	}
	string longitude, latitude, length, state, city, input;
	longitude = ""; latitude = ""; length = ""; state = ""; city = ""; input = "";
	while (!fin.eof()) {
		getline(fin, input);
		if (fin.fail()) break;
		istringstream parse(input);
		parse >> longitude >> latitude >> length >> state;
		city = input.substr(30, input.length());
		vi.push_back(new intersection(atof(longitude.c_str()), atof(latitude.c_str()), atof(length.c_str()), state, city));
	}
	fin.close();
}

//this function reads the connections.txt file
void readFileConnections(string fName, vector<connection*> &vc, vector<intersection*> &vi) {
	ifstream fin(fName.c_str());
	if (fin.fail()) {
		cout << "Error opening " << fName << ".\n";
		exit(1);
	}
	string road, roadType, placeFrom, placeTo, length, input;
	connection* c = NULL;
	int to, from;
	
	while (!fin.eof()) {
		getline(fin, input);
		if (fin.fail()) 
			break;
		istringstream parse(input);
		parse >> road >> roadType >> placeFrom >> placeTo >> length;
		c = new connection(road, roadType, atoi(placeFrom.c_str()), atoi(placeTo.c_str()), atof(length.c_str()));
		vc.push_back(c);
		to = atoi(placeTo.c_str());
		from = atoi(placeFrom.c_str());
		vi[from]->addConnection(c);
		vi[to]->addConnection(c);
	}
	fin.close();
}

//this function finds the city given a cityname and further specified state information
Link* findCity(List** hashTable, string cityName) {
	string cityLower = toLower(cityName);
	int key = Hash(cityLower);
	int incr;
	Link * currLink;
	string state;

	if (hashTable[key]->getFirst() == NULL) {
		cout << "Could not find "<<cityLower<<".\n";
		return NULL;
	}

	if (hashTable[key]->getFirst()->data != NULL) {
		if ((hashTable[key]->getFirst()->next == NULL) && 
			(hashTable[key]->getFirst()->data->getCityLower() == cityLower)) 
			return hashTable[key]->getFirst();
		else if (hashTable[key]->getFirst()->next != NULL) {
			currLink = hashTable[key]->getFirst();
			incr = 0;
			if (currLink->data->getCityLower() == cityLower) 
				incr++;
			while (currLink->next != NULL) {
				currLink = currLink->next;
				if ((currLink->data != NULL) && 
					(currLink->data->getCityLower() == cityLower)) 
					incr++;
			}
			currLink = hashTable[key]->getFirst();

			//if multiple states were found with the same city name
			if (incr > 1) {
				cout << cityName << " was found in the following states: ";
				while (currLink != NULL) {	
					if (currLink->data->getCityLower() == cityLower) 
						cout << currLink->data->getState() << ", ";
					currLink = currLink->next;
				}
				cout << "Enter state: ";
				getline(cin, state);
				state = toLower(state);	

				currLink = hashTable[key]->getFirst();
				if (currLink->data->getStateLower() == state) 
					return currLink;
				while (currLink->next != NULL) {
					currLink = currLink->next;
					if (currLink->data != NULL && 
						currLink->data->getStateLower() == state && 
						currLink->data->getCityLower() == cityLower) 
						return currLink;
				}
				cout << "Error reading input.\n";
				return NULL;
			}
			else if (incr == 1) {
				if (currLink->data->getCityLower() == cityLower) 
					return currLink;
				while (currLink->next != NULL) {
					currLink = currLink->next;
					if (currLink->data != NULL && 
						currLink->data->getCityLower() == cityLower) 
						return currLink;
				}
				cout << "Error reading input.\n";
				return NULL;
			}
			else {
				cout << "Error finding city.\n";
				return NULL;
			}
		}
		cout << "Error finding city.\n";
		return NULL;
	}
	else {
		cout << "Error finding city.\n";
		return NULL;
	}
}

//finds the shortest path between a start and end intersection
vector<intersection*> shortestPath(intersection * &start, intersection * &end, vector<intersection*> vi, vector<connection*> vc) {

	vector<intersection*> inters;
	intersection* intersectionCurr = NULL;
	intersection* intersectionNext = NULL;
	connection* connectionCurr = NULL;
	int curr, istart, iend;
	
	if (start == end) {
		return inters;
	}

	priorityQueue* pq = new priorityQueue();
	pq->vi.push_back(start);
	start->setShortest(0);

	while (true) {
		if (pq->vi.empty())
			break;
		intersectionCurr = pq->vi.front();
		curr = intersectionCurr->getPos();
		pq->vi.erase(pq->vi.begin());
		pq->refreshQueue();
		intersectionCurr->visit();

		for (int i = 0; i < intersectionCurr->getConnections().size(); i++) {	
			connectionCurr = intersectionCurr->getConnections()[i];
			if (vi[connectionCurr->getStart()] == intersectionCurr) {
				istart = connectionCurr->getStart();
				iend = connectionCurr->getEnd();
			}
			else {
				istart = connectionCurr->getEnd();
				iend = connectionCurr->getStart();
			}						
			intersectionNext = vi[iend];				

			if (intersectionNext->hasBeenVisited())				
				continue; //go to end of the loop (skip over)
			if (intersectionNext == end) {
				double val = intersectionCurr->getShortest() + connectionCurr->getLength();
				intersectionNext->setShortest(val);
				intersectionNext->setPrevious(connectionCurr, vi[istart]);
				intersectionNext->visit();
				vector<intersection*> path;	
				intersection* curr = end;
				while (curr != NULL) {
					path.push_back(curr);
					curr = curr->getPreviousI();
				}
				return path;
			}
			if (intersectionNext->inqueue()) {	
				if (intersectionCurr->getShortest() + connectionCurr->getLength() < intersectionNext->getShortest()) {
					intersectionNext->setShortest(intersectionCurr->getShortest() + connectionCurr->getLength());
					intersectionNext->setPrevious(connectionCurr, vi[istart]);
					pq->refreshQueue();
					pq->add(intersectionNext);
				}
				continue; //go to end of the loop
			}
			intersectionNext->setShortest(intersectionCurr->getShortest() + connectionCurr->getLength());
			intersectionNext->setPrevious(connectionCurr, vi[istart]);
			pq->add(intersectionNext);
		}
		intersectionCurr->unq();
	}
	return inters;
}

//draw the map
void loadMap(string fName, double* xVal, double* yVal, double* secondsPerPixel) {
	FILE* file = fopen(fName.c_str(), "rb");
	if (file == NULL) {
		cout << "Error opening "<<fName<<" \n";
		return;
	}

	ifstream fin(fName.c_str());
	if (fin.fail()) {
		cout << "Error opening " << fName << " \n";
		return;
	}
	int numRows, numCols, bpp, spp, longS, latS, min, max, val;
	string garbage;	

	fin >> garbage >> numRows >> garbage >> numCols >> garbage >> bpp >> garbage >> spp;
	fin >> garbage >> longS >> garbage >> latS >> garbage >> min >> garbage >> max >> garbage >> val;
	int rowSize = numCols * sizeof(short int);
	short int *temp = (short int*) malloc(2*(numRows + 1)*numCols);
	int num = fread(temp, rowSize, numRows + 1, file);
	make_window(numCols-1, numRows-1);
	set_pen_color_int(37, 86, 138);
	fill_rectangle(0, 0, numCols, numRows);
	set_pen_width(2);

	for (int r = 1; r<numRows + 1; r++) {
		for (int c = 0; c<numCols; c++) {
			move_to(c, r - 1);	
			if (temp[r*numCols + c] == val) {
				set_pen_color_int(37,86,138); //water
				draw_point();
			}
			else {			
				short int elev = temp[r*numCols + c];
				if (elev < 0) set_pen_color_int(37, 86, 138);
				else if (elev == 176 || elev == 183 || elev == 174 || elev == 75) set_pen_color_int(37, 86, 138);
				else if (elev >= 0 && elev < 500)//240,230,160
					set_pen_color_int(90 + elev * 150 / 500, 180 + elev * 50 / 500, 120 + elev * 40 / 500); //180, 110, 90
				else if (elev >= 500 && elev < 3000)
					set_pen_color_int(240- (elev - 500) *60/2500, 230- (elev - 500) *120/2500, 160- (elev - 500) *70/2500);//180,170,170
				else if (elev >= 3000 && elev < 3500)
					set_pen_color_int(180 + (elev - 3000) * 70 / 500, 170 + (elev - 3000) * 80 / 500 , 170 + (elev - 3000) * 80 / 500 );
				else if (elev >= 3500)
					set_pen_color(color::white);
				else { set_pen_color_int(255, 0, 0); 
				}
				draw_point();
			}
		}
	}
	fclose(file);
	fin.close();
	*xVal = longS / 3600;
	*yVal = latS / 3600;
	*secondsPerPixel = (double) spp / 3600;
}

//draws the shortest path
void drawPath(vector<intersection*> vi, List** table, place* pEnd, place* pStart, double tx, double ty, double dpp) {

	double xStart = (tx - pEnd->getLongitude()) / (-dpp);
	double yStart = (ty - pEnd->getLatitude()) / (dpp);
	
	move_to(xStart, yStart);
	set_pen_width(5);
	set_pen_width(2);
	set_pen_color_int(255, 100, 0);
	intersection* currIntersection = vi.front();
	while (currIntersection != NULL) {			
		double x = (tx - currIntersection->getLongitude()) / (-dpp);
		double y = (ty - currIntersection->getLatitude()) / (dpp);
		draw_to(x, y);
		currIntersection = currIntersection->getPreviousI();
	}
	double endX = (tx - pStart->getLongitude()) / (-dpp);
	double endY = (ty - pStart->getLatitude()) / (dpp);
	draw_to(endX, endY);
	set_pen_width(5);
	set_pen_color_int(255, 100, 0);
	set_font("veranda", 15);
	set_pen_color_int(255,255,255);
	write_string(pStart->getCityUpper(), direction::east, false);
	int xcoord = endX;
	int ycoord = endY;
	move_to(xcoord, ycoord);
	set_pen_width(5);
	set_font("veranda", 15);
	set_pen_color_int(0, 0, 0);
	move_to(xcoord - 1, ycoord - 1);
	write_string(pStart->getCityUpper(), direction::east, false);
	move_to(xcoord + 1, ycoord + 1);
	write_string(pStart->getCityUpper(), direction::east, false);
	move_to(xcoord + 1, ycoord - 1);
	write_string(pStart->getCityUpper(), direction::east, false);
	move_to(xcoord - 1, ycoord + 1);
	write_string(pStart->getCityUpper(), direction::east, false);
	move_to(xcoord, ycoord);
	set_pen_color_int(200, 255, 255);
	write_string(pStart->getCityUpper(), direction::east, false);

	move_to(xStart, yStart);
	xcoord = xStart;
	ycoord = yStart;
	set_font("veranda", 15);
	set_pen_color_int(0, 0, 0);
	move_to(xcoord - 1, ycoord - 1);
	write_string(pEnd->getCityUpper(), direction::east, false);
	move_to(xcoord + 1, ycoord + 1);
	write_string(pEnd->getCityUpper(), direction::east, false);
	move_to(xcoord + 1, ycoord - 1);
	write_string(pEnd->getCityUpper(), direction::east, false);
	move_to(xcoord - 1, ycoord + 1);
	write_string(pEnd->getCityUpper(), direction::east, false);
	move_to(xcoord, ycoord);
	set_pen_color_int(200, 255, 255);
	write_string(pEnd->getCityUpper(), direction::east, false);
}

//prompts start and end and then calls to find shortest path
void promptNav(List** table, vector<intersection*> vi, vector<connection*> vc) {
	string start, end;
	Link *startLink = NULL, *endLink = NULL;
	double longit, latit, dpp;
	string defawlt = "../MapProject/usaW125N50D60.dat";	
	loadMap(defawlt, &longit, &latit, &dpp);

	cout << "\nStarting city: ";
	while (startLink == NULL) {
		getline(cin, start);
		startLink = findCity(table, start);
		if (startLink != NULL) {
			startLink->print();
		}
	}
	cout << "\nDestination city: ";
	while (endLink == NULL) {
		getline(cin, end);
		endLink = findCity(table, end);
		if (endLink != NULL) {
			endLink->print();
		}
	}
	int st = startLink->data->getRoad();
	int en = endLink->data->getRoad();
	cout << "\n-\tFinding shortest path.\n";
	vector<intersection*> shortestpath;
	clock_t cstart = clock();
	shortestpath = shortestPath(vi[st], vi[en], vi, vc);
	if (shortestpath.empty()) {
		cout << "Error finding path\n";
		return;
	}
	double duration = (clock() - cstart) / (double)CLOCKS_PER_SEC;
	cout << "-\tDistance: " << shortestpath[0]->getShortest() << " miles, found in "<<duration<<" seconds\n";
	drawPath(shortestpath, table, endLink->data, startLink->data, longit, latit, dpp);

}

void main() {

	vector<intersection*> interarray;
	vector<connection*> connectarray;
	List** hashtable = new List*[1000];
	for (int i = 0; i < 1000; i++)
		hashtable[i] = new List();

	cout << "The United States Navigation Map\n---------------------------------\n\n";
	cout << "-\tReading named-places.txt\n";
	readFileNamedPlaces("../MapProject/named-places.txt", hashtable);	
	cout << "-\tReading intersections.txt\n";
	readFileIntersections("../MapProject/intersections.txt", interarray);
	cout << "-\tReading connections.txt\n";
	readFileConnections("../MapProject/connections.txt", connectarray, interarray);
	cout << "-\tDrawing map\n";
	
	promptNav(hashtable, interarray, connectarray);
}