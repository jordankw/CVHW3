#include <opencv2/opencv.hpp>

cv::VideoCapture cap;
cv::Rect pellet1(7, 47, 10, 10);
cv::Rect pellet2(207, 47, 10, 10);
cv::Rect pellet3(7, 207, 10, 10);
cv::Rect pellet4(207, 207, 10, 10);

cv::Rect lives(16, 273, 31, 13);


void detectPellet(cv::Mat frame) {
	

	imshow("pellet1", frame(pellet1));
	imshow("pellet2", frame(pellet2));
	imshow("pellet3", frame(pellet3));
	imshow("pellet4", frame(pellet4));

}
void detectLives(cv::Mat& frame) {
	imshow("lives", frame(lives));


}

int main(int argc, char** argv) {
	cap = cv::VideoCapture("C:/Users/jordan/Documents/Google Drive/CMPR 575/HW3/input/part1a/input1a1.avi");
	bool eof = false;
	float blurSize = 0.1f;
	cv::namedWindow("video 1", 1);
	cv::namedWindow("video 2", 1);

	cv::namedWindow("pellet1",cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet1", cv::Size(100, 100));
	cv::namedWindow("pellet2", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet2", cv::Size(100, 100));
	cv::namedWindow("pellet3", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet3", cv::Size(100, 100));
	cv::namedWindow("pellet4", cv::WINDOW_NORMAL);
	cv::resizeWindow("pellet4", cv::Size(100, 100));

	cv::namedWindow("lives", cv::WINDOW_NORMAL);


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


		//blurSize = (sinf(i / 10.0) + 1) * 3 ;
		//cv::GaussianBlur(frame, frame, cv::Size(13, 13), blurSize);
		//printf("blurSize: %f\n", blurSize);

		imshow("video 2", thresh);
		imshow("video 1", frame);
		//detectPellet(frame);
		detectLives(frame);
		cv::waitKey(30);
		
		i++;
	} while (!eof);
	cv::waitKey(0);
}