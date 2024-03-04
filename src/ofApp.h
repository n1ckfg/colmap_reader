#pragma once

#include "ofMain.h"
#include "ColmapArray.h"
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
        
        ofEasyCam cam; // add mouse controls for camera movement
        
        Eigen::Vector3d triangulate_simple(const std::vector<Eigen::Vector2d>& points, const std::vector<Eigen::Matrix<double, 3, 4>>& camera_mats);

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
