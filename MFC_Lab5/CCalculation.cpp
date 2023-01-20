#include "pch.h"
#include "CCalculation.h"
#include <fstream>

#define CT_A 0 
#define CT_B 1 
#define CT_C 2 
#define CT_R 3 

#define GOLDEN_RATIO 1
#define GOLDEN_RATIO_GRAD 2
#define F 1.6180339887 // Константа для Золотого Сечения


CCalculation::CCalculation() : m_size(0), m_minDelta(DBL_MAX), m_maxDelta(DBL_MIN)
{
	m_pointsVector.clear();
}

bool CCalculation::ReadData()
{
	TCHAR szFilters[] = _T("Text files (*.txt)|*.txt|Binary Files (*.dat)|*.dat|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("dat"), _T("*.dat"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();
		ifstream file(pathName);
		if (file.fail())
			return false;
		m_pointsVector.clear();
		m_size = 0;
		Points tempPoint;
		while (file >> tempPoint.x >> tempPoint.y >> tempPoint.z)
		{
			m_pointsVector.push_back(tempPoint);
		}
		file.close();
	}
	m_size = m_pointsVector.size();

	return true;
}


bool CCalculation::Start(double eps, double & A, double & B, double & C, double & R, double & diff)
{
	m_eps = eps;
	if ((m_eps <= 0) || (m_size <= 0))
		return false;

	A = B = C = R = 0;
	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
	{
		A += (*iter).x;
		B += (*iter).y;
		C += (*iter).z;
	}
	A /= m_size;
	B /= m_size;
	C /= m_size;

	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
	{
		R += sqrt(pow((*iter).x - A, 2) + pow((*iter).y - B, 2) + pow((*iter).z - C, 2));
	}
	R /= m_size;
	double A_old, B_old, C_old, R_old;
	while (true)
	{
		A_old = A;
		B_old = B;
		C_old = C;
		R_old = R;
		GoldenRatioGrad(A, B, C, R);
		
		if (sqrt(pow(A - A_old, 2) + pow(B - B_old, 2) + pow(C - C_old, 2) + pow(R - R_old, 2)) < m_eps)
			break;
	}

	double delta;
	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
	{

		//delta = sqrt(pow((*iter).x - A_old, 2) + pow((*iter).y - B_old, 2) + pow((*iter).z - C_old, 2)) - sqrt(R_old * R_old);
		delta = sqrt(pow((*iter).x - A, 2) + pow((*iter).y - B, 2) + pow((*iter).z - C, 2)) - sqrt(R * R);
		if (delta < m_minDelta)
			m_minDelta = delta;
		else if (delta > m_maxDelta)
			m_maxDelta = delta;
	}

	diff = (abs(m_maxDelta) + abs(m_minDelta)) / 2;
	// diff = m_minDelta;

	return true;
}

string CCalculation::GetPoints()
{
	string tempString = to_string(m_pointsVector[0].z);
	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
	{
		tempString = tempString + to_string((*iter).x) + "\t" + to_string((*iter).y) + "\t" + to_string((*iter).z) + "\n";
	}
	return tempString;
}

//double CCalculation::MSQ(double a, double b, double r)
//{
//	double res = 0;
//	double delta;
//	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
//	{
//		delta = pow((*iter).x - a, 2) + pow((*iter).y - b, 2) - r * r;
//		res += delta * delta;
//	}
//	return res;
//}

double CCalculation::MSQ(double a, double b, double c, double r)
{
	double res = 0;
	double delta;
	for (vector<Points>::iterator iter = m_pointsVector.begin(); iter != m_pointsVector.end(); ++iter)
	{
		delta = pow((*iter).x - a, 2) + pow((*iter).y - b, 2) + pow((*iter).z - c, 2) - r * r;
		if (delta)
		res += delta * delta;
	}
	return res;
}

