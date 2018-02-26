#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;
bool first = false;
cv::Point lastPos;

cv::Point getPacmanPos(cv::Mat frame) {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	if (contours.size() == 0) {
		return cv::Point(-1, -1);
		printf("dfdfd\n");
	}
	else {
		std::vector<cv::Moments> mu(contours.size());
		mu[0] = cv::moments(contours[0], 1);
		cv::Point pacpoint(mu[0].m10 / mu[0].m00, mu[0].m01 / mu[0].m00);
		return pacpoint;
	}
}



int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part3a/input3a1.avi");
	cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cap_out.open((const cv::String)"HW3/output/part3a/output3a1.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), csize, true);

	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);
	cv::Mat draw(cv::Size(224, 288), CV_8UC3, cv::Scalar(0));


	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}
		cv::Mat pacman;
		cv::inRange(frame, cv::Scalar(0, 220, 220), cv::Scalar(40, 255, 255), pacman);
		cv::Mat pelletColor;
		cv::Mat twoPac;
		cv::inRange(frame, cv::Scalar(170, 170, 220), cv::Scalar(210, 205, 255), pelletColor);
		cv::Rect bottom(0, 270, 224, 16);
		pacman(bottom) = 0;
		cv::dilate(pacman, twoPac, cv::Mat::ones(cv::Size(4, 4), CV_8U), cv::Point(-1, -1), 1, 0, 0);
		cv::GaussianBlur(twoPac, twoPac, cv::Size(13, 13), 5);
		twoPac = twoPac > 60;

		cv::Point blobPos = getPacmanPos(twoPac);

		if (!first) {
			if (blobPos.x >= 0 && cv::norm(blobPos - lastPos) < 25) {
				cv::line(draw, lastPos, blobPos, cv::Scalar(0, 255, 255), 2, 8, 0);
			}
		}
		lastPos = blobPos;
		draw.copyTo(frame, draw != 0);
		imshow("video 1", frame);
		cap_out << frame;

		cv::waitKey(1);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}