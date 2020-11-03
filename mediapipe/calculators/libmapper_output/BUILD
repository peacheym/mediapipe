cc_library(name="libmapper_output",
    srcs = glob(["*.cc", "*.h", "libmapper.so"]),
    hdrs = glob(["*.h"]),
    visibility=["//visibility:public"],
    
    deps=["//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",],
    alwayslink=1,
)