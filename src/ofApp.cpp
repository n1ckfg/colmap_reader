#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    colmapArray = ColmapArray();
    
    string filePath = ofToDataPath("test_centroids.txt");
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ofLogError() << "Could not open file: " << filePath;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            vector<string> centroidsString = ofSplitString(line, ",");
            for (int i=0; i<centroidsString.size(); i++) {
                vector<string> centroidString = ofSplitString(centroidsString[i], " ");
                float x = ofToFloat(centroidString[0]);
                float y = ofToFloat(centroidString[1]);
                centroids.push_back(ofVec2f(x, y));
            }
        }
    }   
}

Eigen::Vector3d triangulate_simple(const std::vector<Eigen::Vector2d>& points, const std::vector<Eigen::Matrix<double, 3, 4>>& camera_mats) {
    int num_cams = camera_mats.size();
    Eigen::Matrix<double, Eigen::Dynamic, 4> A(num_cams * 2, 4);

    for (int i = 0; i < num_cams; ++i) {
        double x = points[i](0);
        double y = points[i](1);
        Eigen::Matrix<double, 3, 4> mat = camera_mats[i];

        A.block(i * 2, 0, 1, 4) = x * mat.row(2) - mat.row(0);
        A.block(i * 2 + 1, 0, 1, 4) = y * mat.row(2) - mat.row(1);
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Vector4d p3d_homogeneous = svd.matrixV().col(3);
    Eigen::Vector3d p3d = p3d_homogeneous.head(3) / p3d_homogeneous(3);

    return p3d;
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
                ofDrawLine(0.0, 0.0, 0.0, 0.0, -200.0, 0.0);
                
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
