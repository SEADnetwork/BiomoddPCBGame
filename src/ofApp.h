#pragma once

#include "ofMain.h"
#include "Utils.h"
#include "Constants.h"

class WorldUtils {
public:
    // fix this
    static ofVec2f world2grid(ofVec2f pt){
        return ofVec2f(pt.x/ofGetWidth()*grid_size, pt.y/ofGetHeight()*grid_size);
    }
    
    static ofVec2f grid2world(ofVec2f pt){
        return ofVec2f(pt.x*ofGetWidth()/grid_size, pt.y*ofGetHeight()/grid_size);
    }
    
    // returns true if point lies on the line
    //    http://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points
    static bool pointOnLine(const ofVec2f p1, const ofVec2f p2, const ofVec2f currp){
        float dxc = currp.x - p1.x;
        float dyc = currp.y - p1.y;
        
        float dxl = p2.x - p1.x;
        float dyl = p2.y - p1.y;
        
        int cross = (int) dxc * dyl - dyc * dxl;
        
        // check if point lays on line (not necessarily between the two outer points)
        if (cross != 0){ return false; }
        
        if (abs(dxl) >= abs(dyl)){
            return (dxl > 0) ? ((p1.x <= currp.x) && (currp.x <= p2.x )) : ((p2.x <= currp.x) && (currp.x <= p1.x ));
        } else {
            return (dyl > 0) ? ((p1.y <= currp.y) && (currp.y <= p2.y )) : ((p2.y <= currp.y) && (currp.y <= p1.y ));
        }
    }
  
};

class Avatar {
public:
    
    //constructor
    Avatar(){
        const int offset = grid_size/4;
        position = ofVec2f(ofRandom(offset, grid_size-offset), ofRandom(offset, grid_size-offset));
        orientation = (int)ofRandom(100)%8;
        updateHistory();
        alive = true;
    }
    
    //methods
    void update(){
        if (!alive){ return; }
        
        switch (orientation) {
            case 0:
                position.y-=speed;
                break;
            case 1:
                position.x+=speed;
                position.y-=speed;
                break;
            case 2:
                position.x+=speed;
                break;
            case 3:
                position.x+=speed;
                position.y+=speed;
                break;
            case 4:
                position.y+=speed;
                break;
            case 5:
                position.x-=speed;
                position.y+=speed;
                break;
            case 6:
                position.x-=speed;
                break;
            case 7:
                position.x-=speed;
                position.y-=speed;
                break;
            default:
                cout << "wrong position" << endl;
                break;
        }
        
        if ((position.x <= 0) || (position.x >= grid_size) || (position.y <= 0) || (position.y >= grid_size)){
            updateHistory();
            alive = false;
        }
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(ofColor::pink);
        ofPoint worldPoint = ofPoint(WorldUtils::grid2world(position));
        
        ofCircle(worldPoint, 10);
        ofLine(worldPoint, WorldUtils::grid2world(getLastPoint()));
        
        ofPopStyle();
    }
    
    void move(bool left){
        left ? orientation-- : orientation++;
        orientation = orientation%8;
        updateHistory();
    }
    
    bool hasturned(){
        bool rv = hasturned_;
        hasturned_ = false;
        return rv;
    }
    
    ofVec2f getLastPoint(){
        return history.back();
    }
    
    
    
    //members
    ofVec2f position; //in grid
    unsigned int orientation;
    int speed = 1;
    bool hasturned_;
    vector<ofVec2f>history;
    bool alive;
    
//    +--------------------------+
//    |             0            |
//    |   7        +             |
//    |     X      |    XX 1     |
//    |     XXX    |   XX        |
//    |       XXX  |XXXX         |
//    | 6  +--------------+  2   |
//    |          XX|XXXXX        |
//    |        XXX |    XX       |
//    |       XX   |     X       |
//    |       X    +        3    |
//    |     5      4             |
//    |                          |
//    |                          |
//    +--------------------------+
    
private:
    void updateHistory(){
        hasturned_ = true;
        history.push_back(position);
    }

    
};


class Playershistory {
public:
    //typedefs
    typedef vector<ofVec2f> Line;
    typedef pair<string, Line> NamedLine;
    typedef map<string, Line> MapLine;
    
    
    //methods
    Playershistory(){}
    
    void push(const string name, const ofVec2f & pt){
        auto res = mline.find(name);
        if (res == mline.end()){
            // not found
            mline.insert(getNamedLine(name, &pt));
        } else {
            // found
            (*res).second.push_back(pt);
        }
    }
    
    void set(const string name, const Line & pts){
        auto res = mline.find(name);
        if (res == mline.end()){
            // not found
            auto ret = mline.insert(getNamedLine(name));
            
            //ret.first points to the newly created element
            // .second points to the value
            (*(ret.first)).second = pts;
        } else {
            // found
            (*res).second = pts;
        }
    }
    
    void update(){
        for (auto it(mline.begin()); it != mline.end(); it++){
            // do something while updating
        }
    }
    
    void draw(){
        for (auto it(mline.begin()); it != mline.end(); it++){
            // do something while updating
            ofPushStyle();
            ofSetColor(ofColor::white);
            ofNoFill();
            
            ofBeginShape();
            
            for (auto pt((*it).second.begin()); pt != (*it).second.end(); pt++){
                const ofPoint vp = ofPoint(WorldUtils::grid2world(*pt));
                ofVertex(vp.x, vp.y);
            }
            
            ofEndShape();
            ofPopStyle();

        }
    }
    
    bool checkBump(const ofVec2f pos){
        for (auto it(mline.begin()); it != mline.end(); it++){
            
            Line * currline = &(*it).second;
            
            if (currline->size()>1){
                //start at second point
                for (auto pt(currline->begin()+1); pt != currline->end(); pt++){
                    if (WorldUtils::pointOnLine(*(pt-1), *pt, pos)){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    
    //members
    MapLine mline;
    
    
    //MARK: private Player history
    private:
    
    NamedLine getNamedLine(const string name, const ofVec2f * pt = nil){
        Line newLine;
        if (pt){
            newLine.push_back(*pt);
        }
        NamedLine newNamedLine(name, newLine);
        return newNamedLine;
    }
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    Avatar avatar;
    Playershistory playerhistory;

    void reset();
    
    
};
