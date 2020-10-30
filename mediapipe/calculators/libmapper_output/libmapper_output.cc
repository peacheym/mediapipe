#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mapper/mapper_cpp.h"

namespace mediapipe
{

    class libmapper_output : public CalculatorBase
    {

    public:
        libmapper_output(){};

        ~libmapper_output(){};

        static ::mediapipe::Status GetContract(CalculatorContract *cc)
        {

            cc->Inputs().Tag("LANDMARKS").Set<mediapipe::NormalizedLandmarkList>();

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Open(CalculatorContext *cc)
        {

            // Add a new libmapper device -> Should this be a global?

            mapper::Device dev("test");

            LOG(INFO) << "HELLO, WORLD!";

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Process(CalculatorContext *cc)
        {
            // LOG(INFO) << "Process";
            const auto &input_packet = cc->Inputs().Tag("LANDMARKS");
            if (input_packet.IsEmpty())
            {
                LOG(INFO) << "Empty Packet!";
            }

            const auto &input_landmarks = input_packet.Get<NormalizedLandmarkList>();

            LOG(INFO) << "Landmark 6: ("
                      << input_landmarks.landmark(6).x() << "," << input_landmarks.landmark(6).y() << "," << input_landmarks.landmark(6).z();

            // for (int i = 0; i < input_landmarks.landmark_size(); ++i)
            // {
            //     const NormalizedLandmark &landmark = input_landmarks.landmark(i);

            // }

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Close(CalculatorContext *cc)
        {

            return ::mediapipe::OkStatus();
        }
    };

    REGISTER_CALCULATOR(libmapper_output);
} // namespace mediapipe