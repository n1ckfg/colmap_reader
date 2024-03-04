#pragma once

#include "ofMain.h"
#include "ColmapArray.h"

class ofApp : public ofBaseApp {
    
	public:
		void setup();
		void update();
		void draw();
        
        ColmapArray colmapArray;
        vector<ofVec2f> centroids;
    
        ofEasyCam cam; // add mouse controls for camera movement

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
