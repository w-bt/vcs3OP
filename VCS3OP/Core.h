#pragma once
#include "Share.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Core
{
public:
	int 	widthSI, heightSI, widthCanvas, heightCanvas;
	static	int optPause, optPreview, optOthers;
	Mat 	SI, filteredSI, canvas;
	string	p, q, g, h, v1, v2, v3, ext, scheme;

	Core();
	void	reset();
	int 	viewMainMenu();
	void 	viewEncoding();
	void 	viewDecoding();
	void 	viewOption();
	void	setOptionPause();
	void	setOptionPreview();
	void	setOptionOthers();
	void 	centerString(char* s);
	void 	setSecretImage();
	void 	filter();
	void	setChameleonVariable();	
	int		getRandomNumber();
	string	getChameleonHashValue(string seed1, string seed2);
	string	getSeed(int index, Share& S);
	void	genPoints(Share& S1, Share& S2);
	int		genFirstPoint(Share& S1, Share& S2);
	static Point2i index2point(int index, int width);
	vector<int>	getPosition(Share S);
	vector<int>	getMinDistance(int a1, int a2, int b1, int b2, int c1, int c2, int d1, int d2);
	int		checkFlag2(vector<int> position1, vector<int> position2);
	int		checkFlag3(vector<int> position1, vector<int> position2);
	vector<int>	genSecondPoint(Share& S1, Share& S2, int flag1);
	int		genThirdPoint(Share& S1, Share& S2, int flag1, vector<int> flag2);
	void	getIntersectionProp(Share& S1, Share& S2);
	void	previewIA(Share S1, Share S2);
	void	randomSIPosition(Share& S1, Share& S2);
	static Mat	generateNewImg(int h, int w, int channel);
	void	getNormalRotationImage(Share& S);
	void	getRotationImage(Share& S);
	void	generateShare(Share& S1, Share& S2);
	static string str2bin(string str,int base,int pad);

	void	splitParam(string binParam1, string binParam2, Share& S1, Share& S2);
	string 	str2number(string str, int baseSource, int baseDestination);
	int 	str2int(string str,int base);
	void 	stacking(Share S1, Share S2);
	void	getCanvas(Share& S1, Share& S2);
	void	stackShares2Canvas(Share S1, Share S2);
	static int getX();
};