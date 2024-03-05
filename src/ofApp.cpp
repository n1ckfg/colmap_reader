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

Eigen::Vector3d ofApp::triangulateSimpleEigen(const vector<Eigen::Vector2d>& points, const vector<Eigen::Matrix<double, 3, 4>>& camera_mats) {
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

/*
Eigen::MatrixXd ofApp::triangulatePossibleEigen(const Eigen::Tensor<double, 4>& points, bool undistort = true, int min_cams = 2, bool progress = false, double threshold = 0.5) {
    assert(points.dimension(0) == cameras.size() && "Invalid points shape, first dimension should be equal to number of cameras.");

    int n_cams = static_cast<int>(points.dimension(0));
    int n_points = static_cast<int>(points.dimension(1));
    int n_possible = static_cast<int>(points.dimension(2));

    Eigen::MatrixXd out(n_points, 3);
    Eigen::Tensor<bool, 3> picked_vals(n_cams, n_points, n_possible);
    Eigen::VectorXd errors(n_points);
    Eigen::Tensor<double, 3> points_2d(n_cams, n_points, 2);

    std::unordered_map<int, std::unordered_map<int, std::vector<std::pair<int, int>>>> all_iters;

    for (int cam_num = 0; cam_num < n_cams; ++cam_num) {
        for (int point_num = 0; point_num < n_points; ++point_num) {
            for (int possible_num = 0; possible_num < n_possible; ++possible_num) {
                if (!std::isnan(points(cam_num, point_num, possible_num, 0))) {
                    all_iters[point_num][cam_num].push_back(std::make_pair(cam_num, possible_num));
                }
            }
        }
    }

    for (const auto& it : all_iters) {
        for (const auto& inner_it : it.second) {
            inner_it.second.push_back(std::make_pair(inner_it.first, -1));
        }
    }

    for (int point_ix = 0; point_ix < n_points; ++point_ix) {
        double best_error = 200;
        Eigen::Vector3d best_point;

        int n_cams_max = static_cast<int>(all_iters[point_ix].size());

        for (const auto& picked : cartesian_product(all_iters[point_ix].begin(), all_iters[point_ix].end())) {
            std::vector<std::pair<int, int>> picked_valid;
            for (const auto& p : picked) {
                if (p.second != -1) {
                    picked_valid.push_back(p);
                }
            }

            if (picked_valid.size() < min_cams && picked_valid.size() != n_cams_max) {
                continue;
            }

            std::vector<int> cnums, xnums;
            for (const auto& p : picked_valid) {
                cnums.push_back(p.first);
                xnums.push_back(p.second);
            }

            Eigen::Tensor<double, 3> pts = points.slice(cnums, point_ix, xnums);
            MyTriangulator cc = subset_cameras(cnums);

            Eigen::VectorXd p3d = cc.triangulate(pts, undistort);
            double err = cc.reprojection_error(p3d, pts, true);

            if (err < best_error) {
                best_point = p3d.topRows(3);
                best_error = err;

                if (best_error < threshold) {
                    break;
                }
            }
        }

        if (best_error < 200) {
            out.row(point_ix) = best_point;
            std::vector<std::pair<int, int>> picked_valid;
            for (const auto& p : cartesian_product(all_iters[point_ix].begin(), all_iters[point_ix].end())) {
                if (p.second != -1) {
                    picked_valid.push_back(p);
                }
            }
            for (const auto& p : picked_valid) {
                cnums.push_back(p.first);
                xnums.push_back(p.second);
            }
            picked_vals.slice(cnums, point_ix, xnums) = true;
            errors(point_ix) = best_error;
            points_2d.slice(cnums, point_ix) = points.slice(cnums, point_ix, xnums);
        }
    }

    return out;
}
*/

ofVec3f ofApp::triangulateCentroids(vector<ofVec2f>& centroids, vector<ofVec3f>& positions, vector<ofQuaternion>& rotations) {
    if (centroids.size() == positions.size() && centroids.size() == rotations.size()) {
        vector<Eigen::Vector2d> points;
        vector<Eigen::Matrix<double, 3, 4>> camera_mats;

        for (int i=0; i<centroids.size(); i++) {
            points.push_back(ofToEigenVec2(centroids[i]));
            camera_mats.push_back(convertToMatEigen(ofToEigenVec3(positions[i]), ofToEigenQuat(rotations[i])));
        }
        
        Eigen::Vector3d p = triangulateSimpleEigen(points, camera_mats);
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
