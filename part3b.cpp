#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;
cv::Mat drawRed, drawCyan, drawOrange, drawPink;
bool first = false;
cv::Point redPos, cyanPos, orangePos, pinkPos, lastRedPos, lastCyanPos, lastOrangePos, lastPinkPos;

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

void ghostBlobs(cv::Mat frame) {
	cv::Mat cyan, pink, orange, red;
	cv::inRange(frame, cv::Scalar(0, 0, 130), cv::Scalar(40, 40, 255), red);
	cv::inRange(frame, cv::Scalar(220, 180, 195), cv::Scalar(255, 198, 255), pink);
	cv::inRange(frame, cv::Scalar(204, 204, 0), cv::Scalar(255, 255, 60), cyan);
	cv::inRange(frame, cv::Scalar(72, 170, 225), cv::Scalar(130, 210, 255), orange); //adjust
	cv::GaussianBlur(red, red, cv::Size(13, 13), 5);
	cv::GaussianBlur(pink, pink, cv::Size(13, 13), 5);
	cv::GaussianBlur(cyan, cyan, cv::Size(13, 13), 5);
	cv::GaussianBlur(orange, orange, cv::Size(13, 13), 5);
	cv::GaussianBlur(pink, pink, cv::Size(13, 13), 5);
	red = red > 60;
	pink = pink > 50;
	cyan = cyan > 40;
	orange = orange > 40;

	redPos = getPacmanPos(red);
	orangePos = getPacmanPos(orange);
	pinkPos = getPacmanPos(pink);
	cyanPos = getPacmanPos(cyan);

	if (!first) {
		if (cv::norm(redPos - lastRedPos) < 25) {
			cv::line(drawRed, lastRedPos, redPos, cv::Scalar(0, 0, 255), 2, 8, 0);
		}
		if (cv::norm(cyanPos - lastCyanPos) < 25) {
			cv::line(drawRed, lastCyanPos, cyanPos, cv::Scalar(255, 255, 0), 2, 8, 0);
		}
		if (cv::norm(pinkPos - lastPinkPos) < 25) {
			cv::line(drawRed, lastPinkPos, pinkPos, cv::Scalar(255, 170, 255), 2, 8, 0);
		}
		if (cv::norm(orangePos - lastOrangePos) < 25) {
			cv::line(drawRed, lastOrangePos, orangePos, cv::Scalar(80, 170, 255), 2, 8, 0);
		}
	}
	lastRedPos = redPos;
	lastOrangePos = orangePos;
	lastCyanPos = cyanPos;
	lastPinkPos = pinkPos;	
}


int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part3b/input3b1.avi");
	cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cap_out.open((const cv::String)"HW3/output/part3b/output3b1.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), csize, true);

	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	drawRed = cv::Mat(cv::Size(224, 288), CV_8UC3, cv::Scalar(0));

	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}
		cv::Mat frame2 = frame;
		cv::Rect bottom(0, 270, 224, 16);
		frame2(bottom) = 0;
		ghostBlobs(frame2);
		drawRed.copyTo(frame, drawRed != 0);

		imshow("video 1", frame);
		cap_out << frame;
		cv::waitKey(1);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}