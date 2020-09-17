#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <subRecord.h>
#include <epicsExport.h>

#include <Eigen/Dense>
#include <unsupported/Eigen/Splines>

typedef Eigen::Spline<double, 1, 2> Spline1D;
typedef Eigen::SplineFitting<Spline1D> SplineFitting1D;

using namespace std;

//static int gb_Debug = 0;
//epicsExportAddress(int, gb_Debug); /*iocsh export variable*/

static Eigen::RowVectorXd mgradient;
static Eigen::RowVectorXd mcurrent;

class FieldMapWave {
public:
	FieldMapWave(const string filename):mfilename(filename) {
		printf("FieldMapWave Constructor\n");
		loadData();
	};

	~FieldMapWave() {
		printf("~FieldMapWave Destructor\n");
	};

	double GetClosestKey(double pvvalue);
	void GenerateSignal(const size_t size, const double t_end, Eigen::RowVectorXd &time, Eigen::RowVectorXd &signal);
	void SaveReference(const Eigen::RowVectorXd &t, const Eigen::RowVectorXd &y); 
	void TraverseSpline(const Spline1D &spline, const size_t size, const double t_end, Eigen::RowVectorXd &time, Eigen::RowVectorXd &y);
	void SaveSpline(const Eigen::RowVectorXd &t, const Eigen::RowVectorXd &y); 
	void ExpandingSpline(const Eigen::RowVectorXd &current, const Eigen::RowVectorXd &gradient);

	Eigen::RowVectorXd & GetGradient() { return mgradient; }
	Eigen::RowVectorXd & GetCurrent() { return mcurrent; }

	//Variable
private:
	string mfilename;
	ifstream dataFile;
	//Map<Field Gradient, Current>
	map<double, double> m;
	string strToken;
	//Eigen::RowVectorXd mgradient;
	//Eigen::RowVectorXd mcurrent;

	//Function
private:
	int loadData();
};

int FieldMapWave::loadData()
{
	string strToken;
	dataFile.open(mfilename.c_str());

	if(dataFile.fail()) {
		cout <<"File Not Found" << endl;
		return(-1);
	};

	ifstream file(mfilename.c_str());
	string line;
	int size = 0;
	while(getline(file,line))
		size++;

	printf("Size:%d\n", size);

	mcurrent.resize(size+1);
	mgradient.resize(size+1);

	int index = 0;
	while(!dataFile.eof())
	{
		getline(dataFile, strToken);
		if(strToken[0] == '#' || strToken.empty() == true) continue;

		char str[strToken.size()];
		strcpy (str, strToken.c_str());

		char *pch = 0;
		if(!(pch = strtok (str," \t,"))) continue;

		mcurrent(index) = strtod(pch, NULL);

		while (pch != NULL)
		{
			if(!(pch = strtok (NULL," \t,"))) continue;
			mgradient(index) = strtod(pch, NULL);
		};

		//printf("Current: %f, Gradient: %f\n", mcurrent(index), mgradient(index));
		index++;
	};
	mcurrent(index) = 200;
	mgradient(index) = 0;

	const auto fit = SplineFitting1D::Interpolate(mgradient, 2, mcurrent);
	Spline1D spline(fit);

	// Traverse spline
	{
		double current_end = 200.0;		//later, should be configurable 
		//int last = mcurrent.size();
		//double current_end = mcurrent(last);
		size_t size = 1500;				//later, should be configurable 
		Eigen::RowVectorXd current;
		Eigen::RowVectorXd gradient;
		TraverseSpline(spline, size, current_end, current, gradient);
		ExpandingSpline(current, gradient);
	};

	return(0);
}

