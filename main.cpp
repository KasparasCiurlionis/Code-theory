#include <iostream>
#include <bitset>
#include <time.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

// BMP header structure (simplified)
struct BMPHeader {
    uint16_t fileType;       // File type ("BM")
    uint32_t fileSize;       // File size in bytes
    uint16_t reserved1;      // Reserved
    uint16_t reserved2;      // Reserved
    uint32_t offsetData;     // Offset to start of pixel data
    uint32_t size;           // Header size
    int32_t width;           // Image width
    int32_t height;          // Image height
    uint16_t planes;         // Number of color planes
    uint16_t bitCount;       // Bits per pixel
    uint32_t compression;    // Compression type
    uint32_t imageSize;      // Image size (may be 0 if uncompressed)
};

struct BMPHeader2 {
    uint16_t fileType = 0x4D42; // 'BM' in little-endian
    uint32_t fileSize = 0;      // File size in bytes
    uint32_t reserved = 0;      // Reserved
    uint32_t offsetData = 54;   // Offset to pixel data
    uint32_t size = 40;         // Header size
    int32_t width = 0;          // Image width
    int32_t height = 0;         // Image height
    uint16_t planes = 1;        // Number of planes
    uint16_t bitCount = 24;     // Bits per pixel
    uint32_t compression = 0;   // Compression (0 = uncompressed)
    uint32_t imageSize = 0;     // Image size (can be 0 for uncompressed)
    int32_t xPixelsPerMeter = 0x130B; // Pixels per meter (horizontal)
    int32_t yPixelsPerMeter = 0x130B; // Pixels per meter (vertical)
    uint32_t colorsUsed = 0;    // Number of colors in palette
    uint32_t colorsImportant = 0; // Important colors
};

// Function to read the BMP header
BMPHeader readBMPHeader(std::ifstream& file) {
    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header.fileType), sizeof(header.fileType));
    file.read(reinterpret_cast<char*>(&header.fileSize), sizeof(header.fileSize));
    file.read(reinterpret_cast<char*>(&header.reserved1), sizeof(header.reserved1));
    file.read(reinterpret_cast<char*>(&header.reserved2), sizeof(header.reserved2));
    file.read(reinterpret_cast<char*>(&header.offsetData), sizeof(header.offsetData));
    file.read(reinterpret_cast<char*>(&header.size), sizeof(header.size));
    file.read(reinterpret_cast<char*>(&header.width), sizeof(header.width));
    file.read(reinterpret_cast<char*>(&header.height), sizeof(header.height));
    file.read(reinterpret_cast<char*>(&header.planes), sizeof(header.planes));
    file.read(reinterpret_cast<char*>(&header.bitCount), sizeof(header.bitCount));
    file.read(reinterpret_cast<char*>(&header.compression), sizeof(header.compression));
    file.read(reinterpret_cast<char*>(&header.imageSize), sizeof(header.imageSize));
    return header;
}


int n;
int k;
int m;
double pe;

size_t bitsetSize;
size_t controlSize;


using namespace std;

bool* createGenMatrix();
bool* createControlMatrix(bool* bits);
bool** GenerateAllVectors(int n);
bool isValidBinaryString(string& str);
bool* readGenMatrix();

bool* intToBinary(int num);
int binaryToInt(bool* num);

std::vector<int> generateVectorOfWeight(int m, int w);
void fillVector(std::vector<int>& vec, int w,  int j, int sum);


void uint8ToBoolArray(uint8_t value, bool* boolArray);
uint8_t boolArrayToUint8(const bool* boolArray);

void imageToBinary();
int modifyImage() ;
int modifiedToImage(string binaryFile,string outputImage);

bool* encode(bool* input);
bool* distort(bool* encoded);
bool* decode(bool* encoded);

bool* genMatrix;
bool* contMatrix;
int cosetCount;
bool** syndrome;
int* cosetWieght;


