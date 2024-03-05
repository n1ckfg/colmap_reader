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
                float x = ofToFloat(centroidString[0]) / 640.0;
                float y = ofToFloat(centroidString[1]) / 480.0;
                centroids.push_back(ofVec2f(x, y));
            }
        }
    }   
    
    positions = colmapArray.cameras[0].positions;
    rotations = colmapArray.cameras[0].rotations;
    testPoint = triangulateCentroids(centroids, positions, rotations) * globalScale;
}

Eigen::Vector2d ofApp::ofToEigenVec2(ofVec2f input) {
    return Eigen::Vector2d(input.x, input.y);
}

Eigen::Vector3d ofApp::ofToEigenVec3(ofVec3f input) {
    return Eigen::Vector3d(input.x, input.y, input.z);
}

Eigen::Quaterniond ofApp::ofToEigenQuat(ofQuaternion input) {
    return Eigen::Quaterniond(input.x(), input.y(), input.z(), input.w());
}

ofVec2f ofApp::eigenToOfVec2(Eigen::Vector2d input) {
    return ofVec2f(input.x(), input.y());
}

ofVec3f ofApp::eigenToOfVec3(Eigen::Vector3d input) {
    return ofVec3f(input.x(), input.y(), input.z());
}

ofQuaternion ofApp::eigenToOfQuat(Eigen::Quaterniond input) {
    return ofQuaternion(input.x(), input.y(), input.z(), input.w());
}

Eigen::Matrix<double, 3, 4> ofApp::convertToMatEigen(const Eigen::Vector3d& translation, const Eigen::Quaterniond& quaternion) {
    Eigen::Matrix<double, 3, 4> transformationMatrix;
    transformationMatrix.block<3, 3>(0, 0) = quaternion.toRotationMatrix();
    transformationMatrix.block<3, 1>(0, 3) = translation;
    //transformationMatrix.row(3) << 0, 0, 0, 1;
    return transformationMatrix;
}

Eigen::Vector3d ofApp::triangulateEigen(const vector<Eigen::Vector2d>& points, const vector<Eigen::Matrix<double, 3, 4>>& camera_mats) {
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

ofVec3f ofApp::triangulateCentroids(vector<ofVec2f>& centroids, vector<ofVec3f>& positions, vector<ofQuaternion>& rotations) {
    if (centroids.size() == positions.size() && centroids.size() == rotations.size()) {
        vector<Eigen::Vector2d> points;
        vector<Eigen::Matrix<double, 3, 4>> camera_mats;

        for (int i=0; i<centroids.size(); i++) {
            points.push_back(ofToEigenVec2(centroids[i]));
            camera_mats.push_back(convertToMatEigen(ofToEigenVec3(positions[i]), ofToEigenQuat(rotations[i])));
        }
        
        Eigen::Vector3d p = triangulateEigen(points, camera_mats);
        return eigenToOfVec3(p);
    } else {
        return ofVec3f();
    }
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
                ofVec3f p = ccam.positions[j] * globalScale;
                ofPushMatrix();
                ofTranslate(p);
                
                ofMatrix4x4 rotMatrix;
                ccam.rotations[j].get(rotMatrix);
                ofMultMatrix(rotMatrix);
                
                ofSetColor(255);
                ofDrawBox(dotSize);
                ofPopMatrix();
                ofDrawLine(p, testPoint);
            }
        }
    }
    
    ofPushMatrix();
    ofTranslate(testPoint);
    ofSetColor(255,255,0);
    ofDrawBox(dotSize);
    ofPopMatrix();
    
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
