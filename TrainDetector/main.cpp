#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
class Szamlalok {
public:
	vector<Point> jelenlegiKontur;
	vector<Point> kozeppont;
	Rect currentBoundingRect;
	double atlo, kep_meret;
	Point kozeppont_jelenleg;
	Szamlalok(vector<Point> kontur) {
		jelenlegiKontur = kontur;
		//Point kozeppont_jelenleg;
		currentBoundingRect = boundingRect(jelenlegiKontur);
		kozeppont_jelenleg.x = (currentBoundingRect.x + currentBoundingRect.x + currentBoundingRect.width) / 2;
		kozeppont_jelenleg.y = (currentBoundingRect.y + currentBoundingRect.y + currentBoundingRect.height) / 2;
		kozeppont.push_back(kozeppont_jelenleg);
		atlo = sqrt(pow(currentBoundingRect.width, 2) + pow(currentBoundingRect.width, 2));
		kep_meret = (float)currentBoundingRect.width /
			(float)currentBoundingRect.height;
	}
	String toString() {
		String s = to_string(atlo) + " " + to_string(kep_meret) + " "
			+ to_string(currentBoundingRect.x) + " " + to_string(currentBoundingRect.y) + " "
			+ to_string(kozeppont_jelenleg.x) + " " + to_string(kozeppont_jelenleg.y) + " "
			+ to_string(currentBoundingRect.width) + " " + to_string(currentBoundingRect.height);

		return s;
	}


};
void kontur_rajzol(Size imgSize, vector<vector<Point>> konturok, String nev) {
	Mat kep(imgSize, CV_8UC3, Scalar(0.0, 0.0, 0.0));
	drawContours(kep, konturok, -1, Scalar(255.0, 255.0, 255.0), -1);
	//resize(kep, kep, Size(kep.cols / 2, kep.rows / 2));
	//namedWindow(nev, CV_WINDOW_AUTOSIZE);
	imshow(nev, kep);
}
void megjelol(vector<Szamlalok> &kocsik, Mat &frameCopy) {
	for (int i = 0; i < kocsik.size(); i++) {
		rectangle(frameCopy, kocsik[i].currentBoundingRect, Scalar(144.0, 0.0, 255.0), 10);
		int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
		double dblFontScale =
			kocsik[i].atlo / 60.0;
		putText(
			frameCopy,
			kocsik[i].toString(),
			kocsik[i].kozeppont.back(),
			1,
			1,
			Scalar(255.0, 0, 0),
			2
		);
	}
}
int main() {


	VideoCapture cap;
	//03128288.MP4 03128282.MP4 03128283.MP4
	cap.open("d:\\Programok\\Projects\\Visual Studio\\cutted_videos\\03128288.MP4");
	Point vonal[2];
	Mat frame1, frame2, grey1, grey2, diff, threshold2;
	Mat clone;
	int fuggoleges_hatar;
	vector<Rect> targets;
	vector<Szamlalok> jelenlegikeretek;
	vector<vector<Point> > konturok;
	vector<vector<Point> > convexHulls(konturok.size());
	bool szunet = false;
	//sdffadfasdfasdfasdf
	int frame_count = 0;
	while (1) {
		
		if (!cap.isOpened()) {
			cout << "error reading video file" << endl << endl;
			getchar();
			if (frame1.empty()|| frame2.empty()) {
				return 0;
			}
			
			return 0;

		}
		while (cap.get(CV_CAP_PROP_POS_FRAMES) < cap.get(CV_CAP_PROP_FRAME_COUNT) - 1) {
			
			cap.read(frame1);
			cap.read(frame2);
			if (frame1.empty()|| frame2.empty()) {
				return 0;
			}
			

			
			cvtColor(frame1, grey1, COLOR_BGR2GRAY);
			cvtColor(frame2, grey2, COLOR_BGR2GRAY);
			absdiff(grey1, grey2, diff);
			




			
		
		
			fuggoleges_hatar = (int)round(frame1.cols*0.2);
			vonal[0].x = fuggoleges_hatar;
			vonal[0].y = 0;
			vonal[1].x = fuggoleges_hatar;
			vonal[1].y = frame1.rows - 1;


		

			adaptiveThreshold(diff,
				threshold2, 255,
				ADAPTIVE_THRESH_MEAN_C,
				THRESH_BINARY_INV,
				21,
				10);
			//elõször itt kell kisérletezni
			//threshold(diff, threshold2, 40, 245.0, CV_THRESH_BINARY);
			//resize(threshold2, threshold2, Size(threshold2.cols / 2, threshold2.rows / 2));
			//namedWindow("Threshold_before blur", CV_WINDOW_AUTOSIZE);
			//imshow("Threshold_before blur", threshold2);


			GaussianBlur(threshold2, threshold2, Size(5, 5), 1.2);
			
			findContours(threshold2, konturok, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			//imshow("Threshold_after_blur", threshold2);
			vector<vector<Point> > convexHulls(konturok.size());
			for (int i = 0; i < konturok.size(); i++) {
				convexHull(konturok[i], convexHulls[i]);
			}
			//kontur_rajzol(threshold2.size(), convexHulls, "konvex cuccok kirajzolasa");
			for (auto &konvex_fosok : convexHulls) {
				Szamlalok kocsi(konvex_fosok);
				if (kocsi.currentBoundingRect.width > 60 &&
					kocsi.currentBoundingRect.height > 60 &&
					kocsi.atlo>100) {
					jelenlegikeretek.push_back(kocsi);


				}
			}
			Mat frameCopy = frame1.clone();
			megjelol(jelenlegikeretek, frameCopy);
			resize(frameCopy, frameCopy, Size(frameCopy.cols / 2, frameCopy.rows / 2));
			namedWindow("Video", CV_WINDOW_AUTOSIZE);
			imshow("Video", frameCopy);
			char filename[128];
			sprintf(filename, "d:\\mappa\\frame_%06d.png", frame_count);
			cv::imwrite(filename, frameCopy);
			frame_count++;

			switch (waitKey(100)) {

			case 27: //'esc' key has been pressed, exit program.
				return 0;
			case 112: //'p' has been pressed. this will pause/resume the code.
				szunet = !szunet;
				if (szunet == true) {
					cout << "Code paused, press 'p' again to resume" << endl;
					while (szunet == true) {
						//stay in this loop until 
						switch (waitKey()) {
							//a switch statement inside a switch statement? Mind blown.
						case 112:
							//change pause back to false
							szunet = false;
							cout << "Code resumed." << endl;
							break;
						}
					}
				}


			}











			jelenlegikeretek.clear();



		}
		cap.release();
	}

	return 0;
}