#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;
int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part1a/input1a2.avi");

	cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cap_out.open((const cv::String)"HW3/output/part1a/output1a2.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), csize, true);
	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);

	int i = 0;
	do {
		cv::Mat frame, thresh;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}
		cv::inRange(frame, cv::Scalar(100, 30, 30), cv::Scalar(150, 60, 50), thresh);

		frame.setTo(cv::Scalar(0, 255, 0), thresh);

		imshow("video 2", thresh);
		imshow("video 1", frame);

		cap_out << frame;

		cv::waitKey(30);

		i++;
	} while (!eof);
	cv::waitKey(0);
}