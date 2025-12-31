#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat anonymize_face_simple(Mat image, double factor = 3.0) {
	int h = image.rows;
	int w = image.cols;
	int kW = int(w / factor);
	int kH = int(h / factor);
	if (kW % 2 == 0) {
		kW -= 1;
	}

	if (kH % 2 == 0) {
		kH -= 1;
	}

	Mat blurred_image;
	GaussianBlur(image, blurred_image, Size(kW, kW), 0);
	return blurred_image;
}

int main_face() {
	Mat img = imread("face4.jpg");
	Mat imgSmall;
	Mat imgGrayBig;
	resize(img, imgSmall, Size(640, 480));
	Mat imgGray;
	cvtColor(imgSmall, imgGray, COLOR_BGR2GRAY);
	
	CascadeClassifier frontal_detector = CascadeClassifier("haarcascade_frontalface_default.xml");
	CascadeClassifier profile_detector = CascadeClassifier("haarcascade_profileface.xml");

	Mat equalized_image;
	equalizeHist(imgGray, equalized_image);

	vector<Rect> frontal_faces;
	frontal_detector.detectMultiScale(equalized_image, frontal_faces, 1.1, 10, 0, Size(30, 30));
	vector<Rect> profile_faces;
	profile_detector.detectMultiScale(equalized_image, profile_faces, 1.1, 3, 0, Size(30, 30));

	for (int i = 0; i < frontal_faces.size(); i++) {
		Rect face = frontal_faces[i];
		Mat roi = imgSmall(face);
		Mat blurred_roi = anonymize_face_simple(roi);

		blurred_roi.copyTo(imgSmall.rowRange(face.y, face.y + face.height).colRange(face.x, face.x + face.width));
		//rectangle(imgSmall, face, Scalar(0, 0, 255), 2);
	}

	for (int i = 0; i < profile_faces.size(); i++) {
		Rect face = profile_faces[i];
		Mat roi = imgSmall(face);
		Mat blurred_roi = anonymize_face_simple(roi);
		blurred_roi.copyTo(imgSmall.rowRange(face.y, face.y + face.height).colRange(face.x, face.x + face.width));
		//rectangle(imgSmall, face, Scalar(0, 0, 255), 2);
	}

	imshow("imgray", imgSmall);
	imwrite("result4.jpg", imgSmall);
	waitKey(0);

	return 0;
}