//void CCalculation::LinearDixotomy(double& value1, double value2, double value3, int type)
//{
//	double step = m_eps * 100;
//
//	double V_Left, V_Right, MSQ1, MSQ2;
//
//	switch (type)
//	{
//	case CT_A:
//		MSQ1 = MSQ(value1, value2, value3);
//		MSQ2 = MSQ(value1 - step, value2, value3);
//		break;
//	case CT_B:
//		MSQ1 = MSQ(value2, value1, value3);
//		MSQ2 = MSQ(value2, value1 - step, value3);
//		break;
//	case CT_R:
//		MSQ1 = MSQ(value2, value3, value1);
//		MSQ2 = MSQ(value2, value3, value1 - step);
//		break;
//	}
//	if (MSQ1 > MSQ2)
//	{
//		V_Right = V_Left = value1;
//		while (true)
//		{
//			V_Left = V_Left - step;
//			switch (type)
//			{
//			case CT_A:
//				MSQ1 = MSQ(V_Left, value2, value3);
//				MSQ2 = MSQ(V_Left + step, value2, value3);
//				break;
//			case CT_B:
//				MSQ1 = MSQ(value2, V_Left, value3);
//				MSQ2 = MSQ(value2, V_Left + step, value3);
//				break;
//			case CT_R:
//				MSQ1 = MSQ(value2, value3, V_Left);
//				MSQ2 = MSQ(value2, value3, V_Left + step);
//				break;
//			}
//			if (MSQ1 > MSQ2)
//				break;
//		}
//	}
//	else
//	{
//		V_Right = V_Left = value1;
//		while (true)
//		{
//			V_Right += step;
//			switch (type)
//			{
//			case CT_A:
//				MSQ1 = MSQ(V_Right, value2, value3);
//				MSQ2 = MSQ(V_Right + step, value2, value3);
//				break;
//			case CT_B:
//				MSQ1 = MSQ(value2, V_Right, value3);
//				MSQ2 = MSQ(value2, V_Right + step, value3);
//				break;
//			case CT_R:
//				MSQ1 = MSQ(value2, value3, V_Right);
//				MSQ2 = MSQ(value2, value3, V_Right + step);
//				break;
//			}
//			if (MSQ1 < MSQ2)
//				break;
//		}
//	}
//	double V_Mid;
//
//	while (V_Right - V_Left > m_eps)
//	{
//		V_Mid = (V_Right + V_Left) / 2;
//		switch (type)
//		{
//		case CT_A:
//			MSQ1 = MSQ(V_Mid + m_eps / 3, value2, value3);
//			MSQ2 = MSQ(V_Mid - m_eps / 3, value2, value3);
//			break;
//		case CT_B:
//			MSQ1 = MSQ(value2, V_Mid + m_eps / 3, value3);
//			MSQ2 = MSQ(value2, V_Mid - m_eps / 3, value3);
//			break;
//		case CT_R:
//			MSQ1 = MSQ(value2, value3, V_Mid + m_eps / 3);
//			MSQ2 = MSQ(value2, value3, V_Mid - m_eps / 3);
//			break;
//		}
//		MSQ1 < MSQ2 ? V_Left = V_Mid : V_Right = V_Mid;
//	}
//	value1 = (V_Right + V_Left) / 2;
//}

