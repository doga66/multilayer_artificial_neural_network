#pragma once

namespace deneme2 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Diagnostics;  // Debug::WriteLine için gerekli namespace

    // Perceptron sýnýfý
    public ref class Perceptron
    {
    private:
        array<array<float>^>^ weights; // Her sýnýf için ayrý aðýrlýk seti
        float bias;
        float learningRate;
        float epoch;

    public:
        Perceptron(int inputSize, float learningRate, float epcoh)
        {
            this->weights = gcnew array<array<float>^>(4);  // 4 sýnýf için
            for (int i = 0; i < 4; i++)  // Her sýnýf için aðýrlýklar
            {
                this->weights[i] = gcnew array<float>(inputSize);
            }
            this->bias = 0;
            this->learningRate = learningRate;
            this->epoch = epcoh;
        }

        // Aktivasyon fonksiyonu (Heaviside step function)
        int ActivationFunction(float sum)
        {
            return (sum >= 0) ? 1 : 0;
        }

        int Perceptron::Predict(array<int>^ input)
        {
            float maxSum = -99999999;
            int predictedClass = -1;

            for (int k = 0; k < 4; k++) // For each class
            {
                float sum = 0;
                for (int j = 0; j < input->Length; j++)
                {
                    sum += input[j] * weights[k][j];
                }
                sum += bias;

                if (sum > maxSum)
                {
                    maxSum = sum;
                    predictedClass = k;
                }
            }

            return predictedClass;
        }


        // Perceptron'u eðit
        void Train(System::Collections::Generic::List<array<int>^>^ inputs, System::Collections::Generic::List<array<int>^>^ outputs, int epochs)
        {
            Random^ rand = gcnew Random();

            // Baþlangýç aðýrlýklarýný rastgele seç
            for (int i = 0; i < 4; i++) // Her sýnýf için
            {
                for (int j = 0; j < inputs[0]->Length; j++)  // Input boyutu kadar
                {
                    weights[i][j] = (float)(rand->NextDouble() * 2.0 - 1.0); // -1.0 ile 1.0 arasýnda rastgele bir deðer
                }
            }

            // Eðitim döngüsü
            for (int epoch = 0; epoch < epochs; epoch++)
            {
                for (int i = 0; i < inputs->Count; i++) // Tüm eðitim verileri için
                {
                    array<int>^ input = inputs[i];
                    array<int>^ output = outputs[i];

                    float maxSum = -99999999; // Baþlangýçta en küçük deðeri ver
                    int predictedClass = -1; // Baþlangýçta bir sýnýf belirle

                    // Her sýnýf için tahmin yap
                    for (int k = 0; k < 4; k++) // Her sýnýf için
                    {
                        float sum = 0;
                        for (int j = 0; j < input->Length; j++)
                        {
                            sum += input[j] * weights[k][j]; // Aðýrlýklarla çarpma
                        }
                        sum += bias; // Bias ekle

                        int prediction = ActivationFunction(sum); // Tahmin yap
                        int error = output[k] - prediction; // Hata hesapla

                        // Aðýrlýklarý güncelle
                        for (int j = 0; j < input->Length; j++)
                        {
                            weights[k][j] += learningRate * error * input[j];
                        }
                        bias += learningRate * error; // Bias'ý güncelle

                        // Maximum selector: Eðer bu sýnýf daha yüksek bir sonuç veriyorsa, bunu seç
                        if (sum > maxSum)
                        {
                            maxSum = sum;
                            predictedClass = k;
                        }
                    }
                    // Tahmin edilen sýnýfý yazdýr
                    Console::WriteLine("Input: (" + String::Join(", ", input) + ") | Predicted Class: " + predictedClass + " | Expected Output: " + GetExpectedClass(output));
                }
            }
        }

        int GetExpectedClass(array<int>^ output)
        {
            for (int i = 0; i < output->Length; i++)
            {
                if (output[i] == 1)
                    return i; // Sýnýfý döndür
            }
            return -1; // Eðer geçerli bir sýnýf yoksa, -1 döndür
        }

        // Eðitilmiþ aðýrlýklarý ve bias'ý döndür
        array<array<float>^>^ GetWeights()
        {
            return weights;
        }

        float GetBias()
        {
            return bias;
        }
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            pointsClass0 = gcnew System::Collections::Generic::List<System::Drawing::Point>();
            pointsClass1 = gcnew System::Collections::Generic::List<System::Drawing::Point>();
            pointsClass2 = gcnew System::Collections::Generic::List<System::Drawing::Point>();
            pointsClass3 = gcnew System::Collections::Generic::List<System::Drawing::Point>();
            trainingInputs = gcnew System::Collections::Generic::List<array<int>^>();
            trainingOutputs = gcnew System::Collections::Generic::List<array<int>^>();

            learningRateTextBox = gcnew System::Windows::Forms::TextBox();
            learningRateTextBox->Location = System::Drawing::Point(10, 40); // Ýstediðiniz konumu ayarlayýn
            learningRateTextBox->Text = "0.1"; // Varsayýlan deðer
            this->Controls->Add(learningRateTextBox);

            epochTextBox = gcnew System::Windows::Forms::TextBox();
            epochTextBox->Location = System::Drawing::Point(10, 60); // Ýstediðiniz konumu ayarlayýn
            epochTextBox->Text = "999"; // Varsayýlan deðer
            this->Controls->Add(epochTextBox);

            perceptron = nullptr;
            this->MouseClick += gcnew MouseEventHandler(this, &MyForm::OnMouseClick);
            this->Paint += gcnew PaintEventHandler(this, &MyForm::OnPaint);
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;
        System::Collections::Generic::List<System::Drawing::Point>^ pointsClass0;
        System::Collections::Generic::List<System::Drawing::Point>^ pointsClass1;
        System::Collections::Generic::List<System::Drawing::Point>^ pointsClass2;
        System::Collections::Generic::List<System::Drawing::Point>^ pointsClass3;
        System::Collections::Generic::List<array<int>^>^ trainingInputs;
        System::Collections::Generic::List<array<int>^>^ trainingOutputs;
        System::Windows::Forms::TextBox^ learningRateTextBox;
        System::Windows::Forms::TextBox^ epochTextBox;
        int currentClass; // 0: Class 0, 1: Class 1, 2: Class 2, 3: Class 3
        Perceptron^ perceptron;

        // Form components
        System::Windows::Forms::Button^ class0Button;
        System::Windows::Forms::Button^ class1Button;
        System::Windows::Forms::Button^ class2Button;
        System::Windows::Forms::Button^ class3Button;
        System::Windows::Forms::Button^ trainButton;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->ClientSize = System::Drawing::Size(640, 480);
            this->Text = L"MyForm";
            this->Padding = System::Windows::Forms::Padding(0);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;

            // Button for Class 0
            this->class0Button = gcnew System::Windows::Forms::Button();
            this->class0Button->Location = System::Drawing::Point(10, 10);
            this->class0Button->Text = L"Class 0";
            this->class0Button->Click += gcnew EventHandler(this, &MyForm::Class0Button_Click);
            this->Controls->Add(this->class0Button);

            // Button for Class 1
            this->class1Button = gcnew System::Windows::Forms::Button();
            this->class1Button->Location = System::Drawing::Point(90, 10);
            this->class1Button->Text = L"Class 1";
            this->class1Button->Click += gcnew EventHandler(this, &MyForm::Class1Button_Click);
            this->Controls->Add(this->class1Button);

            // Button for Class 2
            this->class2Button = gcnew System::Windows::Forms::Button();
            this->class2Button->Location = System::Drawing::Point(270, 10);
            this->class2Button->Text = L"Class 2";
            this->class2Button->Click += gcnew EventHandler(this, &MyForm::Class2Button_Click);
            this->Controls->Add(this->class2Button);

            // Button for Class 3
            this->class3Button = gcnew System::Windows::Forms::Button();
            this->class3Button->Location = System::Drawing::Point(360, 10);
            this->class3Button->Text = L"Class 3";
            this->class3Button->Click += gcnew EventHandler(this, &MyForm::Class3Button_Click);
            this->Controls->Add(this->class3Button);

            // Train button
            this->trainButton = gcnew System::Windows::Forms::Button();
            this->trainButton->Location = System::Drawing::Point(180, 10);
            this->trainButton->Text = L"Train";
            this->trainButton->Click += gcnew EventHandler(this, &MyForm::TrainButton_Click);
            this->Controls->Add(this->trainButton);
        }

        void OnMouseClick(Object^ sender, MouseEventArgs^ e)
        {
            int x = e->X - 320;
            int y = e->Y - 240;

            if (currentClass == 0)
            {
                pointsClass0->Add(System::Drawing::Point(x, y));
                trainingInputs->Add(gcnew array<int> { x, y });
                trainingOutputs->Add(gcnew array<int> { 1, 0, 0, 0 });
                Debug::WriteLine("Added Class 0: (" + x + ", " + y + ")");
            }
            else if (currentClass == 1)
            {
                pointsClass1->Add(System::Drawing::Point(x, y));
                trainingInputs->Add(gcnew array<int> { x, y });
                trainingOutputs->Add(gcnew array<int> { 0, 1, 0, 0 });
                Debug::WriteLine("Added Class 1: (" + x + ", " + y + ")");
            }
            else if (currentClass == 2)
            {
                pointsClass2->Add(System::Drawing::Point(x, y));
                trainingInputs->Add(gcnew array<int> { x, y });
                trainingOutputs->Add(gcnew array<int> { 0, 0, 1, 0 });
                Debug::WriteLine("Added Class 2: (" + x + ", " + y + ")");
            }
            else if (currentClass == 3)
            {
                pointsClass3->Add(System::Drawing::Point(x, y));
                trainingInputs->Add(gcnew array<int> { x, y });
                trainingOutputs->Add(gcnew array<int> { 0, 0, 0, 1 });
                Debug::WriteLine("Added Class 3: (" + x + ", " + y + ")");
            }

            Invalidate(); // Formu yeniden çizmek için
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;

            // Koordinat sistemini çiz (X ve Y eksenleri)
            Pen^ axisPen = gcnew Pen(Color::Black);
            // Yatay eksen (X ekseni)
            g->DrawLine(axisPen, 0, 240, this->ClientSize.Width, 240);  // Yatay eksen (x=0, y=240)
            // Dikey eksen (Y ekseni)
            g->DrawLine(axisPen, 320, 0, 320, this->ClientSize.Height);  // Dikey eksen (x=320, y=0)

            // Eksenlerin üzerindeki iþaretleri (x ve y deðerlerini) çiz
            for (int i = -320; i <= 320; i += 40)  // X eksenindeki iþaretler
            {
                g->DrawLine(axisPen, 320 + i, 240 - 5, 320 + i, 240 + 5);
            }
            for (int i = -240; i <= 240; i += 40)  // Y eksenindeki iþaretler
            {
                g->DrawLine(axisPen, 320 - 5, 240 + i, 320 + 5, 240 + i);
            }

            // Her sýnýfýn noktalarýný çiz
            for each (System::Drawing::Point point in pointsClass0)
            {
                g->FillEllipse(Brushes::Red, point.X + 320, point.Y + 240, 10, 10);
            }
            for each (System::Drawing::Point point in pointsClass1)
            {
                g->FillEllipse(Brushes::Green, point.X + 320, point.Y + 240, 10, 10);
            }
            for each (System::Drawing::Point point in pointsClass2)
            {
                g->FillEllipse(Brushes::Blue, point.X + 320, point.Y + 240, 10, 10);
            }
            for each (System::Drawing::Point point in pointsClass3)
            {
                g->FillEllipse(Brushes::Yellow, point.X + 320, point.Y + 240, 10, 10);
            }

            // Perceptron modeline dayalý sýnýflandýrma çizgilerini çizin
            if (perceptron != nullptr)
            {
                // After perceptron training, draw decision boundaries for each class
                for (int x = -320; x <= 320; x++)
                {
                    for (int y = -240; y <= 240; y++)
                    {
                        array<int>^ input = gcnew array<int> { x, y };
                        int predictedClass = perceptron->Predict(input); // You need to implement this method
                        Color pointColor;

                        // Set color based on predicted class
                        switch (predictedClass)
                        {
                        case 0: pointColor = Color::Red; break;
                        case 1: pointColor = Color::Green; break;
                        case 2: pointColor = Color::Blue; break;
                        case 3: pointColor = Color::Yellow; break;
                        }

                        g->FillRectangle(gcnew SolidBrush(pointColor), x + 320, y + 240, 1, 1);
                    }
                }
            }
        }


        void Class0Button_Click(Object^ sender, EventArgs^ e)
        {
            currentClass = 0;
        }

        void Class1Button_Click(Object^ sender, EventArgs^ e)
        {
            currentClass = 1;
        }

        void Class2Button_Click(Object^ sender, EventArgs^ e)
        {
            currentClass = 2;
        }

        void Class3Button_Click(Object^ sender, EventArgs^ e)
        {
            currentClass = 3;
        }

        void TrainButton_Click(Object^ sender, EventArgs^ e)
        {
            // Get the learning rate and epochs from the text boxes
            float learningRate = System::Single::Parse(learningRateTextBox->Text);
            int epochs = System::Int32::Parse(epochTextBox->Text);

            // Initialize and train the perceptron
            perceptron = gcnew Perceptron(2, learningRate, epochs);  // 2 inputs (x, y)
            perceptron->Train(trainingInputs, trainingOutputs, epochs);

            // Redraw the form after training
            Invalidate();
        }

    };
}
