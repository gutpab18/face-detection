#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class FaceDetector {
public:
    FaceDetector(const String& faceCascadePath, const String& eyesCascadePath, int cameraDevice);

    void run();

private:
    CascadeClassifier faceCascade;
    CascadeClassifier eyesCascade;
    VideoCapture capture;

    void detectFaces(Mat frame);
    void displayFaces(Mat frame, const std::vector<Rect>& faces);
    void displayEyes(Mat frame, const std::vector<Rect>& eyes, const Rect& face);
};

FaceDetector::FaceDetector(const String& faceCascadePath, const String& eyesCascadePath, int cameraDevice) {
    String faceCascadeName = samples::findFile(faceCascadePath);
    String eyesCascadeName = samples::findFile(eyesCascadePath);

    if (!faceCascade.load(faceCascadeName)) {
        cout << "--(!)Error loading face cascade\n";
        exit(EXIT_FAILURE);
    }

    if (!eyesCascade.load(eyesCascadeName)) {
        cout << "--(!)Error loading eyes cascade\n";
        exit(EXIT_FAILURE);
    }

    capture.open(cameraDevice);

    if (!capture.isOpened()) {
        cout << "--(!)Error opening video capture\n";
        exit(EXIT_FAILURE);
    }
}

void FaceDetector::run() {
    Mat frame;

    while (capture.read(frame)) {
        if (frame.empty()) {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }

        detectFaces(frame);

        if (waitKey(10) == 27) {
            break; // escape
        }
    }
}

void FaceDetector::detectFaces(Mat frame) {
    Mat frameGray;
    cvtColor(frame, frameGray, COLOR_BGR2GRAY);
    equalizeHist(frameGray, frameGray);

    std::vector<Rect> faces;
    faceCascade.detectMultiScale(frameGray, faces);

    for (const Rect& face : faces) {
        displayFaces(frame, { face });

        Mat faceROI = frameGray(face);
        std::vector<Rect> eyes;
        eyesCascade.detectMultiScale(faceROI, eyes);

        displayEyes(frame, eyes, face);
    }
}

void FaceDetector::displayFaces(Mat frame, const std::vector<Rect>& faces) {
    for (const Rect& face : faces) {
        Point center(face.x + face.width / 2, face.y + face.height / 2);
        ellipse(frame, center, Size(face.width / 2, face.height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
    }
}

void FaceDetector::displayEyes(Mat frame, const std::vector<Rect>& eyes, const Rect& face) {
    for (const Rect& eye : eyes) {
        Point eyeCenter(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
        int radius = cvRound((eye.width + eye.height) * 0.25);
        circle(frame, eyeCenter, radius, Scalar(255, 0, 0), 4);
    }

    imshow("Capture - Face detection", frame);
}

int main(int argc, const char **argv) {
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|/usr/local/Cellar/opencv/4.8.1_5/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
                             "{eyes_cascade|/usr/local/Cellar/opencv/4.8.1_5/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
                             "{camera|0|Camera device number.}");

    parser.about(
            "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
            "You can use Haar or LBP features.\n\n");

    parser.printMessage();

    FaceDetector faceDetector(parser.get<String>("face_cascade"), parser.get<String>("eyes_cascade"), parser.get<int>("camera"));
    faceDetector.run();

    return 0;
}