void CCalculation::GoldenRatioGrad(double& value1, double& value2, double& value3, double& value4)
{
	double step = m_eps * 100;
	const int PARAMS_AMOUNT = 4;

	double V_Left, V_Right, MSQ1, MSQ2;


	double tMSQ1, tMSQ2, deltaMSQ;
	int typeArray[PARAMS_AMOUNT];
	double deltaMSQArray[PARAMS_AMOUNT];
	double* valuesArray[PARAMS_AMOUNT];

	//A
	tMSQ1 = MSQ(value1, value2, value3, value4);
	tMSQ2 = MSQ(value1 - step, value2, value3, value4);
	deltaMSQ = abs(tMSQ1 - tMSQ2);
	typeArray[0] = CT_A;
	deltaMSQArray[0] = deltaMSQ;
	valuesArray[0] = &value1;

	//B
	tMSQ1 = MSQ(value1, value2, value3, value4);
	tMSQ2 = MSQ(value1, value2 - step, value3, value4);
	deltaMSQ = abs(tMSQ1 - tMSQ2);
	typeArray[1] = CT_B;
	deltaMSQArray[1] = deltaMSQ;
	valuesArray[1] = &value2;
	//C
	tMSQ1 = MSQ(value1, value2, value3, value4);
	tMSQ2 = MSQ(value1, value2, value3 - step, value4);
	deltaMSQ = abs(tMSQ1 - tMSQ2);
	typeArray[2] = CT_C;
	deltaMSQArray[2] = deltaMSQ;
	valuesArray[2] = &value3;
	//R
	tMSQ1 = MSQ(value1, value2, value3, value4);
	tMSQ2 = MSQ(value1, value2, value3, value4 - step);
	deltaMSQ = abs(tMSQ1 - tMSQ2);
	typeArray[3] = CT_R;
	deltaMSQArray[3] = deltaMSQ;
	valuesArray[3] = &value4;
	for (int i = 0; i != PARAMS_AMOUNT - 1; ++i)
	{
		if (deltaMSQArray[i] < deltaMSQArray[i + 1])
		{
			swap(deltaMSQArray[i], deltaMSQArray[i + 1]);
			swap(typeArray[i], typeArray[i + 1]);
			swap(valuesArray[i], valuesArray[i + 1]);
		}
	}
	//value1 = deltaMSQArray[0];
	//value2 = deltaMSQArray[1];
	//value3 = deltaMSQArray[2];
	for (int i = 0; i != PARAMS_AMOUNT; ++i)
	{
		switch (typeArray[i])
		{
		case CT_A:
			MSQ1 = MSQ(value1, value2, value3, value4);
			MSQ2 = MSQ(value1 - step, value2, value3, value4);
			break;
		case CT_B:
			MSQ1 = MSQ(value1, value2, value3, value4);
			MSQ2 = MSQ(value1, value2 - step, value3, value4);
			break;
		case CT_C:
			MSQ1 = MSQ(value1, value2, value3, value4);
			MSQ2 = MSQ(value1, value2, value3 - step, value4);
			break;
		case CT_R:
			MSQ1 = MSQ(value1, value2, value3, value4);
			MSQ2 = MSQ(value1, value2, value3, value4 - step);
			break;
		}
		if (MSQ1 > MSQ2)
		{
			V_Right = V_Left = *valuesArray[i];
			while (true)
			{
				V_Left = V_Left - step;
				switch (typeArray[i])
				{
				case CT_A:
					MSQ1 = MSQ(V_Left, value2, value3, value4);
					MSQ2 = MSQ(V_Left + step, value2, value3, value4);
					break;
				case CT_B:
					MSQ1 = MSQ(value1, V_Left, value3, value4);
					MSQ2 = MSQ(value1, V_Left + step, value3, value4);
					break;
				case CT_C:
					MSQ1 = MSQ(value1, value2, V_Left, value4);
					MSQ2 = MSQ(value1, value2, V_Left + step, value4);
					break;
				case CT_R:
					MSQ1 = MSQ(value1, value2, value3, V_Left);
					MSQ2 = MSQ(value1, value2, value3, V_Left + step);
					break;
				}
				if (MSQ1 > MSQ2)
					break;
			}
		}
		else
		{
			V_Right = V_Left = *valuesArray[i];
			while (true)
			{
				V_Right += step;
				switch (typeArray[i])
				{
				case CT_A:
					MSQ1 = MSQ(V_Right, value2, value3, value4);
					MSQ2 = MSQ(V_Right + step, value2, value3, value4);
					break;
				case CT_B:
					MSQ1 = MSQ(value1, V_Right, value3, value4);
					MSQ2 = MSQ(value1, V_Right + step, value3, value4);
					break;
				case CT_C:
					MSQ1 = MSQ(value1, value2, V_Right, value4);
					MSQ2 = MSQ(value1, value2, V_Right + step, value4);
					break;
				case CT_R:
					MSQ1 = MSQ(value1, value2, value3, V_Right);
					MSQ2 = MSQ(value1, value2, value3, V_Right + step);
					break;
				}
				if (MSQ1 < MSQ2)
					break;
			}
		}
		double V_Mid;
		double sigma;

		while (V_Right - V_Left > m_eps)
		{
			V_Mid = (V_Right + V_Left) / 2;
			sigma = (V_Right - V_Left) * (1 / F - 0.5);
			switch (typeArray[i])
			{
			case CT_A:
				MSQ1 = MSQ(V_Mid + sigma, value2, value3, value4);
				MSQ2 = MSQ(V_Mid - sigma, value2, value3, value4);
				break;
			case CT_B:
				MSQ1 = MSQ(value1, V_Mid + sigma, value3, value4);
				MSQ2 = MSQ(value1, V_Mid - sigma, value3, value4);
				break;
			case CT_C:
				MSQ1 = MSQ(value1, value2, V_Mid + sigma, value4);
				MSQ2 = MSQ(value1, value2, V_Mid - sigma, value4);
				break;
			case CT_R:
				MSQ1 = MSQ(value1, value2, value3, V_Mid + sigma);
				MSQ2 = MSQ(value1, value2, value3, V_Mid - sigma);
				break;
			}
			MSQ1 < MSQ2 ? V_Left = V_Mid : V_Right = V_Mid;
		}
		*valuesArray[i] = (V_Right + V_Left) / 2;
		V_Right = V_Left = MSQ1 = MSQ2 = 0;

		//double tempIndexes[PARAMS_AMOUNT];
		////A
		//tMSQ1 = MSQ(value1, value2, value3, value4);
		//tMSQ2 = MSQ(value1 - step, value2, value3, value4);
		//deltaMSQ = abs(tMSQ1 - tMSQ2);
		//for (int j = 0; j != PARAMS_AMOUNT; ++j)
		//{
		//	if (typeArray[j] == CT_A)
		//	{
		//		deltaMSQArray[j] = deltaMSQ;
		//		tempIndexes[CT_A] = typeArray[j];
		//		break;
		//	}
		//}

		////B
		//tMSQ1 = MSQ(value1, value2, value3, value4);
		//tMSQ2 = MSQ(value1, value2 - step, value3, value4);
		//deltaMSQ = abs(tMSQ1 - tMSQ2);
		//for (int j = 0; j != PARAMS_AMOUNT; ++j)
		//{
		//	if (typeArray[j] == CT_B)
		//	{
		//		deltaMSQArray[j] = deltaMSQ;
		//		tempIndexes[CT_B] = typeArray[j];
		//		break;
		//	}
		//}
		////C
		//tMSQ1 = MSQ(value1, value2, value3, value4);
		//tMSQ2 = MSQ(value1, value2, value3 - step, value4);
		//deltaMSQ = abs(tMSQ1 - tMSQ2);
		//for (int j = 0; j != PARAMS_AMOUNT; ++j)
		//{
		//	if (typeArray[j] == CT_C)
		//	{
		//		deltaMSQArray[j] = deltaMSQ;
		//		tempIndexes[CT_C] = typeArray[j];
		//		break;
		//	}
		//}
		////R
		//tMSQ1 = MSQ(value1, value2, value3, value4);
		//tMSQ2 = MSQ(value1, value2, value3, value4 - step);
		//deltaMSQ = abs(tMSQ1 - tMSQ2);
		//for (int j = 0; j != PARAMS_AMOUNT; ++j)
		//{
		//	if (typeArray[j] == CT_R)
		//	{
		//		deltaMSQArray[j] = deltaMSQ;
		//		tempIndexes[CT_R] = typeArray[j];
		//		break;
		//	}
		//}
		//if (i < PARAMS_AMOUNT - 2)
		//{
		//	for (int j = i + 1; j != PARAMS_AMOUNT - 2; ++j)
		//	{
		//		if (deltaMSQArray[j] < deltaMSQArray[j + 1])
		//		{
		//			swap(deltaMSQArray[j], deltaMSQArray[j + 1]);
		//			swap(typeArray[j], typeArray[j + 1]);
		//			swap(valuesArray[j], valuesArray[j + 1]);
		//		}
		//	}
		//}
	}
}