int main()
{
    srand(time(0));
    bool valid=false;
    string inp;
    int inpInt;



    n = 6;
    k = 1;

    pe = 0.0333;

    cout<<"Input n: "<<endl;
    cin >> inpInt;
    n=inpInt;


    cout<<"Input k:"<<endl;
    cin >> inpInt;
    k=inpInt;

    cout<<"Input pe:"<<endl;
    cin >> pe;


    m=k;
    bitsetSize = n*k;
    controlSize = (n-k)*n;


    genMatrix=new bool[bitsetSize];



    cout<<"Use gen matrix in data.txt (y/n):"<<endl;
    cin >> inp;
    if(inp=="y"){
        //gentneration matrix from data.txt is used
        genMatrix = readGenMatrix() ;
    }
    else{
        //a random gent matrix is created
        genMatrix = createGenMatrix();
    }


    cout <<"generation matrix:"<< endl;
    for(int i=0;i<k;i++){
        for(int j=0;j<n;j++){
            cout << (genMatrix[n*i+j]) << " ";
        }
        cout << endl;
    }

    //control matrix is created according to the gen matrix
    contMatrix=new bool[controlSize];
    contMatrix = createControlMatrix(genMatrix);

    cout <<"control matrix:"<< endl;
    for(int i=0;i<n-k;i++){
        for(int j=0;j<n;j++){
            cout << (contMatrix[n*i+j]) << " ";
        }
        cout << endl;
    }


    //how many k size binary vectors there are
    int rows=exp2(m);

    bool** matrix = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new bool[m];
    }
    //generates all k sized vectors for standard table calculation
    matrix=GenerateAllVectors(m);

    int w=1;
    int sizes=exp2(n);
    bool* standardTable=new bool[sizes]; //marks all the already found members so the smallest not found member can be discovered
    for(int i=0;i<exp2(n);i++){
        standardTable[i]=0;
    }

    //generates all weight 1 vectors for possible coset leaders
    std::vector<int> numbers=generateVectorOfWeight(n, w);

    //converts the binary to decimal which will be used to check the standard table position
    for (int num : numbers) {
        bool* matrixa=new bool[n];
        matrixa=intToBinary(num);
    }



    cosetCount=exp2(n-k);//how many coset leader there will be

    bool** leaders = new bool*[cosetCount];//coset leaders
    for (int i = 0; i < cosetCount; ++i) {
        leaders[i] = new bool[n];
    }
    std::cout << "Standard Table"<< std::endl;
    for(int i=0;i<n;i++){
        leaders[0][i]=0;
        cout<<leaders[0][i];
    }
    cout<<" ";
    //generate the first row, addition is made according to the size k matrix
    for(int j=1;j<rows;j++){
        bool* tempMatrix=new bool[n];
        for(int z=0;z<n;z++){
            tempMatrix[z]=0;
        }
        for(int z=0;z<m;z++){
            if(matrix[j][z]==1){
                for(int u=0;u<n;u++){
                    if(genMatrix[n*z+u]==1){
                        tempMatrix[u]=!tempMatrix[u];
                    }
                }
            }
        }

        for(int z=0;z<n;z++){
            cout<<tempMatrix[z];
        }
        cout<<" ";

        int tempNum=binaryToInt(tempMatrix);
        standardTable[tempNum]=1;
    }
    cout<<endl;

    //finding all the other coset leaders by generating the standard table
    for(int i=1;i<cosetCount;i++){
        int leader=0;
        while(w<=n){
            for (auto it = numbers.begin(); it != numbers.end(); ) {
                int key=*it;
                if (standardTable[key]==0) {
                    leader=key;
                    leaders[i]=intToBinary(key);
                    numbers.erase(it);
                    break;
                } else {
                    it = numbers.erase(it);
                }
            }
            if(leader!=0){
                for(int j=0;j<n;j++){
                    cout<<leaders[i][j];
                }
                cout<<" ";
                break;
            }
            else{
                w++;
                numbers=generateVectorOfWeight(n, w);
            }
        }
        for(int j=1;j<rows;j++){
            bool* tempMatrix=new bool[n];
            for(int z=0;z<n;z++){
                tempMatrix[z]=leaders[i][z];
            }
            for(int z=0;z<m;z++){
                if(matrix[j][z]==1){
                    for(int u=0;u<n;u++){
                        if(genMatrix[n*z+u]==1){
                            tempMatrix[u]=!tempMatrix[u];
                        }
                    }
                }
            }

            for(int z=0;z<n;z++){
                cout<<tempMatrix[z];
            }
            cout<<" ";

            int tempNum=binaryToInt(tempMatrix);
            //cout<<tempNum<<endl;
            standardTable[tempNum]=1;
        }
        cout<<endl;

    }

    cosetWieght=new int[cosetCount];
    syndrome = new bool*[cosetCount];
    for (int i = 0; i < cosetCount; ++i) {
        syndrome[i] = new bool[n-k];
        cosetWieght[i]=0;
    }

    //getting the coset weight and calulating the syndromes
    for(int i=0;i<cosetCount;i++){
        for (int j = 0; j < n; ++j) {
            cosetWieght[i]+=leaders[i][j];
        }

        for(int z=0;z<n-k;z++){
            int sum=0;
            for(int j=0;j<n;j++){
                sum+=contMatrix[z*n+j] & leaders[i][j];

            }
            syndrome[i][z]=sum%2;
        }

    }

    std::cout << "syndromes:"<< std::endl;
    for (int i = 0; i < cosetCount; ++i) {
        for(int j=0;j<n-k;j++){
            cout<<syndrome[i][j];
        }
        cout << " ";
        cout<<cosetWieght[i];
        cout << endl;
    }




    bool exit=false;
    int choice;
    while(true){
    cout<<"choose scenario:"<<endl;
    cin>>choice;
    bool* inputBool=new bool[k];
    bool* encoded;
    bool* distorted;
    bool* decoded;
    string mistake;

    //all the 3 scenarios can be choosen by typing either 1,2 or 3 otherwise the program exits
    switch(choice){
        case 1:{
            string input;
            valid=false;
            //checks if its a valid length binary string
            while(!valid){

                cout << "Enter a "<<k<< " long binary string: "<<endl;
                cin >> input;

                if(input.length() == k){
                    if(isValidBinaryString(input)){
                        valid=true;
                    }
                    else{
                        cout << "invalid format!" << endl;
                    }
                }
                else{
                    cout << "invalid length!" << endl;
                }
            }

            //converts char to bool
            for(int i=0;i<input.length();i++){
                if(input[i]=='1'){
                    inputBool[i]=1;
                }
                else{
                    inputBool[i]=0;
                }

            }

            encoded=encode(inputBool);


            cout << "encoded:  ";
            for(int i=0;i<n;i++){
                cout << (encoded[i]) << " ";
            }
            cout << endl;

            cout << "distorted:";
            distorted=distort(encoded);
            for(int i=0;i<n;i++){
                cout << (distorted[i]) << " ";
            }
            cout << endl;
            //calculates the amount of mistakes and marks their positions in a array
            int mistakes=0;
            for(int i=0;i<n;i++){
                cout << encoded[i]<<" "<<distorted[i]<<endl;
                if(encoded[i]!=distorted[i]){

                    if(encoded[i]==1){
                        mistake[i]='1';
                    }
                    else{
                        mistake[i]='0';
                    }
                    mistakes++;
                }
                else{
                    mistake[i]='-';
                }
            }
            cout<<"mistakes:"<<mistakes<<endl;
            for(int i=0;i<n;i++){
                cout<<mistake[i];
            }
            cout << endl;
            //bit correction loops until a non int is entered
            while(true){
                int mistakeInput;
                cout<<"enter the bit you want to fix: ";
                cout<<endl;
                cin>>mistakeInput;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore();
                    break;
                }
                else{
                    if(mistakeInput>0 && mistakeInput<=n){
                        distorted[mistakeInput-1]=!distorted[mistakeInput-1];
                        for(int i=0;i<n;i++){
                            cout<<distorted[i];
                        }
                        cout<<endl;
                    }
                    else{
                        cout<<"bit out of bounds"<<endl;
                    }
                }
            }
            decoded=decode(distorted);
            cout<<"decoded: ";
            for(int i=0;i<k;i++){
                cout<<decoded[i];
            }
            cout << endl;




            break;
        }
        case 2:{
            string input;
            std::cout << "Enter a string: ";
            cin.get();
            getline(cin, input);

            int textLength=input.length()*8;

            int curStr=0;
            bool* stringEncoded=new bool[textLength];
            bool* stringDistorted=new bool[textLength];
            bool* stringDecoded=new bool[textLength];


            //converts the text to binarry
            for (char& c : input) {
                std::bitset<8> b(c);
                for(int i=0;i<8;i++){
                    stringEncoded[curStr*8+i]=b[i];
                }
                curStr++;
            }




            //divides the binary into k size pieces and encodes them
            for(int i=0;i<textLength/k;i++){
                for(int j=0;j<k;j++){
                    inputBool[j]=stringEncoded[i*k+j];
                }
                encoded=encode(inputBool);
                distorted=distort(encoded);
                decoded=decode(encoded);
                for(int j=0;j<k;j++){
                    stringDecoded[i*k+j]=decoded[j];
                    //the distorted data is also saved
                    stringDistorted[i*k+j]=distorted[j];
                }
            }
            //the leftover after division is also encoded and decoded
            if(textLength%k!=0){
                for(int j=0;j<textLength%k;j++){
                    inputBool[j]=stringEncoded[textLength/k*k+j];
                }
                for(int j=textLength%k;j<k;j++){
                    inputBool[j]=0;
                }
                encoded=encode(inputBool);
                decoded=distort(encoded);
                decoded=decode(decoded);
                for(int j=0;j<textLength%k;j++){
                    stringDecoded[textLength/k*k+j]=decoded[j];
                    //the distorted data is also saved
                    stringDistorted[textLength/k*k+j]=distorted[j];
                }
            }
            cout << endl;
                cout<<"unencoded text: ";
                for(int i=0;i<textLength/8;i++){
                    std::bitset<8> bits;
                    for (int j = 0; j < 8; ++j) {
                        bits[j] = stringDistorted[i*8 + j];
                    }
                    cout<<static_cast<char>(bits.to_ulong());
            }

            cout << endl;
            cout<<"decoded text: ";
            for(int i=0;i<textLength/8;i++){
                std::bitset<8> bits;
                for (int j = 0; j < 8; ++j) {
                    bits[j] = stringDecoded[i*8 + j];
                }
                cout<<static_cast<char>(bits.to_ulong());
            }
            cout << endl;

            break;
        }
        case 3:{
            imageToBinary();
            modifyImage() ;
            modifiedToImage("modified_output.bin","output_image.bmp");
            modifiedToImage("distorted_output.bin","distorted_image.bmp");
            break;
        }
        default:
            exit=true;
            break;
        }
        if(exit){
            break;
        }
    }

    return 0;
}

