#include "ofApp.h"
#include <random>

#define ma_mod(a, n) ((a%n)+n)%n //need this weird mod function to deal with negatives
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	//initialize transition variables
	trans_ON = 0;
	max_diff = 0;
	pp = 0;

	//choose transition function here, 'Add', 'mul5', or 'sqr'
	trans = 'Add';

	W = ofGetWidth(); 
	H = ofGetHeight();

	//work with just one image or multiple
	MODE = "One"; //"One" or "Multiple"
	string filename = "gate.jpg";

	//load images
	if (MODE == "Multiple") {
		ofDirectory dir("data");
		dir.listDir();
		D = dir.size();
		for (int i = 15; i < dir.size(); i++) {
			string name = dir.getName(i);
			ofImage imgi;
			imgi.load(name);
			imgi.resize(W, H);
			images.push_back(imgi);
		}

		img_idx = 1;
		img = images.at(img_idx);
	}
	else if (MODE == "One") {
		img.load(filename);
		img.resize(W, H);
	}
	
	src = img.getPixels().getData(); //initialize array with pixel data


	constructTable(); //create table for multiplicative transitions

	m = 1; //for lemma14()
}

//--------------------------------------------------------------
void ofApp::update(){
	if (trans_ON == 1) {
		TDS(); //do transition
	}
	else {
		UDS(); //do update function
	}
	//update images with new pixels
	img.update();
	img2.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	img.draw(0, 0, W, H); //draw image to screen
}

void ofApp::UDS() {
	/*
	* Apply update function to each pixel
	*/
	for (int x = 0; x < W; x ++) {
		for (int y = 0; y < H; y ++) {
			col_idx = 3*(y * W + x);

			//current pixel values
			r = src[col_idx]; 
			g = src[col_idx + 1];
			b = src[col_idx + 2];


			//create params here
			char addparams[3] = { 10, -6, 15};
			char multparams[3] = { 2,3, 201 };
			char matparams[3][3] = { {3,4,6},{10,7,12},{2,44,9} };
			//niceMatrix(matrix1);
			
			//set update function here by uncommenting one or more functions
			//add(addparams);
			//Matmul(matparams);
			//mult(multparams);
			//affine();
			//period();
			//lemma14();
			//circles();
			//diamond();
			//porabs();

			//update pixel
			src[col_idx] = r;
			src[col_idx + 1] = g;
			src[col_idx + 2] = b;
		}
	}
	
}

void ofApp::TDS() {
	max_diff = 0;
	//pp is for preprocessing before transition
	if (pp == 1) {
		img2 = images.at(img_idx + 1);
		next = img2.getPixels().getData();
		if (trans == 'mul5') {
			MakeODD(src);
			MakeODD(next);
			flyp(src, next);
		}
		else if (trans == 'sqr') {
			MakeEVEN(src);
			MakeEVEN(next);
		}
		pp = 0;
	}
	chooseT();
}

//transition preprocessing
void ofApp::MakeODD(unsigned char* arr) {
	//make all colors odd
	for (int i = 0; i < H * W * 3; i++) {
		arr[i] += (arr[i] + 1) % 2;
	}
}

void ofApp::MakeEVEN(unsigned char* arr) {
	//make all colors even
	for (int i = 0; i < H * W * 3; i++) {
		arr[i] += (arr[i]) % 2;
	}
}

void ofApp::flyp(unsigned char* arr1, unsigned char* arr2) {
	//multiply color by -1 if it is not generated by 5 in Z_256
	for (int x = 0; x < H * W * 3; x++) {
		int i = arr1[x];
		int j = arr2[x];
		if (steps[i][j] == -1) {
			arr1[x] = 256 - arr1[x];
		}
	}
}

void ofApp::constructTable() {
	int m;
	//figure out which numbers are generated by 5 vs -5 in Z_256
	for (int n = 0; n < 256; n++) {
		m = 256 - n;
		for (int k = 1; k < 65; k++) {
			m = (m * 5) % 256;
			if (steps[n][m] == 0) {
				steps[n][m] = -1;
			}
		}
	}
}

//transition sequence
void ofApp::chooseT() {
	//transition function applied to each pixel
	for (int x = 0; x < (H * W * 3); x++) {
		col_idx = x;

		if (faulty_merge == true) {
			addT(1);
		}
		else if (trans == 'Add') {
			//set parameter for additive transition here
			addT(2);
		}
		else if (trans == 'mul5') {
			//set parameter for multiplicative transition here
			multT(3);
		}
		else if (trans == 'sqr') {
			sqrdiff();
		}

	}
	if (max_diff == 0) {
		//transition is over
		trans_ON = 0;
		faulty_merge = false;
		pp = 1;
		img_idx = (img_idx + 1) % D;
	}

}

