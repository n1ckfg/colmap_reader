#include "ColmapArray.h"

ColmapArray::ColmapArray() {
    loadColmapCameras(ofToDataPath("cameras.txt"));
    loadColmapImages(ofToDataPath("images.txt"));
}

ColmapArray::ColmapArray(string camerasFile, string imagesFile) {
    loadColmapCameras(camerasFile);
    loadColmapImages(imagesFile);
}

void ColmapArray::loadColmapCameras(string filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ofLogError() << "Could not open file: " << filePath;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            //ofLog() << "Input: " << line;
            
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

void ColmapArray::loadColmapImages(string filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        ofLogError() << "Could not open file: " << filePath;
        return;
    }
    
    int lineCounter = 1;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            //ofLog() << "Input: " << line;
            
            vector<string> lineArray = ofSplitString(line, " ");
            
            if (ofToInt(lineArray[0]) == lineCounter) {
                int camera_id = ofToInt(lineArray[8]);
                
                float qx = ofToFloat(lineArray[2]);
                float qy = ofToFloat(lineArray[3]);
                float qz = ofToFloat(lineArray[4]);
                float qw = ofToFloat(lineArray[1]);
                
                float tx = ofToFloat(lineArray[5]);
                float ty = ofToFloat(lineArray[7]);
                float tz = ofToFloat(lineArray[6]);
                
                ofQuaternion rot = ofQuaternion(qx, qy, qz, qw);
                ofVec3f pos = ofVec3f(tx, ty, tz);
                
                for (int i=0; i<cameras.size(); i++) {
                    if (camera_id == cameras[i].camera_id) {
                        cameras[i].rotations.push_back(rot);
                        cameras[i].positions.push_back(pos);
                    }
                }

                ofLog() << "Found rot " << rot << ", pos " << pos << " for camera " << camera_id << ".";
                
                lineCounter++;
            }
        }
    }

    file.close();
}