bool* encode(bool* input){
    bool* encoded=new bool[n];

    for(int i=0;i<n;i++){
        encoded[i]=0;
    }


    //multiplication with the gen matrix
    for(int i=0;i<n;i++){
        int sum=0;
        for(int j=0;j<k;j++){
            sum+=genMatrix[j*n+i] * input[j];
        }
        sum=sum%2;
        if(sum==1){
            encoded[i]=1;
        }
    }
    return encoded;
}

bool* distort(bool* encoded){
    bool* distorted=new bool[n];
    for(int i=0;i<n;i++){
        double random_number = static_cast<double>(rand()) / RAND_MAX;
        //changed bit if smaller than the determined random threshold
        if(random_number < pe){
            distorted[i]=!encoded[i];
        }
        else{
            distorted[i]=encoded[i];
        }
    }
    return distorted;
}

bool* decode(bool* encoded){
    bool* decodedSyndrome=new bool[n-k];
    int syndromWeight=-1;
    int newWeight=-1;

    bool* changed=new bool[n];

    //multiplication with the control matrix
    for(int z=0;z<n-k;z++){
            int sum=0;
            for(int j=0;j<n;j++){
                sum+=contMatrix[z*n+j] & encoded[j];
            }
            decodedSyndrome[z]=sum%2;
    }

    //finding which coset leader has the same syndrome and getting their weigth
    for(int i=0;i<cosetCount;i++){
        for(int j=0;j<n-k;j++){
            if(decodedSyndrome[j]!=syndrome[i][j]){
                break;
            }
            else if(j==n-k-1){
                syndromWeight=cosetWieght[i];
            }
        }
        if(syndromWeight!=-1){
            break;
        }
    }
    //if the weight is 0 then its the correct code
    if(syndromWeight!=0){
        for(int i=0;i<n;i++){
            //changing the bit i
            for(int j=0;j<n;j++){
                changed[j]=encoded[j];
                if(i==j){
                    changed[j]=!changed[j];
                }
            }
            //calculating its syndrome
            for(int z=0;z<n-k;z++){
                    int sum=0;
                    for(int j=0;j<n;j++){
                        sum+=contMatrix[z*n+j] & changed[j];
                    }
                    decodedSyndrome[z]=sum%2;
            }
            //checking the weight
            for(int z=0;z<cosetCount;z++){
                for(int j=0;j<n-k;j++){
                    if(decodedSyndrome[j]!=syndrome[z][j]){
                        break;
                    }
                    else if(j==n-k-1){
                        newWeight=cosetWieght[z];
                    }
                }
                if(newWeight!=-1){
                    break;
                }
            }
            //if the weight is smaller change the vector being decoded to it
            if(newWeight<syndromWeight){
                encoded=changed;
                syndromWeight=newWeight;
                if(syndromWeight==0){
                    break;
                }
                //removed as its unnecessary
                /*
                else{
                    i=0;
                }
                */
            }


        }
    }

    return encoded;
}

