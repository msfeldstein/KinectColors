#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    feedbackBuffer.allocate(ofGetWidth(), ofGetHeight());
    feedbackBuffer.bind();
    ofClear(0, 0, 0);
    feedbackBuffer.unbind();
    person.loadImage("person.png");
    hueShader.load("shader.vert","shader.frag");
    depthPlayer.loadMovie("OneHandDepth.mov");
    depthPlayer.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    depthPlayer.update();
    feedbackBuffer.bind();
    ofPushMatrix();
    float scaleAmount = 0.02;
    ofScale(1.0 + scaleAmount, 1.0 + scaleAmount);
    ofScale(1.0, -1.0);
    float dx =-ofGetWidth() * scaleAmount / 2.0;
    float dy = -ofGetHeight() * (1.0 - scaleAmount / 2.0);
    ofTranslate(dx, dy);
    hueShader.begin();
    feedbackBuffer.draw(0, 0);
    hueShader.end();
    ofPopMatrix();
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    depthPlayer.draw(0, 0);
    ofDisableBlendMode();
    ofPopMatrix();
    feedbackBuffer.unbind();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofScale(1.0, -1.0);
    ofTranslate(0.0, -ofGetHeight());
    feedbackBuffer.draw(0, 0);
    ofPopMatrix();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
