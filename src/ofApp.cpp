#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    loadColmapFile(ofToDataPath("cameras.txt"));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

void ofApp::loadColmapFile(string filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ofLogError() << "Could not open file: " << filePath;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            ofLog() << "Input: " << line;
            vector<string> lineArray = ofSplitString(line, " ");

            int camera_id = ofToInt(lineArray[0]);
            string model = lineArray[1];
            int width = ofToInt(lineArray[2]);
            int height = ofToInt(lineArray[3]);
            vector<float> params;
            for (int i=4; i<lineArray.size(); i++) {
                params.push_back(ofToFloat(lineArray[i]));
            }
            
            ColmapCamera camera = ColmapCamera(camera_id, model, width, height, params);
            cameras.push_back(camera);
            ofLog() << "Found " << cameras.size() << " camera(s).";
        }
    }

    file.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