bool* createGenMatrix() {
    bool* bits=new bool[bitsetSize];
    for(int i=0;i<bitsetSize;i++){
        bits[i]=0;
    }
    //identity matrix
    for(int i=0;i<k;i++){
        bits[i*n+i]=1;
    }
    //randomixed bits
    for(int i=0;i<k;i++){
        for(int j=k;j<n;j++){
            int randomNum = rand() % 2;
            if(randomNum == 1){
                bits[i*n+j]=1;
            }
        }
    }
    return bits;
}

bool* createControlMatrix(bool* bits) {
    bool* cont=new bool[controlSize];
    for(int i=0;i<controlSize;i++){
        cont[i]=0;
    }
    //the flipped part
    for(int i=0;i<n-k;i++){
        for(int j=0;j<k;j++){
            cont[i*n+j]=bits[j*n+(k+i)];
        }
    }
    //itentity matrix
    for(int i=0;i<n-k;i++){
        cont[i*n+k+i]=1;
    }
    return cont;
}

//the string is binary
bool isValidBinaryString(string& str) {
    for (char c : str) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}

//generates all possible binary vectors of the given size
bool** GenerateAllVectors(int m) {
    int rows=exp2(m);
    bool** matrix = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new bool[m];
        for (int j = 0; j < m; ++j) {
            matrix[i][j]=0;
        }
    }
    //get all the possible vectors trough binary addition
    for(int i=1;i<rows;i++){
        for (int j = 0; j < m; ++j) {
            matrix[i][j]=matrix[i-1][j];
        }
        for (int j = 0; j < m; ++j) {
            if(matrix[i][j]==0){
                matrix[i][j]=1;
                break;
            }
            else{
                matrix[i][j]=0;
            }
        }
    }
    return matrix;
}