//void CCalculation::GoldenRatioGrad(double& value1, double& value2, double& value3)
//{
//	double step = m_eps * 100;
//	const int PARAMS_AMOUNT = 3;
//
//	double V_Left, V_Right, MSQ1, MSQ2;
//
//
//	double tMSQ1, tMSQ2, deltaMSQ;
//	int typeArray[PARAMS_AMOUNT];
//	double deltaMSQArray[PARAMS_AMOUNT];
//	double* valuesArray[PARAMS_AMOUNT];
//
//	//A
//	tMSQ1 = MSQ(value1, value2, value3);
//	tMSQ2 = MSQ(value1 - step, value2, value3);
//	deltaMSQ = abs(tMSQ1 - tMSQ2);
//	typeArray[0] = CT_A;
//	deltaMSQArray[0] = deltaMSQ;
//	valuesArray[0] = &value1;
//
//	//B
//	tMSQ1 = MSQ(value1, value2, value3);
//	tMSQ2 = MSQ(value1, value2 - step, value3);
//	deltaMSQ = abs(tMSQ1 - tMSQ2);
//	typeArray[1] = CT_B;
//	deltaMSQArray[1] = deltaMSQ;
//	valuesArray[1] = &value2;
//	//R
//	tMSQ1 = MSQ(value1, value2, value3);
//	tMSQ2 = MSQ(value1, value2, value3 - step);
//	deltaMSQ = abs(tMSQ1 - tMSQ2);
//	typeArray[2] = CT_R;
//	deltaMSQArray[2] = deltaMSQ;
//	valuesArray[2] = &value3;
//	for (int i = 0; i != PARAMS_AMOUNT - 1; ++i)
//	{
//		if (deltaMSQArray[i] < deltaMSQArray[i + 1])
//		{
//			swap(deltaMSQArray[i], deltaMSQArray[i + 1]);
//			swap(typeArray[i], typeArray[i + 1]);
//			swap(valuesArray[i], valuesArray[i + 1]);
//		}
//	}
//	//value1 = deltaMSQArray[0];
//	//value2 = deltaMSQArray[1];
//	//value3 = deltaMSQArray[2];
//	for (int i = 0; i != PARAMS_AMOUNT; ++i)
//	{
//		switch (typeArray[i])
//		{
//		case CT_A:
//			MSQ1 = MSQ(value1, value2, value3);
//			MSQ2 = MSQ(value1 - step, value2, value3);
//			break;
//		case CT_B:
//			MSQ1 = MSQ(value1, value2, value3);
//			MSQ2 = MSQ(value1, value2 - step, value3);
//			break;
//		case CT_R:
//			MSQ1 = MSQ(value1, value2, value3);
//			MSQ2 = MSQ(value1, value2, value3 - step);
//			break;
//		}
//		if (MSQ1 > MSQ2)
//		{
//			V_Right = V_Left = *valuesArray[i];
//			while (true)
//			{
//				V_Left = V_Left - step;
//				switch (typeArray[i])
//				{
//				case CT_A:
//					MSQ1 = MSQ(V_Left, value2, value3);
//					MSQ2 = MSQ(V_Left + step, value2, value3);
//					break;
//				case CT_B:
//					MSQ1 = MSQ(value2, V_Left, value3);
//					MSQ2 = MSQ(value2, V_Left + step, value3);
//					break;
//				case CT_R:
//					MSQ1 = MSQ(value2, value3, V_Left);
//					MSQ2 = MSQ(value2, value3, V_Left + step);
//					break;
//				}
//				if (MSQ1 > MSQ2)
//					break;
//			}
//		}
//		else
//		{
//			V_Right = V_Left = *valuesArray[i];
//			while (true)
//			{
//				V_Right += step;
//				switch (typeArray[i])
//				{
//				case CT_A:
//					MSQ1 = MSQ(V_Right, value2, value3);
//					MSQ2 = MSQ(V_Right + step, value2, value3);
//					break;
//				case CT_B:
//					MSQ1 = MSQ(value2, V_Right, value3);
//					MSQ2 = MSQ(value2, V_Right + step, value3);
//					break;
//				case CT_R:
//					MSQ1 = MSQ(value2, value3, V_Right);
//					MSQ2 = MSQ(value2, value3, V_Right + step);
//					break;
//				}
//				if (MSQ1 < MSQ2)
//					break;
//			}
//		}
//		double V_Mid;
//		double sigma;
//
//		while (V_Right - V_Left > m_eps)
//		{
//			V_Mid = (V_Right + V_Left) / 2;
//			sigma = (V_Right - V_Left) * (1 / F - 0.5);
//			switch (typeArray[i])
//			{
//			case CT_A:
//				MSQ1 = MSQ(V_Mid + sigma, value2, value3);
//				MSQ2 = MSQ(V_Mid - sigma, value2, value3);
//				break;
//			case CT_B:
//				MSQ1 = MSQ(value1, V_Mid + sigma, value3);
//				MSQ2 = MSQ(value1, V_Mid - sigma, value3);
//				break;
//			case CT_R:
//				MSQ1 = MSQ(value1, value2, V_Mid + sigma);
//				MSQ2 = MSQ(value1, value2, V_Mid - sigma);
//				break;
//			}
//			MSQ1 < MSQ2 ? V_Left = V_Mid : V_Right = V_Mid;
//		}
//		*valuesArray[i] = (V_Right + V_Left) / 2;
//		V_Right = V_Left = MSQ1 = MSQ2 = 0;
//
//		if (i == 0)
//		{
//			//A
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1 - step, value2, value3);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_A)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//			deltaMSQArray[0] = deltaMSQ;
//			//B
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1, value2 - step, value3);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_B)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//			//R
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1, value2, value3 - step);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_R)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//			if (deltaMSQArray[1] < deltaMSQArray[2])
//			{
//				swap(deltaMSQArray[1], deltaMSQArray[2]);
//				swap(typeArray[1], typeArray[2]);
//				swap(valuesArray[1], valuesArray[2]);
//			}
//		}
//		else if (i == 1)
//		{
//			//A
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1 - step, value2, value3);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_A)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//			deltaMSQArray[0] = deltaMSQ;
//			//B
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1, value2 - step, value3);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_B)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//			//R
//			tMSQ1 = MSQ(value1, value2, value3);
//			tMSQ2 = MSQ(value1, value2, value3 - step);
//			deltaMSQ = abs(tMSQ1 - tMSQ2);
//			for (int j = 0; j != PARAMS_AMOUNT; ++j)
//			{
//				if (typeArray[j] == CT_R)
//				{
//					deltaMSQArray[j] = deltaMSQ;
//					break;
//				}
//			}
//		}
//	}
//}

