#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point getMidpoint(Point p1, Point p2) {
	Point midPoint;
	midPoint.x = (p1.x + p2.x) / 2;
	midPoint.y = (p1.y + p2.y) / 2;
	return midPoint;
}

bool compareRect(Rect r1, Rect r2) {
	return r1.x <= r2.x;
}

int main() {
	cout << "Opencv version: " << CV_VERSION << endl;

	Mat imgGray = imread("C:\\Users\\Thiago Monteiro\\Documents\\Coder Backup\\PC and Mac Apps\\Learning Languages\\OpenCV C++\\Measure C++\\img.jpg", IMREAD_GRAYSCALE);
	Mat imgSmall;
	resize(imgGray, imgSmall, Size(800, 600));

	//imshow("imgray", imgSmall);
	//waitKey(0);
	
	Mat imgBlur;
	GaussianBlur(imgSmall, imgBlur, Size(7, 7), 0);

	//imshow("imgray", imgBlur);
	//waitKey(0);

	Mat edges;
	Canny(imgBlur, edges, 50, 100);

	//imshow("imgray", imgCanny);
	//waitKey(0);

	Mat dilation;
	dilate(edges, edges, Mat());
	erode(edges, edges, Mat());

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> filteredContours;
	vector<Rect> boundingRects;
	//Mat drawing = Mat::zeros(edges.size(), CV_8UC3);
	//int counter = 0;
	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours.at(i));
		//if (area >= 300) {
			//counter++;
			//drawContours(drawing, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
		//}
		if (area >= 200) {
			filteredContours.push_back(contours.at(i));
			boundingRects.push_back(boundingRect(contours.at(i)));
		}
	}

	sort(boundingRects.begin(), boundingRects.end(), compareRect);

	Rect firstRect = boundingRects.at(0);

	Point topLeft = Point(firstRect.x, firstRect.y);
	Point topRight = Point(firstRect.x + firstRect.width, firstRect.y);
	Point bottomLeft = Point(firstRect.x, firstRect.y + firstRect.height);
	Point bottomRight = Point(firstRect.x + firstRect.width, firstRect.y + firstRect.height);

	line(imgSmall, topLeft, topRight, Scalar(255, 255, 255));
	line(imgSmall, topLeft, bottomLeft, Scalar(255, 255, 255));
	line(imgSmall, topRight, bottomRight, Scalar(255, 255, 255));
	line(imgSmall, bottomLeft, bottomRight, Scalar(255, 255, 255));

	//circle(imgSmall, topLeft, 5, Scalar(255, 255, 255));
	//circle(imgSmall, topRight, 5, Scalar(255, 255, 255));
	//circle(imgSmall, bottomLeft, 5, Scalar(255, 255, 255));
	//circle(imgSmall, bottomRight, 5, Scalar(255, 255, 255));

	Point midTop = getMidpoint(topLeft, topRight);
	Point midBottom = getMidpoint(bottomLeft, bottomRight);
	Point midLeft = getMidpoint(topLeft, bottomLeft);
	Point midRight = getMidpoint(topRight, bottomRight);

	//line(imgSmall, midTop, midBottom, Scalar(255, 255, 255));
	//line(imgSmall, midLeft, midRight, Scalar(255, 255, 255));

	putText(imgSmall, "2.3cm", Point(midTop.x, midTop.y - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
	putText(imgSmall, "2.3cm", Point(midRight.x + 10, midRight.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
	
	int width = norm(topLeft - topRight);
	float pixelsPerCm = width / 2.3;

	for (int i = 1; i < boundingRects.size(); i++) {
		Rect bb = boundingRects.at(i);
		Point bbTopLeft = Point(bb.x, bb.y);
		Point bbTopRight = Point(bb.x + bb.width, bb.y);
		Point bbBottomLeft = Point(bb.x, bb.y + bb.height);
		Point bbBottomRight = Point(bb.x + bb.width, bb.y + bb.height);

		Point bbMidTop = getMidpoint(bbTopLeft, bbTopRight);
		Point bbMidBottom = getMidpoint(bbBottomLeft, bbBottomRight);
		Point bbMidLeft = getMidpoint(bbTopLeft, bbBottomLeft);
		Point bbMidRight = getMidpoint(bbTopRight, bbBottomRight);

		circle(imgSmall, bbTopLeft, 5, Scalar(255, 255, 255));
		circle(imgSmall, bbTopRight, 5, Scalar(255, 255, 255));
		circle(imgSmall, bbBottomLeft, 5, Scalar(255, 255, 255));
		circle(imgSmall, bbBottomRight, 5, Scalar(255, 255, 255));

		int bbWidth = norm(bbTopLeft - bbTopRight);
		float bbWidthCm = int((bbWidth / pixelsPerCm) * 10) / float(10);
		int bbHeight = norm(bbTopLeft - bbBottomLeft);
		float bbHeightCm = int((bbHeight / pixelsPerCm) * 10) / float(10);
		ostringstream oss;
		oss << setprecision(1) << bbWidthCm << "cm";
		//cout << "Width: " << bbWidthCm << endl;
		//cout << "Height " << bbHeightCm << endl;

		putText(imgSmall, oss.str(), Point(bbMidRight.x + 10, bbMidRight.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		oss.str("");
		oss.clear();
		oss << setprecision(1) << bbHeightCm << "cm";
		putText(imgSmall, oss.str(), Point(bbMidRight.x + 10, bbMidRight.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
	}

	imshow("imgray", imgSmall);
	waitKey(0);

	return 0;
}