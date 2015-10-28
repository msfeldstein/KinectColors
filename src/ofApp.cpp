#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 255;
    farThreshold = 90;
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
//
    showGUI = false;
    gui.setup();
    gui.add(shiftAmount.setup("Hue Shift Amount", 0.01, 0.0, 0.03));
    gui.add(fadeAmount.setup("Fade Amount", 0.0, 0.0, 0.03));
    
    feedbackBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    feedbackBuffer.getTextureReference().setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
    feedbackBuffer.bind();
    ofClear(0, 0, 0, 1);
    feedbackBuffer.unbind();
    person.loadImage("person.png");
    hueShader.setupShaderFromFile(GL_FRAGMENT_SHADER, "hueshift.frag");
    hueShader.linkProgram();
    personShader.setupShaderFromFile(GL_FRAGMENT_SHADER, "drawperson.frag");
    personShader.linkProgram();
//    depthPlayer.loadMovie("MovementDepth.mov");
//    depthPlayer.getTextureReference().setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
//    depthPlayer.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        grayImage.blur(7);
        // update the cv images
        grayImage.flagImageChanged();
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as
    }

////    depthPlayer.update();
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
    hueShader.setUniform1f("fadeAmount", fadeAmount);
    feedbackBuffer.draw(0, 0, feedbackBuffer.getWidth(), feedbackBuffer.getHeight());
    hueShader.end();
    ofPopMatrix();
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    grayImage.draw(0, 0, feedbackBuffer.getWidth(), feedbackBuffer.getHeight());
    ofDisableBlendMode();
    ofPopStyle();
    ofPopMatrix();
    feedbackBuffer.unbind();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0, 0, 0, 1.0);
    ofPushMatrix();
    ofScale(1.0, -1.0);
    ofTranslate(0.0, -ofGetHeight());
    feedbackBuffer.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopMatrix();
    ofEnableAlphaBlending();
    personShader.begin();
    grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    personShader.end();
    if (showGUI) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    } else if (key == 'g') {
        showGUI = !showGUI;        
    } else if (key == ',') {
        farThreshold++;
        std::cout<<"Far Threshold"<<farThreshold<<"\n";
    } else if (key == '.') {
        farThreshold--;
        std::cout<<"Far Threshold"<<farThreshold<<"\n";
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
