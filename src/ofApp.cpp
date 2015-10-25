#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    showGUI = false;
    gui.setup();
    gui.add(shiftAmount.setup("Hue Shift Amount", 0.01, 0.0, 0.03));
    feedbackBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    feedbackBuffer.getTextureReference().setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

    feedbackBuffer.bind();
    ofClear(0, 0, 0, 1);
    feedbackBuffer.unbind();
    person.loadImage("person.png");
    hueShader.load("hueshift.vert","hueshift.frag");
    personShader.load("drawperson.vert", "drawperson.frag");
    depthPlayer.loadMovie("MovementDepth.mov");
    depthPlayer.getTextureReference().setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
    depthPlayer.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    depthPlayer.update();
    feedbackBuffer.bind();
    ofPushMatrix();
    // Flip the FBO
    float scaleAmount = 0.02;
    ofScale(1.0 + scaleAmount, 1.0 + scaleAmount);
    ofScale(1.0, -1.0);
    float dx =-ofGetWidth() * scaleAmount / 2.0;
    float dy = -ofGetHeight() * (1.0 - scaleAmount / 2.0);
    ofTranslate(dx, dy);
    
    // Draw the hue shifted fbo back into itself
    hueShader.begin();
    hueShader.setUniform1f("amount", shiftAmount);
    feedbackBuffer.draw(0, 0);
    hueShader.end();
    ofPopMatrix();
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    depthPlayer.draw(0, 0);
    ofPopStyle();
    ofPopMatrix();
    feedbackBuffer.unbind();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0, 0, 0);
    ofDisableBlendMode();
    ofDisableAlphaBlending();
    ofPushMatrix();
    ofScale(1.0, -1.0);
    ofTranslate(0.0, -ofGetHeight());
    ofDisableAlphaBlending();
    glDisable(GL_BLEND);
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    feedbackBuffer.draw(0, 0);
    ofPopMatrix();
    if (showGUI) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    } else if (key == 'g') {
        showGUI = !showGUI;        
    }

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
