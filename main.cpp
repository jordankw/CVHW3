#include <opencv2/opencv.hpp>

cv::VideoCapture cap;

int main(int argc, char** argv) {
	cap = cv::VideoCapture("C:/Users/jordan/Videos/hdk.mp4");
	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);

	int i = 0;
	do {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			eof = true;
			break;
		}

		blurSize = (sinf(i / 10.0) + 1) * 3 ;
		cv::GaussianBlur(frame, frame, cv::Size(13, 13), blurSize);
		printf("blurSize: %f\n", blurSize);

		imshow("video 1", frame);
		cv::waitKey(30);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}