//transition functions
void ofApp::addT(int a) {
	int diff = abs(next[col_idx] - src[col_idx]);
	if (diff > max_diff) {
		max_diff = diff;
	}
	if (diff != 0) {
		src[col_idx] = ma_mod(src[col_idx] + a, 256);
	}
}

void ofApp::multT(int m) {
	int diff = abs(next[col_idx] - src[col_idx]);
	if (diff > max_diff) {
		max_diff = diff;
	}
	if (diff != 0) {
		src[col_idx] = ma_mod(src[col_idx] * m, 256);
	}
}

void ofApp::sqrdiff() {
	int diff = ma_mod((next[col_idx] - src[col_idx]),256);
	int fudge = ma_mod((src[col_idx] + 2 * next[col_idx]), 256);
	if (abs(diff) > max_diff) {
		max_diff = abs(diff);
	}
	if (diff != 0) {
		src[col_idx] = ma_mod((diff*diff) + src[col_idx], 256);
	}
}


//-----Update functions
void ofApp::add(char params[3]) {

	r = ma_mod(r + params[0], 256);
	g = ma_mod(g + params[1], 256);
	b = ma_mod(b + params[2], 256);

}

void ofApp::mult(char params[3]) {

	r = ma_mod((r * params[0]),256);
	g = ma_mod((g * params[1]), 256);
	b = ma_mod((b * params[2]), 256);

}


void ofApp::niceMatrix(char params[3][3]) {
	//make matrix A and I-A both invertible
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			params[i][j] = ma_mod(params[i][j] * (-8), 256);
			if (i == j) {
				params[i][j] = ma_mod(params[i][j] + 3, 256);
			}
		}
	}
}

void ofApp::Matmul(char params[3][3]) {
	char col[3] = { r, g, b };
	char newcol[3];
	for (int i = 0; i < 3; i++) {
		newcol[i] = 0;
		for (int j = 0; j < 3; j++) {
			newcol[i] = ma_mod((newcol[i] + (params[i][j] * col[j])), 256);
		}
	}
	r = newcol[0];
	g = newcol[1];
	b = newcol[2];
}

void ofApp::affine() {
	int znullcline = r * r + g * g + b*b - 100; //equation describing sphere of radius 100
	char newcol[3];
	newcol[0] = ma_mod(r + 1, 256);
	newcol[1] = ma_mod(g + znullcline, 256);
	newcol[2] = ma_mod(b - 3, 256);

	r = newcol[0];
	g = newcol[1];
	b = newcol[2];
}

void ofApp::period() {
	//have to check in alggeom book to name this
	int rtraj = 255 * cos(b / 255);
	int gtraj = 255 * sin(b / 255);
	r = ma_mod(r + rtraj, 256);
	g = ma_mod(g + gtraj, 256);
	b = ma_mod(b + 1,256);
}

void ofApp::lemma14() {
	//I do not remember where this is from or why it's called lemma 14 but it's pretty cool
	r = ma_mod(m * r + 1, 256);
	g = ma_mod(m * g + 1, 256);
	b = ma_mod(m * b + 1, 256);
	m += 2;
}

void ofApp::circles(int r1 = 100, int r2 = 100, int r3 = 100) {
	//should include params for radii
	char newcol[3];
	newcol[0] = ma_mod(g*g + b*b - r1 + r, 256);
	newcol[1] = ma_mod(r*r + b*b - r2 + g, 256);
	newcol[2] = ma_mod(g*g + r*r - r3 + b, 256);
	r = newcol[0];
	g = newcol[1];
	b = newcol[2];

}

void ofApp::diamond() {
	if ((r >= 128 and g < 128) or (r < 128 and g >= 128)) {
		r = ma_mod(r + 1, 256);
		g = ma_mod(g + 1, 256);
	}
	else {
		r = ma_mod(r - 1, 256);
		g = ma_mod(g - 1, 256);
	}
}

void ofApp::porabs() {
	int rnullcline = (-1) * (r - 128) * (r - 128);
	int gnullcline = (-1) * (r - 128) * (r + 128);
	int newr = ma_mod(rnullcline + r, 256);
	g = ma_mod(gnullcline + g, 256);
	r = newr;

}


//-----Interfacing events-----//
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (trans_ON == 0) {
		trans_ON = 1;
		pp = 1;
		m = 1;
	}
	else {
		trans_ON = 0;
		pp = 1;
		img_idx = (img_idx + 1) % D;
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	faulty_merge = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

