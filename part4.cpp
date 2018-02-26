#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture cap;
cv::VideoWriter cap_out;

std::vector<cv::Mat> numbers;
std::vector<cv::Mat> digits;

void initDigits() {
	digits.push_back(cv::imread("HW3/input/part4/digits/0.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/1.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/2.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/3.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/4.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/5.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/6.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/7.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/8.jpeg") > 80);
	digits.push_back(cv::imread("HW3/input/part4/digits/9.jpeg") > 80);
}
int getDigit(cv::Mat digit) {

	for (int i = 0; i < digits.size(); i++) {
		cv::Mat diff;
		cv::erode(digits[i], diff, digit);

		if (diff.data != 0) {
			int cnz = cv::countNonZero(diff.reshape(1));
			if (cnz > 1) {
				return i;
			}
		}
		
	}


	return -1;
}
int main(int argc, char** argv) {
	cap = cv::VideoCapture("HW3/input/part4/input4_2.avi");
	cv::Size csize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cap_out.open((const cv::String)"HW3/output/part4/output4_2.mp4", CV_FOURCC('H','2','6','4'), cap.get(CV_CAP_PROP_FPS), csize, true);

	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);

	initDigits();

	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}

		numbers.clear();
		for (int i = 0; i < 7; i++) {
			char vidWindow[50];
			cv::Rect digit(49 - (8 * i), 9, 7, 7);
			cv::Mat smallf = frame(digit);
			cv::cvtColor(smallf, smallf, CV_BGR2GRAY);
			cv::threshold(smallf, smallf, 70, 255, cv::THRESH_BINARY);

			numbers.push_back(smallf);
		}

		for (int i = 0; i < numbers.size(); i++) {
			int num = getDigit(numbers[i]);
			if (num != -1) {
				cv::putText(frame, std::to_string(num), cv::Point(210 - (8 * i), 12), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 0));
			}
		}

		imshow("video 1", frame);
		cap_out << frame;
		cv::waitKey(1);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}