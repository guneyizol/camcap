#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <queue>
#include <thread>


using namespace std;
using namespace cv;


class PrintToScreen {
    private:
        char& key;
        bool& isGray;
        std::queue<Mat>& frames;
    
    public:
        PrintToScreen(
            char& key_,
            bool& isGray_,
            std::queue<Mat>& frames_
        ):
            key{key_},
            isGray{isGray_},
            frames{frames_} {

            // empty body
        }

        void operator()() {
            while (key != 'q') {
                if (!frames.empty()) {
                    Mat frame = frames.front();
                    frames.pop();

                    if (isGray) {
                        cvtColor(frame, frame, COLOR_RGB2GRAY);
                    }

                    imshow("Live", frame);
                }
            }
        }
};


int main() {
    VideoCapture cap;
    cap.open(0, cv::CAP_ANY);

    if (!cap.isOpened()) {
        cerr << "Error. Unable to open camera\n";
        return -1;
    }

    cout << "Press 'g' for grayscale" << endl
         << "Press 'c' for colored video" << endl
         << "Press 'q' to terminate" << endl;
    
    char key = 'c';  // print colored frames by default
    bool isGray = false;
    std::queue<Mat> frames;
    PrintToScreen pts(key, isGray, frames);

    thread t{pts};

    Mat frame;
    while (key != 'q') {
        cap.read(frame);

        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // convert to grayscale when 'g' is pressed
        if (key == 'g' && !isGray) {
            isGray = true;
        // convert to color when 'c' is pressed
        } else if (key == 'c' && isGray) {
            isGray = false;
        }

        frames.push(frame);
        key = waitKey(1);
    }

    if (t.joinable())
        t.join();
    
    return 0;
}
