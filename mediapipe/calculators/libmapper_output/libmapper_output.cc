#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mapper_cpp.h"

namespace mediapipe
{

    class libmapper_output : public CalculatorBase
    {

    private:
        mapper::Device *dev; // A global Device

    public:
        libmapper_output(){};

        ~libmapper_output(){};

        bool is_debug = true; // Set this variable to false when optimizing for speed

        // mapper::Device get_device()

        // {
        //     LOG(INFO) << "Number of Signals !!!: " << dev->signals().size();

        //     LOG(INFO) << dev;

        //     LOG(INFO) << "TEST";

        //     return dev;
        // }

        static ::mediapipe::Status GetContract(CalculatorContract *cc)
        {

            cc->Inputs().Tag("LANDMARKS").Set<mediapipe::NormalizedLandmarkList>();

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Open(CalculatorContext *cc)
        {

            // Add a new libmapper device -> Should this be a global?
            LOG(INFO) << "Creating libmapper device!";

            mapper::Device newDevice("libmapper_output");
            dev = &newDevice;

            dev->add_sig(MPR_DIR_OUT, "output_palm_landmark", 1, MPR_FLT);
            dev->add_sig(MPR_DIR_OUT, "output_palm_landmark2", 1, MPR_FLT);
            dev->add_sig(MPR_DIR_OUT, "output_palm_landmark3", 1, MPR_FLT);

            // Poll device to get it up and running
            while (!dev->ready())
            {
                dev->poll(10);
                LOG(INFO) << "Polling...";
            }

            LOG(INFO) << "Success creating libmapper device!";

            // LOG(INFO) << "Number of Signals: " << dev->signals().size();

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Process(CalculatorContext *cc)
        {
            LOG(INFO) << "Process: " << dev;
            dev->poll();

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

            // TODO: Update libmapper signals here!!!! [ i < input_landmarks.landmark_size() ]

            LOG(INFO) << "TEST ME";
            LOG(INFO) << dev;
            LOG(INFO) << "TEST ME AGAIN";

            LOG(INFO) << "Landmark 6: ("
                      << input_landmarks.landmark(6).x() << "," << input_landmarks.landmark(6).y() << "," << input_landmarks.landmark(6).z() << ")";

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