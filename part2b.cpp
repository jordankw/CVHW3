#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;
cv::Rect pellet1(7, 47, 10, 10);
cv::Rect pellet2(207, 47, 10, 10);
cv::Rect pellet3(7, 207, 10, 10);
cv::Rect pellet4(207, 207, 10, 10);
cv::Rect lives(16, 273, 31, 13);
cv::Mat z1 = cv::Mat::zeros(cv::Size(10, 10), CV_8U);
cv::SimpleBlobDetector det;
int numLives = 0;

void detectLives(cv::Mat frame) {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(frame(lives), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	numLives = contours.size();

	printf("number of lives: %i\n", numLives);
	imshow("lives", frame(lives));



	

}


int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part2b/input2b1.avi");
	cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cap_out.open((const cv::String)"HW3/output/part2b/output2b1.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), csize, true);
	bool eof = false;
	det = cv::SimpleBlobDetector();
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("lives", cv::WINDOW_NORMAL);
	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}

		cv::Mat gray;
		cv::cvtColor(frame, gray, CV_RGB2GRAY);
		gray = (gray > 128);

		detectLives(gray);

		char buf[100];
		sprintf_s(buf, "%i", numLives);



		cv::putText(frame, buf, cv::Point2f(4, 282), cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(255, 255, 255, 255));

		imshow("video 1", frame);
		cap_out << frame;
		cv::waitKey(1);

		i++;
	} while (!eof);
	cv::waitKey(0);
}