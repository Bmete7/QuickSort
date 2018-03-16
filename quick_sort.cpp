/*
Burak Mete
150140131
26.10.17*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <cstdlib>
#define N 100000
using namespace std;

class geoCount{
	int population;
	string minAge;
	string maxAge;
	string gender;
   string zip_code;
	string Geo_id;
	public:
		geoCount(string pop = "" , string minAge = "" , string maxAge = "" , string gender = "", string zip_code = "", string Geo_id= "") : population(population) , zip_code(zip_code) , Geo_id(Geo_id) , minAge(minAge), maxAge(maxAge) , gender(gender) {		istringstream (pop) >> population;};
		int getPopulation(){return population;};
		string getMinAge(){return minAge;};
		string getMaxAge(){return maxAge;};
		string getZipCode(){return zip_code;};
		string getGender(){return gender;};
		string getGeoId(){return Geo_id;};
};

class geoContainer{
	int elementCount;
	vector<geoCount> geo_list;
	public:
		geoContainer(){elementCount =0;};
		void addGeo(const geoCount& obj);
		int getelementcount(){return elementCount;};
		void QuickSort(int first, int last);
		int partition(int first,int last);
		string returnIndex(int i);
};

string geoContainer::returnIndex(int i){ /// to return a full string of the attributes seperated by a comma, for faster file writing
	stringstream ss;
	ss << geo_list[i].getPopulation();
	string pop_str = ss.str();
	
	return pop_str + "," + geo_list[i].getMinAge() + "," + geo_list[i].getMaxAge() + "," + geo_list[i].getGender() + "," + geo_list[i].getZipCode() + "," + geo_list[i].getGeoId();
}

void geoContainer::QuickSort(int first, int last){
	if(last > first){
		int piv = partition(first,last);
		QuickSort(first,piv-1);			// Recursive calls
		QuickSort(piv+1, last);			// around the pivot's index
	}
}

int geoContainer::partition(int first,int last){
	int pivotPop = geo_list[last].getPopulation();		// selecting the pivot as the last element.
	string pivotGeo = geo_list[last].getGeoId();			// we also need to check their geo_id values, if population values are same
	
	int i = first-1;
	for(int j = first; j < last; ++j){
		if(geo_list[j].getPopulation() < pivotPop || (geo_list[j].getPopulation()  == pivotPop &&  pivotGeo.compare( geo_list[j].getGeoId() ) > 0)  ){ /// Sort's condition. Only works when population is less, or populations are euqal and geo_list id is less. 		
				++i;
				geoCount gTemp;
				gTemp = geo_list[i];
				geo_list[i] = geo_list[j];
				geo_list[j] = gTemp;
		}
	}
	geoCount gTemp;
	gTemp = geo_list[last];
	geo_list[last] = geo_list[++i];
	geo_list[i] = gTemp;
	return i; // return the replaced pivot's index
}

void geoContainer::addGeo(const geoCount& obj){
	++elementCount;
	geo_list.push_back(obj);
}
	
int main(int argc, char* argv[]){

	ifstream ii;
	ii.open("population_by_zip_2010.csv");
	geoContainer gList;
	
	string empty;
	getline(ii,empty, '\n');
	
	string popS,nAge,xAge,zCode,gend,geoId;
	for(int i = 0 ; i < N; ++i){
		// Let's take all of the identities as strings. Just turn population value to INT within the constructor. ( Because of performance issues )
		getline(ii,popS,',');
		getline(ii,nAge,',');		
		getline(ii,xAge,',');
		getline(ii,gend,',');
		getline(ii,zCode, ',');
		getline(ii,geoId, '\n');
		geoCount g1(popS,nAge,xAge,gend,zCode,geoId);
		
		if(popS == "" || zCode == "" || geoId == "") continue; // These attributes is supposed to be nut null, skip if they are null.
	
		gList.addGeo(g1);
	}
	
	ii.close();
	
	clock_t beginClock = clock();
	
	gList.QuickSort(0,N-1);	
	
	clock_t endClock = clock();
	cout << "Time elapsed:\t" << double(endClock-beginClock) / CLOCKS_PER_SEC << " s" << endl;
	
	ofstream oo;
	oo.open("output.csv");
	
	
	oo << "population,minimum_age,maximum_age,gender,zipcode,geo_id\n";
	for(int i = 0 ; i < N; ++i){
		oo << gList.returnIndex(i); 
		oo << "\n";
	}
	cout << endl;
	cout << "Sorted list created at 'output.csv' " << endl;
	oo.close();
	return 0;
}
