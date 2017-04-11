#include "stdafx.h"
#include "targetver.h"
#include "Share.h"
#include "Core.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <set>
#include <algorithm>    // std::shuffle, sort
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#if defined(_MSC_VER) && _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146 4244 4800)
#endif
#include <mpirxx.h>
#if defined(_MSC_VER) && _MSC_VER
#pragma warning(pop)
#endif

using namespace cv;
using namespace std;

Share::Share(int i)
{
	index		= i;
	angle		= 0;
	revAngle	= 0;
	widthSS 	= 0;
	heightSS 	= 0;
	width		= 0;
	height		= 0;
	rotWidth	= 0;
	rotHeight	= 0;
	paramLength	= ((19*3) + 26 + 13);
}

Share::~Share() {
   return;
}

void Share::setSSSize(int widthSI, int heightSI)
{
	int flag	= 0;
	int	flagW	= 0;
	int w;
	while(flagW == 0){
		cout << "\n> Input the width of SS" << index << ": ";
		cin >> w;
		if(index == 1){
			if(w >= widthSI){
				widthSS		= w;
				width		= w * 2;
				flagW		= 1;
			} else {
				cout << "\nThe width value should be equal or greater than " << widthSI << endl;
			}
		} else {
			if(w >= widthSI){
				widthSS		= w;
				width		= w * 2;
				flagW		= 1;
			} else {
				if(w >= heightSI){
					widthSS		= w;
					width		= w * 2;
					flagW		= 1;
					flag		= 1;
				} else {
					cout << "\nThe width value should be equal or greater than " << widthSI << " or " << heightSI << endl;
				}
			}
		}
	}

	int	flagH	= 0;
	int h;
	while(flagH == 0){
		cout << "\n> Input the height of SS" << index << ": ";
		cin >> h;
		if(index == 1){
			if(h >= heightSI){
				heightSS	= h;
				height		= h * 2;
				flagH		= 1;
			} else {
				cout << "\nThe height value should be equal or greater than " << heightSI << endl;
			}
		} else {
			if(flag){
				if(h >= widthSI){
					heightSS	= h;
					height		= h * 2;
					flagH		= 1;
				} else {
					cout << "\nThe height value should be equal or greater than " << widthSI << endl;
				}
			} else {
				if(h >= heightSI){
					heightSS	= h;
					height		= h * 2;
					flagH		= 1;
				} else {
					cout << "\nThe height value should be equal or greater than " << heightSI << endl;
				}
			}
		}
	}
}

void Share::setAvailableOrientation(int widthSI, int heightSI){
	if((widthSS >= widthSI)&&(widthSS >= heightSI)&&(heightSS >= widthSI)&&(heightSS >= heightSI)){
		availableOrientation.clear();
		availableOrientation.push_back(0);
		availableOrientation.push_back(1);
		availableOrientation.push_back(2);
		availableOrientation.push_back(3);
	} else {
		if((widthSS >= widthSI)&&(heightSS >= heightSI)){
			availableOrientation.clear();
			availableOrientation.push_back(0);
			availableOrientation.push_back(2);
		} else {
			availableOrientation.clear();
			availableOrientation.push_back(1);
			availableOrientation.push_back(3);
		}
	}
}

void Share::getRotationPoints(){
	int tempY, tempX;
	Point2i co;

	if (angle == 0){
		rotP[0] = p[0];
		rotP[1] = p[1];
		rotP[2] = p[2];
	} else {
		if (angle == 90){
			for(int i = 0; i < 3; i = i + 1){
				tempY		= p[i].x;
				tempX		= heightSS - p[i].y + 1;
				rotP[i]		= Point(tempX, tempY);
			}
		} else {
			if (angle == 270) {
				for(int i = 0; i < 3; i = i + 1){
					tempY		= widthSS - p[i].x + 1;
					tempX		= p[i].y;
					rotP[i]		= Point(tempX, tempY);
				}    
			} else {
				if (angle == 180){
					for(int i = 0; i < 3; i = i + 1){
						tempY		= heightSS - p[i].y + 1;
						tempX		= widthSS - p[i].x + 1;
						rotP[i]		= Point(tempX, tempY);
					}          
				}
			}
		}
	}
}

void Share::getRotationNormalPoints(){
	int tempY, tempX;
	Point2i co;

	if (angle == 0){
		rotNP[0] = nP[0];
		rotNP[1] = nP[1];
		rotNP[2] = nP[2];
	} else {
		if (angle == 90){
			for(int i = 0; i < 3; i = i + 1){
				tempY		= nP[i].x;
				tempX		= (height/2) - nP[i].y + 1;
				rotNP[i]	= Point(tempX, tempY);
			}
		} else {
			if (angle == 270) {
				for(int i = 0; i < 3; i = i + 1){
					tempY		= (width/2) - nP[i].x + 1;
					tempX		= nP[i].y;
					rotNP[i]	= Point(tempX, tempY);
				}    
			} else {
				if (angle == 180){
					for(int i = 0; i < 3; i = i + 1){
						tempY		= (height/2) - nP[i].y + 1;
						tempX		= (width/2) - nP[i].x + 1;
						rotNP[i]	= Point(tempX, tempY);
					}          
				}
			}
		}
	}
}

void Share::getRotationSize(){
	if((angle == 0)||(angle == 180)){
		rotWidthSS	= widthSS;
		rotHeightSS	= heightSS;
	} else {
		rotWidthSS	= heightSS;
		rotHeightSS	= widthSS;
	}
}

