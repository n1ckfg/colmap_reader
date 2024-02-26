#include "ColmapCamera.h"

ColmapCamera::ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> _params) {
    camera_id = _camera_id;
    model = _model;
    width = _width;
    height = _height;
    params = _params;
    init();
}

ColmapCamera::ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> _params, vector<ofQuaternion> _rotations, vector<ofVec3f> _positions) {
    camera_id = _camera_id;
    model = _model;
    width = _width;
    height = _height;
    params = _params;
    rotations = _rotations;
    positions = _positions;
    init();
}

void ColmapCamera::init() {
    if (model == "OPENCV") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
        k1 = params[4];
        k2 = params[5];
        p1 = params[6];
        p2 = params[7];
    } else if (model == "PINHOLE") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
    } else if (model == "RADIAL") {
        f = params[0];
        cx = params[1];
        cy = params[2];
        k1 = params[3];
        k2 = params[4];
    } else if (model == "SIMPLE_RADIAL") {
        f = params[0];
        cx = params[1];
        cy = params[2];
        k = params[3];
    } else if (model == "FULL_OPENCV") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
        k1 = params[4];
        k2 = params[5];
        p1 = params[6];
        p2 = params[7];
        k3 = params[8];
        k4 = params[9];
        k5 = params[10];
        k6 = params[11];
    } else if (model == "OPENCV_FISHEYE") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
        k1 = params[4];
        k2 = params[5];
        k3 = params[6];
        k4 = params[7];
    } else if (model == "SIMPLE_RADIAL_FISHEYE") {
        f = params[0];
        cx = params[1];
        cy = params[2];
        k = params[3];
    } else if (model == "RADIAL_FISHEYE") {
        f = params[0];
        cx = params[1];
        cy = params[2];
        k1 = params[3];
        k2 = params[4];
    } else if (model == "FOV") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
        omega = params[4];
    } else if (model == "THIN_PRISM_FISHEYE") {
        fx = params[0];
        fy = params[1];
        cx = params[2];
        cy = params[3];
        k1 = params[4];
        k2 = params[5];
        p1 = params[6];
        p2 = params[7];
        k3 = params[8];
        k4 = params[9];
        sx1 = params[10];
        sy1 = params[11];
    } else {
        model = "SIMPLE_PINHOLE";
        f = params[0];
        cx = params[1];
        cy = params[2];
    }
}

/*
Use these camera models if your images are undistorted a priori. 
SIMPLE_PINHOLE: f, cx, cy
PINHOLE: fx, fy, cx, cy

This should be the camera model of choice if the intrinsics are unknown and
every image has a different camera calibration, e.g., in the case of Internet
photos. 
SIMPLE_RADIAL: f, cx, cy, k
RADIAL: f, cx, cy, k1, k2

Use these camera models, if you know the calibration parameters a priori. You
can also try to let COLMAP estimate the parameters, if you share the intrinsics
for multiple images. Note that the automatic estimation of parameters will most
likely fail, if every image has a separate set of intrinsic parameters. 
OPENCV: fx, fy, cx, cy, k1, k2, p1, p2
FULL_OPENCV: fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, k5, k6

Use these camera models for fisheye lenses and note that all other models are
not really capable of modeling the distortion effects of fisheye lenses.
SIMPLE_RADIAL_FISHEYE: f, cx, cy, k 
RADIAL_FISHEYE: f, cx, cy, k1, k2
OPENCV_FISHEYE: fx, fy, cx, cy, k1, k2, k3, k4 
FOV: fx, fy, cx, cy, omega
THIN_PRISM_FISHEYE: fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, sx1, sy1
*/
