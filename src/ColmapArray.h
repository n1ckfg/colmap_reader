#pragma once

#include "ofMain.h"
#include "ColmapCamera.h"

class ColmapArray {
    
    public:
        ColmapArray();
        ColmapArray(string camerasFile, string imagesFile);
    
        void loadColmapCameras(string filePath);
        void loadColmapImages(string filePath);
        vector<ColmapCamera> cameras;
    
};