//generates all vectors of the given size m and the given weight of w converted to decimal
std::vector<int> generateVectorOfWeight(int m, int w) {
    std::vector<int> numbers;
    fillVector(numbers, w, 0, 0);
    return numbers;
}

void fillVector(std::vector<int>& vec, int w,  int j, int sum) {
    int temp;
    // position to n-(the next weight) so there would always remains enough space for all the bits of the given weight,
    //then calls itself recursively with a decreased weight and a sets starting position +1 from the placed position
    //when the weight of 1 is reached the final bit is found and the sum of all the bit position values converted to decimal are added to the vector

    for(int i=j;i<n-w+1;i++){
        temp=sum+exp2(i);
        if(w==1){
            vec.push_back(temp);
        }
        else{
            fillVector(vec, w-1, i+1, temp);
        }
    }
}

//integer to binary
bool* intToBinary(int num) {
    string binary = "";
    for (int i = 0; i < n; i++) {
        binary+=(num & (1 << i)) ? '1' : '0';
    }
    bool* matrix=new bool[n];
    for(int i=0;i<n;i++){
        if(binary[i]=='1'){
            matrix[i]=1;

        }
        else{
            matrix[i]=0;
        }
    }
    return matrix;
}

//binary to integer
int binaryToInt(bool* num){
    int sum=0;
    for(int i=0;i<n;i++){
        if(num[i]==1){
            sum+=exp2(i);
        }
    }
    return sum;
}

