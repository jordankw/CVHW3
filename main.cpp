#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::Rect pellet1(7, 47, 10, 10);
cv::Rect pellet2(207, 47, 10, 10);
cv::Rect pellet3(7, 207, 10, 10);
cv::Rect pellet4(207, 207, 10, 10);

cv::Rect lives(16, 273, 31, 13);
cv::Mat z1 = cv::Mat::zeros(cv::Size(10, 10), CV_8U);
bool pellets[4] = {0,0,0,0};
int numPellets = 0;
void initPellet(cv::Mat frame) {
	if (countNonZero(frame(pellet1)) > 9 && !pellets[0]) {
		pellets[0] = true;
		numPellets++;
		printf("pellet 1 detected", numPellets);
	}
	if (countNonZero(frame(pellet2)) > 9 && !pellets[1]) {
		pellets[1] = true;
		numPellets++;
		printf("pellet 2 detected", numPellets);
	}
	if (countNonZero(frame(pellet3)) > 9 && !pellets[2]) {
		pellets[2] = true;
		numPellets++;
		printf("pellet 3 detected", numPellets);
	}
	if (countNonZero(frame(pellet4)) > 9 && !pellets[3]) {
		pellets[3] = true;
		numPellets++;
		printf("pellet 4 detected", numPellets);
	}
}
void detectPellet(cv::Mat frame) {
	
	cv::Mat f1 = frame(pellet1);
	//imshow("pellet1", frame(pellet1));
	//imshow("pellet2", frame(pellet2));
	//imshow("pellet3", frame(pellet3));
	//imshow("pellet4", frame(pellet4));

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
void ghostBlobs(cv::Mat frame) {
	cv::Mat cyan, pink, orange, red;
	cv::inRange(frame, cv::Scalar(0, 0, 130), cv::Scalar(40, 40, 255), red);
	cv::inRange(frame, cv::Scalar(220, 180, 195), cv::Scalar(255, 198, 255), pink);
	cv::inRange(frame, cv::Scalar(230, 230, 0), cv::Scalar(255, 255, 25), cyan);
	cv::inRange(frame, cv::Scalar(65, 170, 225), cv::Scalar(95, 190, 255), orange); //adjust
	cv::GaussianBlur(red, red, cv::Size(13, 13), 5);
	cv::GaussianBlur(pink, pink, cv::Size(13, 13), 5);
	cv::GaussianBlur(cyan, cyan, cv::Size(13, 13), 5);
	cv::GaussianBlur(orange, orange, cv::Size(13, 13), 5);
	//cv::GaussianBlur(pink, pink, cv::Size(13, 13), 5);
	red = red > 60;
	pink = pink > 60;
	cyan = cyan > 30;
	orange = orange > 30;
	imshow("redghost", orange);
	
}
void detectLives(cv::Mat& frame) {
	imshow("lives", frame(lives));


}

int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/templates/input4_1.mp4");
	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);

	/*cv::namedWindow("pellet1",cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet1", cv::Size(100, 100));
	cv::namedWindow("pellet2", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet2", cv::Size(100, 100));
	cv::namedWindow("pellet3", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet3", cv::Size(100, 100));
	cv::namedWindow("pellet4", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet4", cv::Size(100, 100));*/

	cv::namedWindow("lives", cv::WINDOW_NORMAL);
	cv::namedWindow("redghost", cv::WINDOW_NORMAL);

	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}

		//cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point())


		cv::Mat thresh;
		cv::inRange(frame, cv::Scalar(84, 30, 30), cv::Scalar(150, 60, 50), thresh);

		cv::Mat pacman;
		cv::inRange(frame, cv::Scalar(0, 220, 220), cv::Scalar(40, 255, 255), pacman);
		cv::Mat pelletColor;
		//cv::Mat newPellet;
		cv::inRange(frame, cv::Scalar(170, 170, 220), cv::Scalar(210, 205, 255), pelletColor);
		//cv::erode(pelletColor, newPellet, cv::Mat::ones(cv::Size(4, 4), CV_8U), cv::Point(-1, -1), 1, 0, 0);
		//blurSize = (sinf(i / 10.0) + 1) * 3 ;
		//cv::GaussianBlur(frame, frame, cv::Size(13, 13), blurSize);
		//printf("blurSize: %f\n", blurSize);

		imshow("video 2", pacman);
		imshow("video 1", frame);
		cv::Mat gray;
		cv::cvtColor(frame, gray, CV_RGB2GRAY);
		gray = (gray > 128);


		ghostBlobs(frame);
		initPellet(pelletColor);
		detectPellet(pacman);
		detectLives(gray);
		cv::waitKey(30);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}