#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
#ifdef USE_TWO_KINECTS
    kinect2.init();
    kinect2.open();
#endif
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
    farThreshold = 70;
    bThreshWithOpenCV = true;
    
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
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {
            
            // or we do it ourselves - show people how they can work with the pixels
            unsigned char * pix = grayImage.getPixels();
            
            int numPixels = grayImage.getWidth() * grayImage.getHeight();
            for(int i = 0; i < numPixels; i++) {
                if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
        }
        
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
    feedbackBuffer.draw(0, 0);
    hueShader.end();
    ofPopMatrix();
    ofPushMatrix();
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    grayImage.draw(0, 0);
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
    feedbackBuffer.draw(0, 0);
    ofPopMatrix();
    ofEnableAlphaBlending();
    personShader.begin();
    grayImage.draw(0, 0);
    personShader.end();
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