void imageToBinary(){
    std::string inputFile = "dog2.bmp";
    const std::string outputFile = "output.bin";

    cout<<"enter file name:";
    cin>>inputFile;

    // Open BMP file for reading
    std::ifstream bmpFile(inputFile, std::ios::binary);
    if (!bmpFile) {
        std::cerr << "Error: Could not open the input BMP file!" << std::endl;
    }

    // Read BMP header
    BMPHeader header = readBMPHeader(bmpFile);


    // Check BMP file type
    if (header.fileType != 0x4D42) { // 'BM' in little-endian
        std::cerr << "Error: Not a valid BMP file!" << std::endl;
    }

    // Seek to the pixel data
    bmpFile.seekg(header.offsetData, std::ios::beg);

    // Read the pixel data
    std::vector<uint8_t> pixelData(header.imageSize);
    bmpFile.read(reinterpret_cast<char*>(pixelData.data()), header.imageSize);
    bmpFile.close();

    // Write the pixel data to a binary file
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open the output binary file!" << std::endl;
    }

    // Write BMP metadata to binary file
    outFile.write(reinterpret_cast<const char*>(&header.width), sizeof(header.width));
    outFile.write(reinterpret_cast<const char*>(&header.height), sizeof(header.height));
    outFile.write(reinterpret_cast<const char*>(&header.bitCount), sizeof(header.bitCount));

    // Write pixel data
    outFile.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

    outFile.close();

    std::cout << "Image successfully written to binary file!" << std::endl;
}

