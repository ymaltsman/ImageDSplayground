#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void TDS();
		void UDS();

		//preprocessing functions for transition
		void MakeODD(unsigned char* arr);
		void MakeEVEN(unsigned char* arr);
		void flyp(unsigned char* arr1, unsigned char* arr2);
		void constructTable();
		void cluster();
		bool avalanche();
		void avalancheGO(int idx);
		bool eqcolors(char arr1[3], char arr2[3]);
		void operatesandpile();

		void chooseT(); //choose transition function
		void addT(int a); //additive transition
		void mult5(); //multiplicative transition
		void sqrdiff();

		//update functions
		void add(char params[3]);
		void mult(char params[3]);
		void niceMatrix(char params[3][3]);
		void Matmul(char params[3][3]);
		void signswitch();
		void rotate();
		void rotxy();
		void square();
		void conv(char params[3]);
		void affine();
		void wehd();
		void lemma14();

	bool trans_ON; //set transition
	bool pp; // alter images for multiplicative transition
	bool faulty_merge;
	int max_diff; //tracks difference between current and next image
	int trans; //for choosing transition function
	int col_idx; //track index of pixel in image
	int img_idx; //track index of image in directory
	int H, W; //size of screen
	int D; //size of directory
	vector<ofImage> images; //vector of images
	ofImage img, img2; //current and next image
	unsigned char* src;
	unsigned char* next; //pixel data of current, next image
	int r, g, b; //current pixel value
	int steps[256][256];//table for multiplicative order
	int mousex, mousey; //mouseclick
	int m; //for lemma14()
	string MODE;
	char clusters[5][3];
};
