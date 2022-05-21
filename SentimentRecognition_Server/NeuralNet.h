#pragma once

#include <conio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <limits>

#define max_(a,b) (((a) > (b)) ? (a) : (b))

#undef max

using namespace std;

double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}
double SigmoidDiff(double x)
{
	return Sigmoid(x)*(1 - Sigmoid(x));
}


class NeuralNet
{

private:
	class Neuron
	{
	public:
		vector<double> weights;
		vector<Neuron*> nextLayer, prevLayer;

		double weightsDelta = 0;

		double value = 0;

		Neuron() {}

		void PrintWeights()
		{
			cout << "_____________" << endl;
			cout << "Value: " << value << endl;
			if (weights.size() > 0) {
				cout << "Weights: ";
				for (int i = 0; i < weights.size(); i++)
				{
					cout << "[" << weights[i] << "], ";
				}
			}
			cout << endl;

		}

		//Not for the last layer neurons!
		double CalculateAverageError()
		{
			double sum = 0;
			for (int i = 0; i < nextLayer.size(); i++)
			{
				sum += nextLayer[i]->weightsDelta * weights[i]; // error = weightsDelta * weight
			}
			sum /= nextLayer.size(); //Average
			return sum;
		}

		void CalculateValue(int neuronNumber)
		{
			double sum = 0;
			for (int i = 0; i < prevLayer.size(); i++)
			{
				sum += prevLayer[i]->value * prevLayer[i]->weights[neuronNumber];
			}
			value = Sigmoid(sum);
		}

		Neuron(double value, vector<Neuron*> nextLayer = {}, vector<double> weights = {})
		{
			this->value = value;
			this->nextLayer = nextLayer;
			this->weights = weights;
		}

	};

	fstream m_WeightsFile;

	vector<Neuron*> m_OutputLayer, m_InputLayer;

	vector<vector<Neuron*>> m_HiddenLayers;

	int m_MaxNeuronsInTheLayer = 0;

	const double m_LearningRate = 0.05;
public:

	void CalculateValues()
	{
		for (int i = 0; i < m_HiddenLayers.size(); i++) {
			for (int j = 0; j < m_HiddenLayers[i].size(); j++) {
				m_HiddenLayers[i][j]->CalculateValue(j);
			}
		}

		for (int i = 0; i < m_OutputLayer.size(); i++) {
			m_OutputLayer[i]->CalculateValue(i);
		}
	}

	void SetInputValues(vector<double> inputValaues)
	{
		for (int i = 0; i < inputValaues.size(); i++) {
			m_InputLayer[i]->value = inputValaues[i];
		}

		CalculateValues();
	}

