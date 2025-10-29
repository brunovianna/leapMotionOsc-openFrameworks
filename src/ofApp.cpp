#include "ofApp.h"

// helper: build a wireframe cube centered at origin
static ofMesh makeWireCube(float size) {
	ofMesh m;
	m.setMode(OF_PRIMITIVE_LINES);
	float h = size * 0.5f;

	// 8 vertices
	vector<glm::vec3> v = {
		{-h, -h, -h}, { h, -h, -h}, { h,  h, -h}, {-h,  h, -h},
		{-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h}
	};
	for (auto &p : v) m.addVertex(p);

	// edges (each pair is a line)
	int edges[][2] = {
		{0,1},{1,2},{2,3},{3,0}, // back face
		{4,5},{5,6},{6,7},{7,4}, // front face
		{0,4},{1,5},{2,6},{3,7}  // connecting edges
	};
	for (auto &e : edges) {
		m.addIndex(e[0]);
		m.addIndex(e[1]);
	}

	return m;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("oscReceiveExample");
	ofSetFrameRate(60); // run at 60 fps
	ofSetVerticalSync(true);

	// listen on the given port
	ofLog() << "listening for osc messages on port " << PORT;
	receiver.setup(PORT);

	// initialize hand meshes (wire cubes)
	rHandMesh = makeWireCube(handMeshScale);
	lHandMesh = makeWireCube(handMeshScale);

	// set default orientation nodes (identity)
	rHandNode.setPosition(0, 0, 0);
	lHandNode.setPosition(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

	// check for waiting messages
	while (receiver.hasWaitingMessages()) {

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// check for right hand message
		if (m.getAddress() == "/hand/right/position") {

			rHandXf = m.getArgAsFloat(0);
			rHandYf = m.getArgAsFloat(1);
			rHandZf = m.getArgAsFloat(2);
		}
		// check for left hand
		else if (m.getAddress() == "/hand/left/position") {

			lHandXf = m.getArgAsFloat(0);
			lHandYf = m.getArgAsFloat(1);
			lHandZf = m.getArgAsFloat(2);

		//check for  orientation as quaternion [w, x, y, z]
		} else if (m.getAddress() == "/hand/right/orientation") {
			rHandOrientationWf = m.getArgAsFloat(0);
			rHandOrientationXf = m.getArgAsFloat(1);
			rHandOrientationYf = m.getArgAsFloat(2);
			rHandOrientationZf = m.getArgAsFloat(3);
		} else if (m.getAddress() == "/hand/left/orientation") {
			lHandOrientationWf = m.getArgAsFloat(0);
			lHandOrientationXf = m.getArgAsFloat(1);
			lHandOrientationYf = m.getArgAsFloat(2);
			lHandOrientationZf = m.getArgAsFloat(3);
		} else {

			// unrecognized message: display on terminal
			string msgString;
			msgString = m.getAddress();
			msgString += ":";
			for (size_t i = 0; i < m.getNumArgs(); i++) {

				// get the argument type
				msgString += " ";
				msgString += m.getArgTypeName(i);
				msgString += ":";

				// display the argument - make sure we get the right type
				if (m.getArgType(i) == OFXOSC_TYPE_INT32) {
					msgString += ofToString(m.getArgAsInt32(i));
				} else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
					msgString += ofToString(m.getArgAsFloat(i));
				} else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
					msgString += m.getArgAsString(i);
				} else {
					msgString += "unhandled argument type " + m.getArgTypeName(i);
				}
			}

			// add to the list of strings to display
			ofLogNotice() << msgString;

		}
	}

	// Apply latest position + orientation to nodes

	
	ofQuaternion rq(rHandOrientationXf, rHandOrientationYf, -rHandOrientationZf, rHandOrientationWf); // invert Z to match oF coordinate system
	rq.normalize();
	rHandNode.setOrientation(rq);
	rHandNode.setPosition(rHandXf, -rHandYf, rHandZf); // invert Y to match oF coordinate system
	
	
	ofQuaternion lq(lHandOrientationXf, lHandOrientationYf, -lHandOrientationZf, lHandOrientationWf); // invert Z to match oF coordinate system
	lq.normalize();
	lHandNode.setOrientation(lq);
	lHandNode.setPosition(lHandXf, -lHandYf, lHandZf); // invert Y to match oF coordinate system



}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(20);
	ofEnableDepthTest();

	// draw a simple origin axis to help orient
	ofPushStyle();
	ofSetColor(150);
	ofSetLineWidth(1);
	ofDrawAxis(40);
	ofPopStyle();

	ofScale(2, 2, 2); // scale up the scene a bit
	ofTranslate(300, 300, 0); // shift the whole scene to see better (since the origin is at top-left)

	// Draw right hand mesh

	ofSetColor(100, 255, 200);
	ofSetLineWidth(2);
	rHandNode.transformGL(); // apply position + orientation
	rHandMesh.draw();
	rHandNode.restoreTransformGL();

	// Draw left hand mesh
	ofSetColor(255, 100, 100);
	ofSetLineWidth(2);
	lHandNode.transformGL();
	lHandMesh.draw();
	lHandNode.restoreTransformGL();

	ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
