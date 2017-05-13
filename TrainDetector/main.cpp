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
	Rect jelenlegiHatarolo;
	vector<Point> centerPositions;
	Point pNextPos;
	double atlo, kep_meret;
	bool blnStillBeingTracked, blnCurrentMatchFoundOrNewBlob;
	int egymastKoveto;



	Szamlalok(vector<Point> kontur) {
		Point kozeppont_jelenleg;
		jelenlegiKontur = kontur;
		//Point kozeppont_jelenleg;
		jelenlegiHatarolo = boundingRect(jelenlegiKontur);
		kozeppont_jelenleg.x = (jelenlegiHatarolo.x + jelenlegiHatarolo.x + jelenlegiHatarolo.width) / 2;
		kozeppont_jelenleg.y = (jelenlegiHatarolo.y + jelenlegiHatarolo.y + jelenlegiHatarolo.height) / 2;
		centerPositions.push_back(kozeppont_jelenleg);
		atlo = sqrt(pow(jelenlegiHatarolo.width, 2) + pow(jelenlegiHatarolo.width, 2));
		kep_meret = (float)jelenlegiHatarolo.width / (float)jelenlegiHatarolo.height;
		blnStillBeingTracked = true;
		blnCurrentMatchFoundOrNewBlob = true;
	}
	String toString() {
		String s = "Atlo: " + to_string(atlo) +
			" kep_meret: " + to_string(kep_meret) +
			" JelenHat.x: " + to_string(jelenlegiHatarolo.x) +
			" JelenHat.y: " + to_string(jelenlegiHatarolo.y) +
			" x: " + to_string(pNextPos.x) +
			" y: " + to_string(pNextPos.y) +
			" width: " + to_string(jelenlegiHatarolo.width) +
			" height: " + to_string(jelenlegiHatarolo.height);

		return s;
	}
	void nextPosition(void) {
		int nmPoz = (int)centerPositions.size();
		if (nmPoz == 1) {
			//cout << nmPoz << endl;
			pNextPos.x = centerPositions.back().x;
			pNextPos.y = centerPositions.back().y;
		}
		else if (nmPoz == 2) {
			//cout << nmPoz << endl;
			int deltaX = centerPositions[1].x - centerPositions[0].x;
			int deltaY = centerPositions[1].y - centerPositions[0].y;
			pNextPos.x = centerPositions.back().x + deltaX;
			pNextPos.y = centerPositions.back().y + deltaY;
		}
		else if (nmPoz == 3) {
			//cout << nmPoz << endl;
			int sumOfXChanges = ((centerPositions[2].x - centerPositions[1].x) * 2) +
				((centerPositions[1].x - centerPositions[0].x) * 1);

			int deltaX = (int)round((float)sumOfXChanges / 3.0);

			int sumOfYChanges = ((centerPositions[2].y - centerPositions[1].y) * 2) +
				((centerPositions[1].y - centerPositions[0].y) * 1);

			int deltaY = (int)round((float)sumOfYChanges / 3.0);

			pNextPos.x = centerPositions.back().x + deltaX;
			pNextPos.y = centerPositions.back().y + deltaY;
		}
		else if (nmPoz == 4) {
			//cout << nmPoz << endl;
			int sumOfXChanges = ((centerPositions[3].x - centerPositions[2].x) * 3) +
				((centerPositions[2].x - centerPositions[1].x) * 2) +
				((centerPositions[1].x - centerPositions[0].x) * 1);

			int deltaX = (int)std::round((float)sumOfXChanges / 6.0);

			int sumOfYChanges = ((centerPositions[3].y - centerPositions[2].y) * 3) +
				((centerPositions[2].y - centerPositions[1].y) * 2) +
				((centerPositions[1].y - centerPositions[0].y) * 1);

			int deltaY = (int)std::round((float)sumOfYChanges / 6.0);

			pNextPos.x = centerPositions.back().x + deltaX;
			pNextPos.y = centerPositions.back().y + deltaY;

		}
		else if (nmPoz >= 5) {
			//cout << nmPoz << endl;
			int sumOfXChanges = ((centerPositions[nmPoz - 1].x - centerPositions[nmPoz - 2].x) * 4) +
				((centerPositions[nmPoz - 2].x - centerPositions[nmPoz - 3].x) * 3) +
				((centerPositions[nmPoz - 3].x - centerPositions[nmPoz - 4].x) * 2) +
				((centerPositions[nmPoz - 4].x - centerPositions[nmPoz - 5].x) * 1);

			int deltaX = (int)std::round((float)sumOfXChanges / 10.0);

			int sumOfYChanges = ((centerPositions[nmPoz - 1].y - centerPositions[nmPoz - 2].y) * 4) +
				((centerPositions[nmPoz - 2].y - centerPositions[nmPoz - 3].y) * 3) +
				((centerPositions[nmPoz - 3].y - centerPositions[nmPoz - 4].y) * 2) +
				((centerPositions[nmPoz - 4].y - centerPositions[nmPoz - 5].y) * 1);

			int deltaY = (int)round((float)sumOfYChanges / 10.0);

			pNextPos.x = centerPositions.back().x + deltaX;
			pNextPos.y = centerPositions.back().y + deltaY;

		}
		else {
			// should never get here
			cout << "fasfdsadfasdfasdfasfda" << endl;
		}
	}



};

