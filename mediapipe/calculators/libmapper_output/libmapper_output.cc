#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include <mapper/mapper_cpp.h>

namespace mediapipe
{

    class libmapper_output : public CalculatorBase
    {

    private:
        mapper::Device dev; // A global Device

    public:
        libmapper_output() : dev("libmapper_output"){};

        ~libmapper_output(){};

        bool is_debug = true; // Set this variable to false when optimizing for speed

        static ::mediapipe::Status GetContract(CalculatorContract *cc)
        {

            cc->Inputs().Tag("LANDMARKS").Set<mediapipe::NormalizedLandmarkList>();

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Open(CalculatorContext *cc)
        {

            // Add a new libmapper device
            LOG(INFO) << "Adding signals to libmapper device!";

            // Add palm signal to libmapper device
            dev.add_signal(MPR_DIR_OUT, "output_palm_landmark", 1, MPR_FLT); // Length might need to be 3, double check the docs

            // Add fingertip signals to libmapper device
            dev.add_signal(MPR_DIR_OUT, "thumb_fingertip_landmark", 1, MPR_FLT);
            dev.add_signal(MPR_DIR_OUT, "index_fingertip_landmark", 1, MPR_FLT);
            dev.add_signal(MPR_DIR_OUT, "middle_finger_landmark", 1, MPR_FLT);
            dev.add_signal(MPR_DIR_OUT, "ring_finger_landmark", 1, MPR_FLT);
            dev.add_signal(MPR_DIR_OUT, "pinky_finger_landmark", 1, MPR_FLT);

            // Poll device to get it up and running
            while (!dev.ready())
            {
                dev.poll(10);
            }

            LOG(INFO) << "libmapper device successfully setup!";
            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Process(CalculatorContext *cc)
        {
            // LOG(INFO) << "Polling Device: " << dev;
            // dev.poll();

            /*
                Get the landmark information from the input stream called LANDMARKS
            */
            const auto &input_packet = cc->Inputs().Tag("LANDMARKS");

            if (input_packet.IsEmpty() && is_debug)
            {
                // If in debug mode and there is and empty packet, notify the log.
                LOG(INFO) << "Empty Packet!";
            }

            /*
                Extract the normalized Landmark List from the input packet
            */
            const auto &input_landmarks = input_packet.Get<NormalizedLandmarkList>();

            /*
                Update the libmapper device's signals with the extracted (x,y,z) information for each landmark
            */

            // Update the Palm landmark signal
            std::array<int, 3> palm_vector{{input_landmarks.landmark(0).x(), input_landmarks.landmark(0).y(), input_landmarks.landmark(0).z()}};
            dev.signals()[0].set_value(palm_vector);

            // Update the finger tip signals
            std::array<int, 3> thumb_vector{{input_landmarks.landmark(4).x(), input_landmarks.landmark(4).y(), input_landmarks.landmark(4).z()}};
            dev.signals()[1].set_value(thumb_vector);

            std::array<int, 3> index_vector{{input_landmarks.landmark(8).x(), input_landmarks.landmark(8).y(), input_landmarks.landmark(8).z()}};
            dev.signals()[2].set_value(index_vector);

            std::array<int, 3> middle_vector{{input_landmarks.landmark(12).x(), input_landmarks.landmark(12).y(), input_landmarks.landmark(12).z()}};
            dev.signals()[3].set_value(middle_vector);

            std::array<int, 3> ring_vector{{input_landmarks.landmark(16).x(), input_landmarks.landmark(16).y(), input_landmarks.landmark(16).z()}};
            dev.signals()[4].set_value(ring_vector);

            std::array<int, 3> pinky_vector{{input_landmarks.landmark(20).x(), input_landmarks.landmark(20).y(), input_landmarks.landmark(20).z()}};
            dev.signals()[5].set_value(pinky_vector);

            // TODO: I am making an assumption that signals are accessed in the same order that they are added -- Determine if this is in fact the case, and if not make adjustements to code
            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Close(CalculatorContext *cc)
        {
            LOG(INFO) << "Ending libmapper connection";

            // TODO: Explicitly close the libmapper connection if required

            return ::mediapipe::OkStatus();
        }
    };

    REGISTER_CALCULATOR(libmapper_output);
} // namespace mediapipe