#pragma once

#include "ofMain.h"
#include "ColmapArray.h"


//#include <Eigen/Dense>
#include "/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3/Eigen/Dense"

class ofApp : public ofBaseApp {
    
	public:
		void setup();
		void update();
		void draw();
        
        ColmapArray colmapArray;
        vector<ofVec2f> centroids;
        vector<ofVec3f> positions;
        vector<ofQuaternion> rotations;
        ofVec3f testPoint;
        float globalScale = 20.0;
        float dotSize = 10.0;
    
        ofEasyCam cam; // add mouse controls for camera movement
        
        ofVec3f triangulateCentroids(vector<ofVec2f>& centroids, vector<ofVec3f>& positions, vector<ofQuaternion>& rotations);
    
        Eigen::Vector3d triangulateEigen(const vector<Eigen::Vector2d>& points, const vector<Eigen::Matrix<double, 3, 4>>& camera_mats);
        Eigen::Matrix<double, 3, 4> convertToMatEigen(const Eigen::Vector3d& translation, const Eigen::Quaterniond& quaternion);
        
        Eigen::Vector2d ofToEigenVec2(ofVec2f input);
        Eigen::Vector3d ofToEigenVec3(ofVec3f input);
        Eigen::Quaterniond ofToEigenQuat(ofQuaternion input);
        
        ofVec2f eigenToOfVec2(Eigen::Vector2d input);
        ofVec3f eigenToOfVec3(Eigen::Vector3d input);
        ofQuaternion eigenToOfQuat(Eigen::Quaterniond input);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
};