int modifyImage() {
    const std::string inputFile = "output.bin";  // Binary file created earlier
    const std::string modifiedFile = "modified_output.bin";
    const std::string modifiedFile2 = "distorted_output.bin";

    // Open binary file for reading
    std::ifstream binFile(inputFile, std::ios::binary);
    if (!binFile) {
        std::cerr << "Error: Could not open the input binary file!" << std::endl;
        return 1;
    }

    // Read image dimensions and pixel metadata
    int32_t width, height;
    uint16_t bitCount;
    binFile.read(reinterpret_cast<char*>(&width), sizeof(width));
    binFile.read(reinterpret_cast<char*>(&height), sizeof(height));
    binFile.read(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));

    // Calculate pixel data size
    int bytesPerPixel = bitCount / 8; // e.g., 24-bit BMP -> 3 bytes per pixel
    int imageSize = width * height * bytesPerPixel;

    // Read pixel data
    std::vector<uint8_t> pixelData(imageSize);
    std::vector<uint8_t> pixelData2(imageSize);
    binFile.read(reinterpret_cast<char*>(pixelData.data()), imageSize);
    binFile.close();
    pixelData2=pixelData;

    bool* u8Bin=new bool[8];
    bool* u24Bin=new bool[24];
    bool* Distortedu24Bin=new bool[24];
    bool* input=new bool[k];
    bool* encoded=new bool[n];
    bool* distorted=new bool[n];
    bool* decoded=new bool[n];

    // Modify pixel data
    for (size_t i = 0; i < pixelData.size(); i += bytesPerPixel) {
        for(int v=0;v<3;v++){
            uint8ToBoolArray(pixelData[i+v], u8Bin);
            for(int j=0;j<8;j++){
                u24Bin[v*8+j]=u8Bin[j];
            }
        }
        //every 24 color bits are divided into k parts encoded, distorted and decoded
        for(int z=0;z<24/k;z++){
            for(int j=0;j<k;j++){
                input[j]=u24Bin[z*k+j];
            }
                encoded=encode(input);
                distorted=distort(encoded);
                decoded=decode(distorted);
            for(int j=0;j<k;j++){
                    u24Bin[z*k+j]=decoded[j];
                    //the distorted data is also saved
                    Distortedu24Bin[z*k+j]=distorted[j];
            }
        }
        //the same is done with the remainder by adding extra zeroes
        if(24%k!=0){
            for(int j=0;j<24%k;j++){
                input[j]=u24Bin[24/k*k+j];
            }
            for(int j=24%k;j<k;j++){
                input[j]=0;
            }
            encoded=encode(input);
            distorted=distort(encoded);
            decoded=decode(distorted);
            for(int j=0;j<24%k;j++){
                u24Bin[24/k*k+j]=decoded[j];
                //the distorted data is also saved
                Distortedu24Bin[24/k*k+j]=distorted[j];
            }
        }

        for(int v=0;v<3;v++){
            uint8ToBoolArray(pixelData[i+v], u8Bin);
            for(int j=0;j<8;j++){
                u8Bin[j]=u24Bin[v*8+j];
            }
            pixelData[i+v] = boolArrayToUint8(u8Bin);
        }

        for(int v=0;v<3;v++){
            for(int j=0;j<8;j++){
                u8Bin[j]= Distortedu24Bin[v*8+j];
            }
            pixelData2[i+v] = boolArrayToUint8(u8Bin);
        }


    }

    // Write modified data to a new binary file
    std::ofstream outFile(modifiedFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open the output binary file!" << std::endl;
        return 1;
    }

    // Write metadata
    outFile.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outFile.write(reinterpret_cast<const char*>(&height), sizeof(height));
    outFile.write(reinterpret_cast<const char*>(&bitCount), sizeof(bitCount));

    // Write modified pixel data
    outFile.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

    outFile.close();

    //the distorted data to binary
    std::ofstream outFile2(modifiedFile2, std::ios::binary);
    if (!outFile2) {
        std::cerr << "Error: Could not open the output binary file!" << std::endl;
        return 1;
    }

    // Write metadata
    outFile2.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outFile2.write(reinterpret_cast<const char*>(&height), sizeof(height));
    outFile2.write(reinterpret_cast<const char*>(&bitCount), sizeof(bitCount));

    // Write modified pixel data
    outFile2.write(reinterpret_cast<const char*>(pixelData2.data()), pixelData2.size());

    outFile2.close();

    std::cout << "Pixel data successfully modified and written to binary file!" << std::endl;

    return 0;
}

