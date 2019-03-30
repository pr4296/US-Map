# US-Map
A C++ school project that uses publicly available elevation, city, and connecting distances data to draw and determine the shortest path between any two user-provided cities.

The executable can be found at under US-Map/MapProject/Release/MapProject.exe  

# Example Executions
Naperville, IL to Miami, FL
![Naperville, IL to Miami, FL Image](https://raw.githubusercontent.com/pr4296/US-Map/master/naperville-miami.png)

Seattle, WA to Bar Harbor, ME
![Seattle, WA to Bar Harbor, ME Image](https://raw.githubusercontent.com/pr4296/US-Map/master/seattle-bar_harbor.png)

Note: Big cities in this file are broken into North/South/East/West CityName, so places like Chicago or Houston don't work, but North Chicago and South Houston do. 


# Files
Source.cpp: contains the functions (including main) that are not a part of any of the structs or classes defined in other files
MapObjects.h: contains the struct declarations and definitions for various objects needed to easily build the overall project
LinkedList.h: Implementation of a linked list  
Intersection.h: implementation of the road intersections
Connection.h: implementation of roads between intersections
Place.h: implementation of the cities (ex: Miami, Seattle) class

# Design 
I designed the map so that it matched elevation maps. The time taken for each path is displayed too.

# Problems
I didn't implement a substring finder, so that inputting “Chicago” gives the user the option to pick from the various places that have Chicago as a substring, such as North Chicago.
