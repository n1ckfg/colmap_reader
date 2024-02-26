#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    colmapArray = ColmapArray();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    cam.begin();
    ofEnableDepthTest();
    ofDrawGrid(20, 10, true, true, true, true);
    
    for (int i=0; i<colmapArray.cameras.size(); i++) {
        ColmapCamera& ccam = colmapArray.cameras[i];
        
        if (ccam.positions.size() == ccam.rotations.size()) {
            for (int j=0; j<ccam.positions.size(); j++) {
                ofPushMatrix();
                ofTranslate(ccam.positions[j] * 50.0);
                
                ofMatrix4x4 rotMatrix;
                ccam.rotations[j].get(rotMatrix);
                ofMultMatrix(rotMatrix);
                
                ofDrawBox(10);
                
                ofPopMatrix();
            }
        }
    }
    
    ofDisableDepthTest();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
