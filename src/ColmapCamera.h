#pragma once

#include "ofMain.h"

class ColmapCamera {
    
    public:
        ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> _params);

        ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> _params, vector<ofQuaternion> _rotations, vector<ofVec3f> _positions);

        void init();
    
        int camera_id;
        string model;
        int width;
        int height;
        vector<float> params;
        vector<ofQuaternion> rotations;
        vector<ofVec3f> positions;
    
        //f = focal length
        float f = 0.0;
        float fx = 0.0;
        float fy = 0.0;
        //c = principal point
        float cx = 0.0;
        float cy = 0.0;
        //k = radial distortion
        float k = 0.0;
        float k1 = 0.0;
        float k2 = 0.0;
        float k3 = 0.0;
        float k4 = 0.0;
        float k5 = 0.0;
        float k6 = 0.0;
        //p = tangential distortion
        float p1 = 0.0;
        float p2 = 0.0;
        //s = thin-prism distortion
        float sx1 = 0.0;
        float sy1 = 0.0;
        //omega = field of view angle
        float omega = 0.0;

};