void kontur_rajzol(Size imgSize, vector<vector<Point>> konturok, String nev) {
	Mat kep(imgSize, CV_8UC3, Scalar(0.0, 0.0, 0.0));
	drawContours(kep, konturok, -1, Scalar(255.0, 255.0, 255.0), -1);
	//resize(kep, kep, Size(kep.cols / 2, kep.rows / 2));
	//namedWindow(nev, CV_WINDOW_AUTOSIZE);
	//imshow(nev, kep);
}
double distanceCalc(Point pont1, Point pont2) {
	int intX = abs(pont1.x - pont2.x);
	int intY = abs(pont1.y - pont2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}
void megjelol(vector<Szamlalok> &kocsik, Mat &frameCopy) {
	for (int i = 0; i < kocsik.size(); i++) {
		if (kocsik[i].blnStillBeingTracked == true) {
			rectangle(frameCopy, kocsik[i].jelenlegiHatarolo, Scalar(144.0, 0.0, 255.0), 10);
			int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = kocsik[i].atlo / 30.0;
			putText(frameCopy, kocsik[i].toString(), kocsik[i].centerPositions.back(), 1, 1, Scalar(255.0, 0, 0), 2);
		}
	}
}
void addCargoToExistingCargos(Szamlalok &currentCargo, vector<Szamlalok> &existingCargos, int &index) {
	existingCargos[index].jelenlegiKontur = currentCargo.jelenlegiKontur;
	existingCargos[index].jelenlegiHatarolo = currentCargo.jelenlegiHatarolo;

	existingCargos[index].centerPositions.push_back(currentCargo.centerPositions.back());

	existingCargos[index].atlo = currentCargo.atlo;
	existingCargos[index].kep_meret = currentCargo.kep_meret;

	existingCargos[index].blnStillBeingTracked = true;
	existingCargos[index].blnCurrentMatchFoundOrNewBlob = true;

}
void addNewCargo(Szamlalok &currentCargo, vector<Szamlalok> &existingCargos) {

	currentCargo.blnCurrentMatchFoundOrNewBlob = true;

	existingCargos.push_back(currentCargo);
}
void matchCurrentCargoToExistingCargos(vector<Szamlalok> &existingCargos, vector<Szamlalok> &currentCargos) {

	for (auto &exist : existingCargos) {

		exist.blnCurrentMatchFoundOrNewBlob = false;
		exist.nextPosition();
	}
	for (auto &current : currentCargos) {

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 10000.0;

		for (unsigned int i = 0; i < existingCargos.size(); i++) {

			if (existingCargos[i].blnStillBeingTracked == true) {

				double dblDistance = distanceCalc(current.centerPositions.back(), existingCargos[i].pNextPos);

				if (dblDistance < dblLeastDistance) {

					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < current.atlo * 0.5) {

			//cout << dblLeastDistance << " : " << current.atlo * 0.5 << endl;
			addCargoToExistingCargos(current, existingCargos, intIndexOfLeastDistance);
		}
		else {

			addNewCargo(current, existingCargos);
		}

	}

	for (auto &exist : existingCargos) {

		if (exist.blnCurrentMatchFoundOrNewBlob == false) {
			exist.egymastKoveto++;
		}

		if (exist.egymastKoveto >= 5) {
			exist.blnStillBeingTracked = false;
		}

	}
}
bool linechecker(vector<Szamlalok> &cargos, int &verticalBorder, int &cargoCount) {
	//cout << "linechecker" << endl;
	bool crossedALine = false;

	for (auto cargo : cargos) {
		
		if (cargo.blnStillBeingTracked == true && cargo.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)cargo.centerPositions.size() - 2;
			int currFrameIndex = (int)cargo.centerPositions.size() - 1;
			//cout << cargo.centerPositions.size() << endl;
			
			 if(cargo.centerPositions[prevFrameIndex].x > verticalBorder
			 && cargo.centerPositions[currFrameIndex].x <= verticalBorder) {
				cargoCount++;
				cout << cargoCount << " cargocount" << endl;
				crossedALine = true;
			}
		}
	}
	return crossedALine;
}
int main() {


	VideoCapture cap;
	//03128288.MP4 03128282.MP4 03128283.MP4
	cap.open("d:\\Programok\\Projects\\Visual Studio\\cutted_videos\\vonat1.MP4");
	Point vonal[2];

	Mat frame1, frame2, grey1, grey2, diff, threshold2, frameCopy;
	Mat kernel3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat kernel7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
	Mat kernel15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));
	int fuggoleges_hatar,
		size = 5,
		adapt_param2 = 13,
		frame_count = 0;
	bool szunet = false;
	bool elsoFrame = true;
	double gaus = 5.3;
	vector<Rect> targets;
	vector<Szamlalok> jelenlegikeretek;
	vector<Szamlalok> vegleges;
	vector<vector<Point> > konturok;
	int trainCount = 0;

	if (!cap.isOpened()) {
		cout << "error reading video file" << endl << endl;
		getchar();
		return 0;
	}
	if (cap.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
		cout << "video contains less then 2 frames" << endl << endl;
		getchar();
		return 0;
	}
	cap.read(frame1);
	cap.read(frame2);
	while (cap.get(CV_CAP_PROP_POS_FRAMES) < cap.get(CV_CAP_PROP_FRAME_COUNT) - 2) {
		if (frame1.empty() || frame2.empty()) {
			return 0;
		}
		grey1 = frame1.clone();
		grey2 = frame2.clone();

		cvtColor(grey1, grey1, COLOR_BGR2GRAY);
		cvtColor(grey2, grey2, COLOR_BGR2GRAY);

		GaussianBlur(grey1, grey1, Size(5, 5), gaus);
		GaussianBlur(grey2, grey2, Size(5, 5), gaus);

		absdiff(grey1, grey2, diff);

		fuggoleges_hatar = (int)round(frame1.cols*0.5);
		vonal[0].x = fuggoleges_hatar;
		vonal[0].y = 0;
		vonal[1].x = fuggoleges_hatar;
		vonal[1].y = frame1.rows - 1;

		adaptiveThreshold(diff, threshold2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 21, adapt_param2);

		for (int i = 0; i < 4; i++) {
			dilate(threshold2, threshold2, kernel3x3);
			dilate(threshold2, threshold2, kernel3x3);
			erode(threshold2, threshold2, kernel3x3);
		}



		findContours(threshold2, konturok, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		vector<vector<Point> > convexHulls(konturok.size());
		for (int i = 0; i < konturok.size(); i++) {
			convexHull(konturok[i], convexHulls[i]);
		}

		for (auto &konvextest : convexHulls) {
			Szamlalok kocsi(konvextest);
			if (kocsi.jelenlegiHatarolo.width >= 230 && /*kocsi.jelenlegiHatarolo.width <= 560 &&*/
				kocsi.jelenlegiHatarolo.height >= 260 && /*kocsi.jelenlegiHatarolo.height <= 560&&*/
				kocsi.atlo > 250) {
				jelenlegikeretek.push_back(kocsi);
			}
		}
		if (elsoFrame == true) {
			for (auto &current : jelenlegikeretek) {
				vegleges.push_back(current);
			}
		}
		else {
			matchCurrentCargoToExistingCargos(vegleges, jelenlegikeretek);

		}

		///////////////////////////////////////////////////////////////////////////////////////
		frameCopy = frame2.clone();
		megjelol(vegleges, frameCopy);


		bool crossed = linechecker(vegleges, fuggoleges_hatar, trainCount);
		
		if (crossed == true) {
			//cout << "red" << endl;
			line(frameCopy, vonal[0], vonal[1], Scalar(200.0, 0.0, 0.0), 10);
		}
		else {
			//cout << "green" << endl;
			line(frameCopy, vonal[0], vonal[1], Scalar(0.0, 200.0, 0.0), 10);
		}


		resize(frameCopy, frameCopy, Size(frameCopy.cols / 2, frameCopy.rows / 2));
		namedWindow("Video", CV_WINDOW_AUTOSIZE);
		imshow("Video", frameCopy);
		//char filename[128];
		//sprintf(filename, "frame_%06d.png", frame_count);
		//imwrite(filename, frameCopy);
///////////////////////////////////////////////////////////////////////////////////////
		jelenlegikeretek.clear();
		////////////////////////////////////////////////////////////////
		frame1 = frame2.clone();
		if ((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) <
			cap.get(CV_CAP_PROP_FRAME_COUNT)) {
			cap.read(frame2);
		}
		else {
			cout << "The end" << endl;
			break;
		}
		elsoFrame = false;
		///////////////////////////////////////////////////////////////////////////
		switch (waitKey(1)) {
		case 113: if (gaus > 0.0) { gaus = gaus - 0.1; } cout << "gaus-- " << gaus << endl; break;
		case 119: gaus = gaus + 0.1; cout << "gaus++ " << gaus << endl; break;
		case 97: if (adapt_param2 > 0) { adapt_param2--; cout << "adapt_param2-- " << adapt_param2 << endl; } break;
		case 115: adapt_param2++;  cout << "adapt_param2++ " << adapt_param2 << endl; break;
		case 27:
			return 0;
		case 112:
			szunet = !szunet;
			if (szunet == true) {
				cout << "Code paused, press 'p' again to resume" << endl;
				while (szunet == true) {
					switch (waitKey()) {
					case 112:
						szunet = false;
						cout << "Code resumed." << endl;
						break;
					}
				}
			}
		}

		//vegleges.clear();
	}
	return 0;
}