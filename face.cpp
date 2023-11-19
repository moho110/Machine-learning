#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace dnn;

int main() {
    // Load the pre-trained model and its configuration file
    String modelFile = "path/to/your/model.caffemodel";
    String configFile = "path/to/your/deploy.prototxt";
    Net net = readNetFromCaffe(configFile, modelFile);

    // Load the input image
    String inputImageFile = "path/to/your/input/image.jpg";
    Mat inputImage = imread(inputImageFile);

    // Check if the image is loaded successfully
    if (inputImage.empty()) {
        std::cerr << "Error: Could not read the image file." << std::endl;
        return -1;
    }

    // Resize the image to the required input size for the model
    Size inputSize = Size(224, 224);
    resize(inputImage, inputImage, inputSize);

    // Preprocess the input image (normalize pixel values)
    Mat inputBlob = blobFromImage(inputImage, 1.0, inputSize, Scalar(104, 117, 123));

    // Set the input blob for the neural network
    net.setInput(inputBlob);

    // Forward pass to get the output
    Mat outputBlob = net.forward();

    // Find the index of the class with the highest confidence
    Point classIdPoint;
    double confidence;
    minMaxLoc(outputBlob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);

    int classId = classIdPoint.x;

    // Load class labels
    std::vector<String> classNames;
    String labelFile = "path/to/your/labels.txt";
    std::ifstream ifs(labelFile.c_str());
    if (!ifs.is_open()) {
        std::cerr << "Error: Could not read the labels file." << std::endl;
        return -1;
    }

    std::string line;
    while (getline(ifs, line))
        classNames.push_back(line);

    // Display the result
    std::cout << "Class: " << classNames[classId] << std::endl;
    std::cout << "Confidence: " << confidence * 100 << "%" << std::endl;

    // Save the result as an output image
    putText(inputImage, "Class: " + classNames[classId], Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    putText(inputImage, "Confidence: " + std::to_string(confidence * 100) + "%", Point(10, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);

    imwrite("output.jpg", inputImage);

    return 0;
}
