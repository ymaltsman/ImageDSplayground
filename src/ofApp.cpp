#include "ofApp.h"

#define ma_mod(a, n) ((a%n)+n)%n
//--------------------------------------------------------------
void ofApp::setup(){
	glEnable(GL_DEPTH_TEST);
	ofSetFrameRate(20);
	trans_ON = 0;
	max_diff = 0;
	pp = 0;
	//choose transition function here, 'Add' or 'mul5'
	trans = 'sqr';

	W = ofGetWidth();
	H = ofGetHeight();

	//load images
	ofDirectory dir("data");
	dir.listDir();
	D = dir.size();
	for (int i = 0; i < dir.size(); i++) {
		string name = dir.getName(i);
		ofImage imgi;
		imgi.load(name);
		imgi.resize(W, H);
		images.push_back(imgi);
	}

	img_idx = 1;
	img = images.at(img_idx);

	src = img.getPixels().getData();

	constructTable(); //create table for multiplicative transitions
	m = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (trans_ON == 1) {
		TDS();
	}
	else {
		UDS();
	}
	img.update();
	img2.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	img.draw(0, 0, W, H);
	//int idx = 5;
	//float x = src[idx];
	//float y = src[idx + 1];
	//float z = src[idx + 2];
	//ofDrawSphere(ofPoint(x + W, y, z), 10);
	
}

void ofApp::UDS() {
	for (int x = 0; x < W*3; x += 3) {
		for (int y = 0; y < H*3; y += 3) {
			col_idx = (y * W) + (x);

			//current pixel values
			r = src[col_idx]; 
			g = src[col_idx + 1];
			b = src[col_idx + 2];

			//create params here
			char vector1[3] = { 0, 0,  10};
			char vector2[3] = { 3,1, 1 };
			char matrix1[3][3] = { {1,9,0},{0,1,1},{-2,0,1} };


			//set update function here by uncommenting one or more functions
			//Matmul(matrix1);
			add(vector1);
			//mult(vector2);
			//affine();
			//wehd();
			//lemma14();
		}
	}
}

void ofApp::TDS() {
	max_diff = 0;
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
	for (int i = 0; i < H * W * 3; i++) {
		arr[i] += (arr[i] + 1) % 2;
	}
}

void ofApp::MakeEVEN(unsigned char* arr) {
	for (int i = 0; i < H * W * 3; i++) {
		arr[i] += (arr[i]) % 2;
	}
}

void ofApp::flyp(unsigned char* arr1, unsigned char* arr2) {

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
	for (int x = 0; x < (H * W * 3); x++) {
		col_idx = x;

		if (trans == 'Add') {
			addT();
		}
		if (trans == 'mul5') {
			mult5();
		}
		if (trans == 'sqr') {
			sqrdiff();
		}

	}
	if (max_diff == 0) {
		trans_ON = 0;
		img_idx = (img_idx + 1) % D;
	}

}

//transition functions
void ofApp::addT() {
	int diff = abs(next[col_idx] - src[col_idx]);
	if (diff > max_diff) {
		max_diff = diff;
	}
	if (diff != 0) {
		src[col_idx] = (src[col_idx] + 1) % 256;
	}
}

void ofApp::mult5() {
	int diff = abs(next[col_idx] - src[col_idx]);
	if (diff > max_diff) {
		max_diff = diff;
	}
	if (diff != 0) {
		src[col_idx] = (src[col_idx] * 5) % 256;
	}
}

void ofApp::sqrdiff() {
	int diff = ma_mod((next[col_idx] - src[col_idx]),256);
	int fudge = ma_mod((src[col_idx] + 2 * next[col_idx]), 256);
	if (abs(diff) > max_diff) {
		max_diff = abs(diff);
	}
	if (diff != 0) {
		src[col_idx] = ma_mod(-1 * (fudge * fudge - src[col_idx] * src[col_idx]) + src[col_idx], 256);
	}
}


//-----Update functions
void ofApp::add(char params[3]) {

	src[col_idx] = (r + params[0]) % 256;
	src[col_idx + 1] = (g + params[1]) % 256;
	src[col_idx + 2] = (b + params[2]) % 256;
}

void ofApp::mult(char params[3]) {

	src[col_idx] = ma_mod((r * params[0]),256);
	src[col_idx + 1] = ma_mod((g * params[1]), 256);
	src[col_idx + 2] = ma_mod((b * params[2]), 256);

}

void ofApp::Matmul(char params[3][3]) {
	char col[3] = { r, g, b };

	for (int i = 0; i < 3; i++) {
		src[col_idx + i] = 0;
		for (int j = 0; j < 3; j++) {
			src[col_idx + i] = ma_mod((src[col_idx + i] + (params[i][j] * col[j])), 256);
		}
	}
}

void ofApp::affine() {
	int xynullcline = r * r + g * g + b*b - 1;
	int znullcline = b - r * r - g * g;
	src[col_idx] = ma_mod(r + xynullcline, 256);
	src[col_idx+1] = ma_mod(g + xynullcline, 256);
	src[col_idx + 2] = ma_mod(b + xynullcline, 256);

}

void ofApp::wehd() {
	int rtraj = 255 * cos(b / 255);
	int gtraj = 255 * sin(b / 255);
	src[col_idx] = ma_mod(r + rtraj, 256);
	src[col_idx + 1] = ma_mod(g + gtraj, 256);
	src[col_idx+2] = ma_mod(b + 1,256);
}

void ofApp::lemma14() {
	src[col_idx] = ma_mod(m * r + 1, 256);
	src[col_idx + 1] = ma_mod(m * g + 1, 256);
	src[col_idx + 2] = ma_mod(m * b + 1, 256);
	m+= 2;
}

//-----Interfacing events-----//
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	trans_ON = 1;
	pp = 1;
	m = 1;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	mousex = x;
	mousey = y;
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
