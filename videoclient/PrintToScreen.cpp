#include <zmq.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    cout << "Press 'g' for grayscale" << endl
         << "Press 'c' for colored video" << endl
         << "Press 'q' to terminate" << endl;
    
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    char key = 'c';  // print colored frames by default
    bool isGray = false;

    while (key != 'q')
    {
        string msg = "send frame";
        zmq::message_t request{&msg, msg.size()};
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        Mat frame{480, 640, 16, reply.data()};

        // convert to grayscale when 'g' is pressed
        if (key == 'g' && !isGray) {
            isGray = true;
        // convert to color when 'c' is pressed
        } else if (key == 'c' && isGray) {
            isGray = false;
        }

        if (isGray)
        {
            cvtColor(frame, frame, COLOR_RGB2GRAY);
        }
        imshow("Live", frame);
        
        key = waitKey(1);
    }
}
