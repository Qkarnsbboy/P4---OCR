#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;

// TO DO: Complete this function
NumImg::NumImg(const char* bmp_filename)
{
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)

    // TO DO:
    // call readGSBMP to initialize img_, h_, and w_;
    
    //calls readGSBMP
    img_ = readGSBMP(bmp_filename, h_, w_);

    // Leave this check
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }
    // Perform any other initialization you need
    
}

// TO DO: Complete this function
NumImg::~NumImg()
{
    // Add code here if necessary

    //deallocate img_
    for(int i=0; i < h_; i++) {
        delete [] img_[i];
    }

    delete[] img_;

}

// TO DO: Complete this function
size_t NumImg::findAndCreateDigitBlobs()
{
    //1. initialize values

    //2. create explored array (dynamically)
    bool** explored = new bool*[h_];
    for (int i = 0; i < h_; i++)
    {
        explored[i] = new bool[w_];
    }

      //initialize explored array
    for (int i = 0; i < h_; i++)
    {
      for (int j = 0; j < w_; j++)
      {
        explored[i][j] = false;
      }
    }

    //3. scan through array until a black digit is found and not in the explored array
    for (int i = 0; i < h_; i++)
    {
        for (int j = 0; j < w_; j++)
        {
            if( (img_[i][j]==0) && (explored[i][j]==false) )
            {
                //4. call BFS function which returns a new digitBlob (with UpperLeft location and dimensions), now add it
                DigitBlob newDigitBlob = createDigitBlob(explored, i, j);

                //add it to blobs vector
                blobs_.push_back(newDigitBlob);

            }
        }
    }


    //5. sort digit blobs based on upper-left-column coordinate
    sortDigitBlobs();


    //5.5 deallocated explored
    for(int i = 0; i <h_; i++)
    {
        delete [] explored[i];
    }
    delete [] explored;

    //6. return size of blobs_
    return blobs_.size();

    
}

// Complete - Do not alter
std::string NumImg::classify(bool withDebug)
{
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}

// Complete - Do not alter
void NumImg::printBoundingBoxes() const
{
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }

}

// Complete - Do not alter
const DigitBlob& NumImg::getDigitBlob(size_t i) const
{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}

// Complete - Do not alter
size_t NumImg::numDigitBlobs() const
{
    return blobs_.size();
}

// Complete - Do not alter
void NumImg::sortDigitBlobs()
{
    std::sort(blobs_.begin(), blobs_.end());
}

// Complete - Do not alter
void NumImg::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < blobs_.size(); i++){
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for(int i = ul.row-1; i < ul.row + h + 1; i++){
            img_[i][ul.col-1] = 128;
            img_[i][ul.col+w] = 128;
        }
        for(int j = ul.col-1; j < ul.col + w + 1; j++){
            img_[ul.row-1][j] = 128;
            img_[ul.row+h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}

// TO DO:
// Add other (helper) function definitions here

//BFS search function
DigitBlob NumImg::createDigitBlob(bool** explored, int pr, int pc)
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here

    //1. initialize values
    int minRow = pr;
    int minCol = pc;
    int maxRow = pr;
    int maxCol = pc;

    
    deque<Location> locDeque;

    Location startLocation;
    startLocation.row = pr;
    startLocation.col = pc;

    //2. mark start location as explored
    explored[pr][pc] = true;

    //3. add start location to queue
    locDeque.push_back(startLocation);

    //4. while queue is not empty
    while(!locDeque.empty())
    {

      //extract item from front of queue
      Location curr = locDeque.front();
      locDeque.pop_front();
          
        //5. for loop for each neighbor location
        for (int i = 0; i < 8; i++)
        {
            //set neighbor
            //N, NW, W, SW, S, SE, E, NE
            Location neighbor;

            //verify its in bounds 
            if(curr.row + neighbor_row[i] < 0)
            {
              neighbor.row = curr.row; //set it equal to current valid row
            }
            else if (curr.row + neighbor_row[i] >= h_)
            {
              neighbor.row = curr.row; //set it equal to current valid row
            }
            else
            {
              neighbor.row = curr.row + neighbor_row[i];
            }

            if(curr.col + neighbor_col[i] < 0)
            {
              neighbor.col = curr.col; //set it equal to current valid column
            }
            else if (curr.col + neighbor_col[i] >= w_)
            {
              neighbor.col = curr.col; //set it equal to current valid column
            }
            else
            {
              neighbor.col = curr.col + neighbor_col[i];
            }

            //if neighbor is black and not explored
            if( (img_[neighbor.row][neighbor.col]==0) && (explored[neighbor.row][neighbor.col]==false))
            {
                //mark neighbor as explored
                explored[neighbor.row][neighbor.col] = true;

                //add neighbor to back of queue
                locDeque.push_back(neighbor);

                //update max and min col/rows
                if(neighbor.row > maxRow)
                {
                    maxRow = neighbor.row;
                }
                else if( (neighbor.row < minRow) && (neighbor.row >= 0) )
                {
                    minRow = neighbor.row;
                }

                if (neighbor.col > maxCol)
                {
                    maxCol = neighbor.col;
                }
                else if ( ( neighbor.col < minCol) && (neighbor.col >= 0) )
                {
                    minCol = neighbor.col;
                }

            }

        } //end of for loop
    } //end of while loop

    //6. create digitBlob object and return it
    Location upperLeft;
    upperLeft.row = minRow;
    upperLeft.col = minCol;

    //constructor (img_, Ul, h_, w_)
    DigitBlob digitblob(img_, upperLeft, maxRow - minRow + 1, maxCol - minCol + 1);

    

    //digitblob.ul_ = upperLeft;
    //digitblob.h_ = maxRow - minRow + 1;
    //digitblob.w_ = maxCol - minCol + 1;

    return digitblob;

}