void FieldMapWave::GenerateSignal(const size_t size, const double t_end,
                     Eigen::RowVectorXd &time, Eigen::RowVectorXd &signal) 
{
	// Setup
	time.resize(size + 1);
	signal.resize(size + 1);

	int idx = 0;
	double t = 0;
	double dt = t_end / size;
	double f = 1.0;

	// Generate signal
	while (t <= t_end) {
		time(idx) = t;
		signal(idx) = sin(2 * M_PI * f * t);
		t += dt;
		idx++;
	}

	// Add last
	time(size) = t_end;
	signal(size) = sin(2 * M_PI * f * t_end);
}
void FieldMapWave::SaveReference(const Eigen::RowVectorXd &t, const Eigen::RowVectorXd &y) 
{
	std::ofstream points_file("./points.csv");
	if (points_file.good() != true) {
		printf("Failed to open file for output!");
		exit(-1);
	}

	for (long i = 0; i < t.size(); i++) {
		points_file << t(i) << "," << y(i) << std::endl;
	}

	points_file.close();
}

void FieldMapWave::TraverseSpline(const Spline1D &spline, const size_t size, const double t_end,
                     Eigen::RowVectorXd &time, Eigen::RowVectorXd &y) 
{
	// Setup
	time.resize(size + 1);
	y.resize(size + 1);
	int idx = 0;
	double t = 0.0;
	double dt = t_end / size;

	while (t <= t_end) {
		time(idx) = t;
		y(idx) = spline(t).coeff(0);
		//printf("1-Current: %f, Gradient: %f\n", time(idx), y(idx));
		t += dt;
		idx++;
	}

	time(size) = t_end;
	y(size) = spline(t_end).coeff(0);
}

void FieldMapWave::SaveSpline(const Eigen::RowVectorXd &t, const Eigen::RowVectorXd &y) 
{
	std::ofstream spline_file("./spline.csv");
	if (spline_file.good() != true) {
		printf("Failed to open file for output!");
		exit(-1);
	};

	for (long i = 0; i < y.size(); i++) {
		spline_file << t(i) << "," << y(i) << std::endl;
	};

	spline_file.close();
}

void FieldMapWave::ExpandingSpline(const Eigen::RowVectorXd &current, const Eigen::RowVectorXd &gradient) 
{
	for (long i = 0; i < gradient.size(); i++) {
		m.insert(std::pair<double,double>(gradient(i), current(i)));

		printf("Gradient: %f, Current: %f\n", gradient(i), current(i));
	};
}

double FieldMapWave::GetClosestKey(double pv_value)
{
	std::map<double, double>::iterator low = m.lower_bound(pv_value);
	return (low->second);
}

static FieldMapWave fieldmapwave("../../db/BI_Map");

static long FieldMapWaveInit(subRecord *pRec)
{
	// Generate and save signal data
#if 0
	Eigen::RowVectorXd time;
	Eigen::RowVectorXd signal;
	//for Testing...
	int size = 30;
	double t_end = 1.0;
	fieldmapwave.GenerateSignal(size, t_end, time, signal);
	fieldmapwave.SaveReference(time, signal);
	const auto fit = SplineFitting1D::Interpolate(signal, 2, time);
#endif

#if 0
	// Fit and generate a spline function
	//const auto fit = SplineFitting1D::Interpolate(fieldmapwave.GetGradient(), 2, fieldmapwave.GetCurrent());
	const auto fit = SplineFitting1D::Interpolate(mgradient, 2, mcurrent);
	Spline1D spline(fit);

	// Traverse spline
	{
		double current_end = 200.0;		//later, should be configurable 
		//int last = mcurrent.size();
		//double current_end = mcurrent(last);
		size_t size = 1500;				//later, should be configurable 
		Eigen::RowVectorXd current;
		Eigen::RowVectorXd gradient;
		fieldmapwave.TraverseSpline(spline, size, current_end, current, gradient);
		//fieldmapwave.SaveSpline(t, y);
		fieldmapwave.ExpandingSpline(current, gradient);
	};
#endif
	return 0;
}

static long FieldMapWaveProc(subRecord *pRec)
{
	if(pRec == NULL) return -1;
	pRec->val = fieldmapwave.GetClosestKey(pRec->a);
	return 0;
}

epicsRegisterFunction(FieldMapWaveInit);
epicsRegisterFunction(FieldMapWaveProc);
