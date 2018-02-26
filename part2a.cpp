#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;
cv::Rect pellet1(7, 47, 10, 10);
cv::Rect pellet2(207, 47, 10, 10);
cv::Rect pellet3(7, 207, 10, 10);
cv::Rect pellet4(207, 207, 10, 10);

cv::Mat z1 = cv::Mat::zeros(cv::Size(10, 10), CV_8U);
bool pellets[4] = { 0,0,0,0 };
int pelletsCounter[4] = { 0,0,0,0 };
int numPellets = 0;
void initPellet(cv::Mat frame) {
	if (countNonZero(frame(pellet1)) > 9 && !pellets[0]) {
		pellets[0] = true;
		numPellets++;
		pelletsCounter[0] = 30;
		printf("pellet 1 detected", numPellets);
	}
	if (countNonZero(frame(pellet2)) > 9 && !pellets[1]) {
		pellets[1] = true;
		numPellets++;
		pelletsCounter[1] = 30;
		printf("pellet 2 detected", numPellets);
	}
	if (countNonZero(frame(pellet3)) > 9 && !pellets[2]) {
		pellets[2] = true;
		numPellets++;
		pelletsCounter[2] = 30;
		printf("pellet 3 detected", numPellets);
	}
	if (countNonZero(frame(pellet4)) > 9 && !pellets[3]) {
		pellets[3] = true;
		numPellets++;
		pelletsCounter[3] = 30;
		printf("pellet 4 detected", numPellets);
	}
}
void detectPellet(cv::Mat frame) {

	cv::Mat f1 = frame(pellet1);

	if (countNonZero(frame(pellet1)) > 4 && pellets[0]) {
		pellets[0] = false;
		numPellets--;
		printf("pellet 1 hit number of pellets: %i\n", numPellets);
	}
	if (countNonZero(frame(pellet2)) > 4 && pellets[1]) {
		pellets[1] = false;
		numPellets--;
		printf("pellet 2 hit number of pellets: %i\n", numPellets);
	}
	if (countNonZero(frame(pellet3)) > 4 && pellets[2]) {
		pellets[2] = false;
		numPellets--;
		printf("pellet 3 hit number of pellets: %i\n", numPellets);
	}
	if (countNonZero(frame(pellet4)) > 4 && pellets[3]) {
		pellets[3] = false;
		numPellets--;
		printf("pellet 4 hit number of pellets: %i\n", numPellets);
	}
	//printf("nonzero 1 = %i\n",countNonZero(frame(pellet1)));


	
	
}


int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part2a/input2a1.avi");
	//cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	//cap_out.open((const cv::String)"HW3/output/part2a/output2a1.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), csize, true);
	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);



	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}


		cv::Mat pacman, pelletColor;
		cv::inRange(frame, cv::Scalar(0, 220, 220), cv::Scalar(40, 255, 255), pacman);

		cv::inRange(frame, cv::Scalar(170, 170, 220), cv::Scalar(210, 205, 255), pelletColor);

		cv::Mat gray;
		cv::cvtColor(frame, gray, CV_RGB2GRAY);
		gray = (gray > 128);

		initPellet(pelletColor);
		detectPellet(pacman);

		if (pellets[0] == false && pelletsCounter[0] > 0) {
			cv::rectangle(frame, pellet1, cv::Scalar(100, 255, 255));
			pelletsCounter[0]--;
		}
		if (pellets[1] == false && pelletsCounter[1] > 0) {
			cv::rectangle(frame, pellet2, cv::Scalar(100, 255, 255));
			pelletsCounter[1]--;
		}
		if (pellets[2] == false && pelletsCounter[2] > 0) {
			cv::rectangle(frame, pellet3, cv::Scalar(100, 255, 255));
			pelletsCounter[2]--;
		}
		if (pellets[3] == false && pelletsCounter[3] > 0) {
			cv::rectangle(frame, pellet4, cv::Scalar(100, 255, 255));
			pelletsCounter[3]--;
		}

		char buf[100];
		sprintf_s(buf, "pellets: %i", numPellets);
		cv::putText(frame, buf, cv::Point2f(160, 7), cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(255, 255, 255, 255));

		imshow("video 2", pacman);
		imshow("video 1", frame);
		//cap_out << frame;
		cv::waitKey(1);

		i++;
	} while (!eof);
	cv::waitKey(0);
}