//void CCalculation::GoldenRatio(double& value1, double value2, double value3, int type)
//{
//	double step = m_eps * 100;
//
//	double V_Left, V_Right, MSQ1, MSQ2;
//
//	switch (type)
//	{
//	case CT_A:
//		MSQ1 = MSQ(value1, value2, value3);
//		MSQ2 = MSQ(value1 - step, value2, value3);
//		break;
//	case CT_B:
//		MSQ1 = MSQ(value2, value1, value3);
//		MSQ2 = MSQ(value2, value1 - step, value3);
//		break;
//	case CT_R:
//		MSQ1 = MSQ(value2, value3, value1);
//		MSQ2 = MSQ(value2, value3, value1 - step);
//		break;
//	}
//	if (MSQ1 > MSQ2)
//	{
//		V_Right = V_Left = value1;
//		while (true)
//		{
//			V_Left = V_Left - step;
//			switch (type)
//			{
//			case CT_A:
//				MSQ1 = MSQ(V_Left, value2, value3);
//				MSQ2 = MSQ(V_Left + step, value2, value3);
//				break;
//			case CT_B:
//				MSQ1 = MSQ(value2, V_Left, value3);
//				MSQ2 = MSQ(value2, V_Left + step, value3);
//				break;
//			case CT_R:
//				MSQ1 = MSQ(value2, value3, V_Left);
//				MSQ2 = MSQ(value2, value3, V_Left + step);
//				break;
//			}
//			if (MSQ1 > MSQ2)
//				break;
//		}
//	}
//	else
//	{
//		V_Right = V_Left = value1;
//		while (true)
//		{
//			V_Right += step;
//			switch (type)
//			{
//			case CT_A:
//				MSQ1 = MSQ(V_Right, value2, value3);
//				MSQ2 = MSQ(V_Right + step, value2, value3);
//				break;
//			case CT_B:
//				MSQ1 = MSQ(value2, V_Right, value3);
//				MSQ2 = MSQ(value2, V_Right + step, value3);
//				break;
//			case CT_R:
//				MSQ1 = MSQ(value2, value3, V_Right);
//				MSQ2 = MSQ(value2, value3, V_Right + step);
//				break;
//			}
//			if (MSQ1 < MSQ2)
//				break;
//		}
//	}
//	double V_Mid;
//	double sigma;
//
//	while (V_Right - V_Left > m_eps)
//	{
//		V_Mid = (V_Right + V_Left) / 2;
//		sigma = (V_Right - V_Left) * (1 / F - 0.5);
//		switch (type)
//		{
//		case CT_A:
//			MSQ1 = MSQ(V_Mid + sigma, value2, value3);
//			MSQ2 = MSQ(V_Mid - sigma, value2, value3);
//			break;
//		case CT_B:
//			MSQ1 = MSQ(value2, value3, V_Mid + sigma);
//			MSQ2 = MSQ(value2, value3, V_Mid - sigma);
//			break;
//		case CT_R:
//			MSQ1 = MSQ(value2, value3, V_Mid + sigma);
//			MSQ2 = MSQ(value2, value3, V_Mid - sigma);
//			break;
//		}
//		MSQ1 < MSQ2 ? V_Left = V_Mid : V_Right = V_Mid;
//	}
//	value1 = (V_Right + V_Left) / 2;
//}
