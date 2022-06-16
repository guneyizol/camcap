#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <zmq.hpp>

#include <iostream>


using namespace std;
using namespace cv;


int main() {
    VideoCapture cap;
    cap.open(0, cv::CAP_ANY);

    if (!cap.isOpened()) {
        cerr << "Error. Unable to open camera\n";
        return -1;
    }

    zmq::context_t context{2};
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    
    Mat frame;
    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        cap.read(frame);
        

        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        size_t frameSize = frame.total() * frame.elemSize();
        zmq::message_t reply{frame.data, frameSize};
        socket.send(reply, zmq::send_flags::none);
    }
    
    return 0;
}