void Share::setIntersection(int startX, int endX, int startY, int endY){
	intersection.clear();
	intersection.push_back(Point(startX,startY));
	intersection.push_back(Point(endX,startY));
	intersection.push_back(Point(startX,endY));
	intersection.push_back(Point(endX,endY));
}

string Share::getBinaryParam(string strP, string strGen){
	string binTempP, binP, binGen, binParam, binAlpha, binBeta, binGamma, binTethaInt, binTethaPrec;
	
	binTempP	= Core::str2bin(strP,10,26);
	binGen		= Core::str2bin(strGen,10,26);
	binAlpha	= Core::str2bin(alpha,16,19);
	binBeta		= Core::str2bin(beta,16,19);
	binGamma	= Core::str2bin(gamma,16,19);
	if(index == 1){
		binP		= binTempP.substr(0,13);
	} else {
		if(index == 2){
			binP		= binTempP.substr(13,13);
		}
	}
	binParam	= binAlpha + binBeta + binGamma + binGen + binP;

	return binParam;
}

void Share::embedParam(string binParam){
	int counterParam, flag;
	vector<Mat> channels(4);
	split(share,channels);

	counterParam	= 0;
	flag			= 0;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			int p, a;

			if(binParam[counterParam] == '0'){
				p	= 255;
				a	= 0;
			} else {
				p	= 0;
				a	= 255;
			}

			channels[0].at<uchar>(i,j) = p;
			channels[1].at<uchar>(i,j) = p;
			channels[2].at<uchar>(i,j) = p;
			channels[3].at<uchar>(i,j) = a;

			counterParam++;

			if(counterParam == binParam.length()){
				flag	= 1;
				break;
			}
		}
		if(flag){
			break;
		}
	}

	merge(channels, share);
}

void Share::setShare(){
	char pathS[100];
	vector<Mat> tempChannels(4);
	int flagS	= 0;
	while(flagS == 0){
		cout << "\n> Input S" << index << "'s path : ";
		cin >> pathS;
		share		= imread(pathS, IMREAD_UNCHANGED);
		if(share.data > 0){
			split(share,tempChannels);
			channels	= tempChannels;
			width 		= share.cols;
			height 		= share.rows;
			flagS		= 1;
		} else {
			cout << "\nNo share found" << endl;
		}
	}
}

string Share::getParam(){
	string binParam;
	int counter, flag;

	binParam		= "";
	counter			= 0;
	flag			= 0;

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			int p = channels[0].at<uchar>(i,j);
			if(p < 128){
				binParam.push_back('1');
			} else {
				binParam.push_back('0');
			}

			counter++;

			if(counter == paramLength){
				flag	= 1;
				break;
			}
		}
		if(flag){
			break;
		}
	}

	return binParam;
}

void Share::get3OP(string v1, string v2, string v3){
	mpz_class mpzArea((width/2) * (height/2));
	mpz_class mpzPoint1, mpzPoint2, mpzPoint3;
	mpz_t mpzV1, mpzV2, mpzV3;
	mpz_init_set_str (mpzV1, v1.c_str(), 16);
	mpz_init_set_str (mpzV2, v2.c_str(), 16);
	mpz_init_set_str (mpzV3, v3.c_str(), 16);

	mpz_mod(mpzPoint1.get_mpz_t(), mpzV1, mpzArea.get_mpz_t());
	mpz_mod(mpzPoint2.get_mpz_t(), mpzV2, mpzArea.get_mpz_t());
	mpz_mod(mpzPoint3.get_mpz_t(), mpzV3, mpzArea.get_mpz_t());

	i[0]	= mpzPoint1.get_ui() + 1;
	i[1]	= mpzPoint2.get_ui() + 1;
	i[2]	= mpzPoint3.get_ui() + 1;

	p[0]	= Core::index2point(i[0], (width/2));
	p[1]	= Core::index2point(i[1], (width/2));
	p[2]	= Core::index2point(i[2], (width/2));
}

void Share::getNormal3OP(){
	nP[0] = p[0];
	nP[1] = Point(p[1].x, p[0].y);
	nP[2] = Point(p[0].x, p[2].y);
}

void Share::getShape3OP(){
	if((nP[1].y == nP[0].y)&&(nP[1].x > nP[0].x)&&(nP[2].x == nP[0].x)&&(nP[2].y > nP[0].y)){
		shape3OP = 1;
	} else {
		if((nP[1].y == nP[0].y)&&(nP[1].x < nP[0].x)&&(nP[2].x == nP[0].x)&&(nP[2].y > nP[0].y)){
			shape3OP = 2;
		} else {
			if((nP[1].y == nP[0].y)&&(nP[1].x < nP[0].x)&&(nP[2].x == nP[0].x)&&(nP[2].y < nP[0].y)){
			shape3OP = 3;
			} else {
				if((nP[1].y == nP[0].y)&&(nP[1].x > nP[0].x)&&(nP[2].x == nP[0].x)&&(nP[2].y < nP[0].y)){
					shape3OP = 4;
				}
			}
		}
	}
}

void Share::shape2angle(int opositeShape){
	int diff;

	if(opositeShape == shape3OP){
		angle = 0;
	} else {
		diff = opositeShape - shape3OP;
		if(diff>0){
			angle = diff * 90;
		} else {
			// to produce positive angle
			angle = 360 + (diff * 90);
		}
	}
}