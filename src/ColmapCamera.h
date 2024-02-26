#pragma once

#include "ofMain.h"

class ColmapCamera {
    
    public:
        ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> params);
    
        int camera_id;
        string model;
        int width;
        int height;
        vector<float> params;
    
};
