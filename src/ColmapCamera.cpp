#include "ColmapCamera.h"

ColmapCamera::ColmapCamera(int _camera_id, string _model, int _width, int _height, vector<float> _params) {
    camera_id = _camera_id;
    model = _model;
    width = _width;
    height = _height;
    params = _params;
}
