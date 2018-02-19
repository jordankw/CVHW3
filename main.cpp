#include <opencv2/opencv.hpp>

cv::VideoCapture cap;

int main(int argc, char** argv) {
	cap = cv::VideoCapture("C:/Users/Jordan/Documents/Drive/CMPR 575/HW3/input/part1a/input1a1.avi");
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

		//cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point())


		cv::Mat thresh;
		cv::inRange(frame, cv::Scalar(84, 30, 30), cv::Scalar(150, 60, 50), thresh);


		blurSize = (sinf(i / 10.0) + 1) * 3 ;
		cv::GaussianBlur(frame, frame, cv::Size(13, 13), blurSize);
		printf("blurSize: %f\n", blurSize);

		imshow("video 2", thresh);
		imshow("video 1", frame);
		cv::waitKey(30);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}