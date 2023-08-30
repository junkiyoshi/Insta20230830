#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetCircleResolution(60);

	for (int i = 0; i < 1; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = 30;
	ofColor color;
	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec2(
			ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.0085), 0, 1, -320, 320),
			ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.0085), 0, 1, -320, 320)
		);
		auto next = glm::vec2(
			ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.0085), 0, 1, -320, 320),
			ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.0085), 0, 1, -320, 320)
		);


		auto distance = location - next;
		distance *= 1;

		for (int i = 0; i < 18; i++) {

			auto future = location + distance * 8;
			auto random_deg = ofRandom(360);
			future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * 5);
			this->size_list.push_back(ofRandom(2, 6));

			color.setHsb(ofGetFrameNum() % 255, 200, 255);
			this->color_list.push_back(color);
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;
		this->size_list[i] *= 1.02;

		this->color_list[i].setHue((int)(this->color_list[i].getHue() + 2) % 255);

		if (glm::distance(glm::vec2(), this->location_list[i]) > 1000) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->size_list.erase(this->size_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int index = 0;
	for (auto& location : this->location_list) {

		ofSetColor(255, this->color_list[index].getBrightness());

		ofPushMatrix();
		ofTranslate(location);

		ofDrawCircle(glm::vec2(), this->size_list[index] * 1.3);

		ofPopMatrix();

		index++;
	}

	index = 0;
	for (auto& location : this->location_list) {

		ofSetColor(this->color_list[index]);

		ofPushMatrix();
		ofTranslate(location);

		ofDrawCircle(glm::vec2(), this->size_list[index++]);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}