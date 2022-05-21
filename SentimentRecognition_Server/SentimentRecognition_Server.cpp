#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "SocketUtility.cpp"
#include "Converter.cpp"
#include "NeuralNet.h"

int main()
{
    SocketUtility* socketUtility = new SocketUtility();
    socketUtility->Listen();
    NeuralNet* neuralNet = new NeuralNet(24 * 24, { 50 }, 1);
    neuralNet->GetWeightsFromFile();

    char dataFromClient[1024];

    std::string responceStr;
    while (socketUtility->GetData(dataFromClient))
    {
        dataFromClient[24 * 24] = '\0';
        neuralNet->SetInputValues(Converter::ConvertBytesToVector(dataFromClient, 24 * 24));
        neuralNet->CalculateValues();
        if (neuralNet->GetOutputNeuronValue(0) < 0.3f)
            responceStr = "SAD";
        else if (neuralNet->GetOutputNeuronValue(0) < 0.7f)
            responceStr = "NEUTRAL";
        else
            responceStr = "HAPPY";

        socketUtility->SendData(responceStr);
        
    }


    return 0;
}