	NeuralNet(int inputNeuronsAmount, vector<int> hiddenLayersNeuronsAmount, int outputNeuronsAmount)
	{
		//Fill output layer
		for (int i = 0; i < outputNeuronsAmount; i++) {
			m_OutputLayer.push_back(new Neuron(0));
		}

		vector<double> weightsTemp;

		//Allocate memory for the vector of hidden layers
		m_HiddenLayers.resize(hiddenLayersNeuronsAmount.size());


		//Fill the last hidden layer
		for (int i = 0; i < hiddenLayersNeuronsAmount[hiddenLayersNeuronsAmount.size() - 1]; i++) {
			for (int j = 0; j < outputNeuronsAmount; j++) {
				weightsTemp.push_back((double)(rand() % 199 - 99) / 100);
			}
			m_HiddenLayers[hiddenLayersNeuronsAmount.size() - 1].push_back(new Neuron(0, m_OutputLayer, weightsTemp));
			weightsTemp.clear();
		}

		//Fill hidden layers (except last)
		for (int i = hiddenLayersNeuronsAmount.size() - 2; i >= 0; i--) {
			for (int j = 0; j < hiddenLayersNeuronsAmount[i]; j++) {
				for (int k = 0; k < hiddenLayersNeuronsAmount[i + 1]; k++) {
					weightsTemp.push_back((double)(rand() % 199 - 99) / 100);
				}
				m_HiddenLayers[i].push_back(new Neuron(0, m_HiddenLayers[i + 1], weightsTemp));
				weightsTemp.clear();
			}
		}

		//Fill the input layer
		for (int i = 0; i < inputNeuronsAmount; i++) {
			for (int j = 0; j < hiddenLayersNeuronsAmount[0]; j++) {
				weightsTemp.push_back((double)(rand() % 199 - 99) / 100);
			}
			m_InputLayer.push_back(new Neuron(0.5, m_HiddenLayers[0], weightsTemp));
			weightsTemp.clear();
		}



		//Set prevLayer var
		for (int i = 1; i < m_HiddenLayers.size(); i++) //1, not 0 (without input layer)
		{
			for (int j = 0; j < m_HiddenLayers[i].size(); j++)
			{
				m_HiddenLayers[i][j]->prevLayer = m_HiddenLayers[i - 1];
			}
		}

		for (int i = 0; i < m_HiddenLayers[0].size(); i++)
		{
			m_HiddenLayers[0][i]->prevLayer = m_InputLayer;
		}

		for (int i = 0; i < outputNeuronsAmount; i++)
		{
			m_OutputLayer[i]->prevLayer = m_HiddenLayers[m_HiddenLayers.size() - 1];
		}

		for (int i = 0; i < hiddenLayersNeuronsAmount.size(); i++)
		{
			if (hiddenLayersNeuronsAmount[i] > m_MaxNeuronsInTheLayer)
				m_MaxNeuronsInTheLayer = hiddenLayersNeuronsAmount[i];
		}

		m_MaxNeuronsInTheLayer = max_(max_(m_MaxNeuronsInTheLayer, inputNeuronsAmount), max_(m_MaxNeuronsInTheLayer, outputNeuronsAmount));
		CalculateValues();
	}

	int GetMaxOutputValueIndex()
	{
		double maxValue = -2;
		int maxIndex;
		for (int i = 0; i < m_OutputLayer.size(); i++)
		{
			if (m_OutputLayer[i]->value > maxValue)
			{
				maxValue = m_OutputLayer[i]->value;
				maxIndex = i;
			}
		}

		return maxIndex;
	}

	void Train(vector<double> expectedValues)
	{
		Neuron *curNeuron;

		double actualValue = 0;
		double error = 0;

		//Calculate weights to the output layer
		for (int i = 0; i < m_OutputLayer.size(); i++) { //Iterate over output layer neurons
			curNeuron = m_OutputLayer[i];
			actualValue = curNeuron->value;
			error = actualValue - expectedValues[i]; //error = actualValue - expectedValue
			curNeuron->weightsDelta = error * SigmoidDiff(actualValue); //wightsDelta = error * sigmoid(actualValue)dx

			for (int j = 0; j < curNeuron->prevLayer.size(); j++) //Iterate over the last hidden layer neurons and change it's weights
			{
				curNeuron->prevLayer[j]->weights[i] -= curNeuron->prevLayer[j]->value * curNeuron->weightsDelta * m_LearningRate; //weight -= value * weightsDelta * learningRate
			}
		}

		CalculateValues();

		//Calculate weights to the hidden layers
		for (int i = m_HiddenLayers.size() - 1; i >= 0; i--) //Iterate over all hidden layers
		{
			for (int j = 0; j < m_HiddenLayers[i].size(); j++) //Iterate over the current hidden layer
			{
				curNeuron = m_HiddenLayers[i][j];

				error = curNeuron->CalculateAverageError();
				curNeuron->weightsDelta = error * SigmoidDiff(curNeuron->value);

				for (int k = 0; k < curNeuron->prevLayer.size(); k++)
				{
					curNeuron->prevLayer[k]->weights[j] -= curNeuron->prevLayer[k]->value * curNeuron->weightsDelta * m_LearningRate; //weight -= value * weightsDelta * learningRate
				}
			}
			CalculateValues();
		}

		CalculateValues();
	}

