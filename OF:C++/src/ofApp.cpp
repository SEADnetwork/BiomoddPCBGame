#include "ofApp.h"

// utils
//--------------------------------------------------------------
ofVec2f world2grid(ofVec2f pt){
    return ofVec2f(pt.x/ofGetWidth()*grid_size, pt.y/ofGetHeight()*grid_size);
}

ofVec2f grid2world(ofVec2f pt){
    return ofVec2f(pt.x*ofGetWidth()/grid_size, pt.y*ofGetHeight()/grid_size);
}

void drawGrid(){
    ofPushStyle();
    ofSetColor(255, 100);
    for (int x(0); x < grid_size; x++){
        ofVec2f convert = grid2world(ofVec2f(x, x));
        ofLine(convert.x, 0, convert.x, ofGetHeight());
        ofLine(0, convert.x, ofGetWidth(), convert.x);
        
    }
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(fps);
    ofBackground(0);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    avatar.update();
    playerhistory.update();
    
    if (avatar.hasturned()){
        playerhistory.set(selfname, avatar.history);
    }
    
    if (playerhistory.checkBump(avatar.position)){
        avatar.alive = false;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawGrid();
    avatar.draw();
    playerhistory.draw();
}

void ofApp::reset(){
    avatar = Avatar();
    playerhistory = Playershistory();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'w':
            avatar.move(true);
            break;
        case 'x':
            avatar.move(false);
            break;
        case 'r':
            reset();
            break;
            
        default:
            break;
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
