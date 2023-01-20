#pragma once
#include <vector>
#include <string>

using namespace std;

class CCalculation
{
public:
	CCalculation();
	bool Start(double, double&, double&, double&, double&, double&);
	string GetPoints();
	bool ReadData();
	double MSQ(double, double, double, double);
	// void LinearDixotomy(double&, double, double, int);
	//void GoldenRatioGrad(double&, double&, double&);
	void GoldenRatioGrad(double&, double&, double&, double&);
	void GoldenRatio(double&, double, double, int);
private:
	struct Points
	{
		double x;
		double y;
		double z;
	};
	vector<Points> m_pointsVector;
	double m_size;
	double m_eps;
	double m_maxDelta;
	double m_minDelta;
};