	void ClearFile()
	{
		m_WeightsFile.open("weights.txt", fstream::out);
		m_WeightsFile.close();
	}

	void SaveWeightsToFile()
	{
		m_WeightsFile.open("weights.txt", fstream::out);
		for (int i = 0; i < m_InputLayer.size(); i++)
		{
			for (int j = 0; j < m_InputLayer[i]->weights.size(); j++)
			{
				m_WeightsFile << m_InputLayer[i]->weights[j] << endl;
			}
		}
		for (int i = 0; i < m_HiddenLayers.size(); i++)
		{
			for (int j = 0; j < m_HiddenLayers[i].size(); j++)
			{
				for (int k = 0; k < m_HiddenLayers[i][j]->weights.size(); k++)
				{
					m_WeightsFile << m_HiddenLayers[i][j]->weights[k] << endl;
				}
			}
		}
		m_WeightsFile.close();
	}
	void GetWeightsFromFile()
	{
		m_WeightsFile.open("weights.txt", fstream::in);
		for (int i = 0; i < m_InputLayer.size(); i++)
		{
			for (int j = 0; j < m_InputLayer[i]->weights.size(); j++)
			{
				m_WeightsFile >> m_InputLayer[i]->weights[j];
			}
		}
		for (int i = 0; i < m_HiddenLayers.size(); i++)
		{
			for (int j = 0; j < m_HiddenLayers[i].size(); j++)
			{
				for (int k = 0; k < m_HiddenLayers[i][j]->weights.size(); k++)
				{
					m_WeightsFile >> m_HiddenLayers[i][j]->weights[k];
				}
			}
		}
		m_WeightsFile.close();
		CalculateValues();
	}
	void PrintOutputValue(bool printInput = true)
	{

		CalculateValues();
		if (printInput)
		{
			cout << "--------" << endl;
			cout << "Input: " << endl;
			for (int i = 0; i < m_InputLayer.size(); i++) {
				cout << i << ")" << m_InputLayer[i]->value << "; ";
			}
			cout << endl;
		}
		cout << "Output: " << endl;
		for (int i = 0; i < m_OutputLayer.size(); i++) {
			cout << i << ")" << m_OutputLayer[i]->value << "; ";
		}
		cout << endl << "--------" << endl;

	}

	double GetOutputNeuronValue(int neuronNumber)
	{
		if (neuronNumber < m_OutputLayer.size())
			return  m_OutputLayer[neuronNumber]->value;
		else
			return 0;
	}

	void PrintNetwork()
	{
		CalculateValues();
		cout << "----------------------" << endl;
		cout << "Input layer:" << endl;
		for (int i = 0; i < m_InputLayer.size(); i++)
		{
			cout << "Value [" << m_InputLayer[i]->value << "]. Weights: ";
			for (int j = 0; j < m_InputLayer[i]->weights.size(); j++)
			{
				cout << m_InputLayer[i]->weights[j] << "; ";
			}
			cout << endl;
		}
		cout << "---" << endl;

		for (int i = 0; i < m_HiddenLayers.size(); i++)
		{
			cout << "Hidden layer " << i << ":" << endl;
			for (int j = 0; j < m_HiddenLayers[i].size(); j++)
			{
				cout << "Value [" << m_HiddenLayers[i][j]->value << "]. Weights: ";
				for (int k = 0; k < m_HiddenLayers[i][j]->weights.size(); k++)
				{
					cout << m_HiddenLayers[i][j]->weights[k] << "; ";
				}
				cout << endl;
			}
		}
		cout << "---" << endl;

		cout << "Output layer:" << endl;
		for (int i = 0; i < m_OutputLayer.size(); i++)
		{
			cout << "Value [" << m_OutputLayer[i]->value << "]";
			cout << endl;
		}
		cout << "----------------------" << endl;
		cout << endl;
	}
};