int modifiedToImage(string binaryFile,string outputImage) {

    // Open binary file for reading
    std::ifstream binFile(binaryFile, std::ios::binary);
    if (!binFile) {
        std::cerr << "Error: Could not open the binary file!" << std::endl;
        return 1;
    }

    // Read metadata
    int32_t width, height;
    uint16_t bitCount;
    binFile.read(reinterpret_cast<char*>(&width), sizeof(width));
    binFile.read(reinterpret_cast<char*>(&height), sizeof(height));
    binFile.read(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));

    // Validate bit depth (only handling 24-bit BMP here)
    if (bitCount != 24) {
        std::cerr << "Error: Only 24-bit images are supported in this example!" << std::endl;
        return 1;
    }

    // Calculate pixel data size
    int bytesPerPixel = bitCount / 8; // 24-bit BMP -> 3 bytes per pixel
    int rowStride = (width * bytesPerPixel + 3) & ~3; // Row size padded to 4 bytes
    int imageSize = rowStride * height;

    // Read pixel data
    std::vector<uint8_t> pixelData(imageSize);
    binFile.read(reinterpret_cast<char*>(pixelData.data()), imageSize);
    binFile.close();

    // Prepare BMP header
    BMPHeader2 header;
    header.width = width;
    header.height = height;
    header.imageSize = imageSize;
    header.fileSize = header.offsetData + imageSize;

    // Open output BMP file for writing
    std::ofstream outFile(outputImage, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open the output BMP file!" << std::endl;
        return 1;
    }

    // Write BMP header
    outFile.write(reinterpret_cast<const char*>(&header.fileType), sizeof(header.fileType));
    outFile.write(reinterpret_cast<const char*>(&header.fileSize), sizeof(header.fileSize));
    outFile.write(reinterpret_cast<const char*>(&header.reserved), sizeof(header.reserved));
    outFile.write(reinterpret_cast<const char*>(&header.offsetData), sizeof(header.offsetData));
    outFile.write(reinterpret_cast<const char*>(&header.size), sizeof(header.size));
    outFile.write(reinterpret_cast<const char*>(&header.width), sizeof(header.width));
    outFile.write(reinterpret_cast<const char*>(&header.height), sizeof(header.height));
    outFile.write(reinterpret_cast<const char*>(&header.planes), sizeof(header.planes));
    outFile.write(reinterpret_cast<const char*>(&header.bitCount), sizeof(header.bitCount));
    outFile.write(reinterpret_cast<const char*>(&header.compression), sizeof(header.compression));
    outFile.write(reinterpret_cast<const char*>(&header.imageSize), sizeof(header.imageSize));
    outFile.write(reinterpret_cast<const char*>(&header.xPixelsPerMeter), sizeof(header.xPixelsPerMeter));
    outFile.write(reinterpret_cast<const char*>(&header.yPixelsPerMeter), sizeof(header.yPixelsPerMeter));
    outFile.write(reinterpret_cast<const char*>(&header.colorsUsed), sizeof(header.colorsUsed));
    outFile.write(reinterpret_cast<const char*>(&header.colorsImportant), sizeof(header.colorsImportant));

    // Write pixel data
    outFile.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());
    outFile.close();

    std::cout << "Binary file successfully converted to BMP image!" << std::endl;

}

void uint8ToBoolArray(uint8_t value, bool* boolArray) {
     for (int i = 0; i < 8; ++i) {
            boolArray[i] = (value & (1 << (7 - i))) != 0;
    }
}

uint8_t boolArrayToUint8(const bool* boolArray) {
     uint8_t value = 0; for (int i = 0; i < 8; ++i) {
          value |= (boolArray[i] << (7 - i));
    } return value;
}

bool* readGenMatrix() {
    const char* filename = "data.txt";

    // Open the text file
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 0;
    }

    // Vector to hold integers
    std::vector<int> integers;

    // Read integers from the file
    int value;
    while (inputFile >> value) { // Reads integers separated by spaces or newlines
        integers.push_back(value);
    }

    // Close the file
    inputFile.close();


    bool* genMatric=new bool[bitsetSize];

    if(integers.size()!=bitsetSize){
        cout<<"incorect size"<<endl;
        return 0;
    }


    for (int i = 0; i < bitsetSize; ++i) {
            if(integers[i]==1){
                genMatric[i]=1;
            }
            else if(integers[i]==0){
                genMatric[i]=0;
            }
            else{
                cout<<"the gen matrix is not binary";
            }
    }



    return genMatric;
}
