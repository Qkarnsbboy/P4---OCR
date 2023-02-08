#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location& other) const
{
    if (this->col < other.col) //check column
    {
        return true;
    }
    else if (this->col > other.col)
    {
        return false;
    }
    else //columns are same
    {
        if (this->row < other.col)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    cout << "Must be exact same --> returning false" << endl;
    return false;
}

// TO DO - Complete this function
DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initilaize h_ and w_ and any other data members
    h_ = 0; //default
    w_ = 0; //default

    //add more for hsym or vsym
    vCenter_ = 0;
    hCenter_ = 0;
    vSym_ = 0;
    hSym_ = 0;
}

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    ul_.row = upperleft.row;
    ul_.col = upperleft.col;

    h_ = height;
    w_ = width;

    //add more for other data members
    vCenter_ = 0;
    hCenter_ = 0;
    vSym_ = 0;
    hSym_ = 0;


}

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob()
{
    // Add code if it is necessary

}

// TO DO - Complete this function
void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_centers_of_mass();
    calc_symmetry();

    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number but you are free to
    //    change our structure

    //Euler
    if (euler_ == -1) //8
    {
      digit_ = '8'; 
    }
    else if (euler_ == 0) //0,4,6,9
    {
      //symmetry
      if ( (vSym_ > .750) && (hSym_ >.750) ) //0 is most symmetric
      {
        digit_ = '0';
      }
      else if (hSym_ < .670) //4 is least horizontally symmetric
      {
        digit_ = '4';
      }
      else if (vCenter_ < .500) //9 has higher center of mass
      {
        digit_ = '9';
      }
      else
      {
        digit_ = '6';
      }
    }
    else if (euler_ == 1) //1,2,3,5,7
    {
      //symmetry
      if (vCenter_ < .425) //7 has the highest center of mass
      {
        digit_ = '7';
      }
      else if ((hSym_ >.800 )) //1 has the highest horizontal symmetry
      {
        digit_ = '1';
      }
      else if (vSym_ > 0.7) //3 has a higher vertical symmetry
      {
        digit_ = '3';
      } 
      else if (vCenter_ < .500) //5 has inbetween mass
      {
        digit_ = '5';
      }
      else if (vCenter_ > .555) //1 has higher vertical center of mass (italics)
      {
        digit_ = '1';
      }
      else if (vCenter_ < .520) //3 has lower italicized vertical center of mass
      {
        digit_ = '3';
      }
      else
      {
        digit_ = '2';
      }

    }



}

// Complete - Do not alter
char DigitBlob::getClassification() const
{
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data

    //center of mass
    cout << "Vertical Center is: " << vCenter_ << endl;
    cout << "Horizontal Center is: " << hCenter_ << endl;

    //symmetry
    cout << "Vertical Symmetry is: " << vSym_ << endl;
    cout << "Horizontal Symmemtry is: " << hSym_ << endl;



    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

// Complete - Do not alter
const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const
{
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const
{
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads()
{
  Location tempUL(ul_.row-1, ul_.col-1); //makes sure to start on a blank row
                              //loop ends at second to last column and second to last row

  //1. create loop to iterate through each bounding box
  for(int i = 0; i < h_+1; i++)
  {
    for (int j = 0; j < w_+1; j++)
    {
      //create four different locations
      Location topLeft;
      topLeft.row= tempUL.row + i; //
      topLeft.col = tempUL.col + j;

      Location topRight(topLeft.row, topLeft.col+1);
      Location botLeft(topLeft.row+1, topLeft.col);
      Location botRight(topLeft.row+1, topLeft.col+1);

      //count black pixels
      int numberBlack = 0;

      //get values at locations
      int topLeftVal = img_[topLeft.row][topLeft.col];
      if(topLeftVal == 0)
      {
        numberBlack++;
      }

      int topRightVal = img_[topRight.row][topRight.col];
      if(topRightVal == 0)
      {
        numberBlack++;
      }

      int botLeftVal = img_[botLeft.row][botLeft.col];
      if(botLeftVal == 0)
      {
        numberBlack++;
      }

      int botRightVal = img_[botRight.row][botRight.col];
      if(botRightVal == 0)
      {
        numberBlack++;
      }

      //logic for classifying bit quads (0=black) (!=0 white)
      if (numberBlack == 0)
      {
        //bitquad0
        bq0_++;
      }
      else if (numberBlack == 4)
      {
        //bitquad4
        bq4_++;
      }
      else if (numberBlack == 1)
      {
        //bitquad1
        bq1_++;
      }
      else if (numberBlack == 3)
      {
        //bitquad 3
        bq3_++;
      }
      //2 black pixels can be bq2 or bqd_
      else if ( (topLeftVal==0) && (botRightVal==0) )
      {
        //bitquad d
        bqd_++;
      }
      else if ( (botLeftVal==0) && (topRightVal==0))
      {
        //bitquad d
        bqd_++;
      }
      else //must be a bq2
      {
        bq2_++;
      }
 

    }
  } 

    
}

// Add more private helper function implementations below
void DigitBlob::calc_centers_of_mass()
{
  //vCenter_
  int numBlack = 0; //counts total black pixels
  int sumBlack = 0; //weighted avg by row

  for(int i = 0; i < h_; i++)
  {
    int blackRow = 0;

    for (int j = 0; j < w_; j++)
    {
      
      if(img_[ul_.row+i][ul_.col+j]==0)
      {
        numBlack++; 
        blackRow++;
      }

      
    }

    sumBlack += (blackRow * i); //wavg of row
  }

  double tempvCenter = sumBlack / (double) numBlack;

  vCenter_ = tempvCenter / (h_-1);

  int sumBlackH = 0;
  //hCenter_
  for(int j = 0; j < w_; j++) //by col
  {
    int blackCol = 0;
    for (int i = 0; i < h_; i++)
    {
      if(img_[ul_.row + i][ul_.col + j] == 0)
      {
        blackCol++;
      }

      
    }

    sumBlackH += (blackCol * j);
  }


  double temphCenter = sumBlackH / (double) numBlack;

  hCenter_ = temphCenter / (w_-1);
}

void DigitBlob::calc_symmetry()
{
  double numPixelsW = 0;
  int vMatch = 0;
  
  //vertical symmetry
  for (int i = 0; i <(h_/2) ; i++) //only checks top half
  {
    for (int j = 0; j<w_; j++)
    {
      if( img_[ul_.row+i][ul_.col+j] == img_[ ul_.row +(h_-i-1) ][ul_.col+j])
      {
        vMatch++;
      }
      numPixelsW++;
    }
  }

  vSym_ = vMatch / numPixelsW;

  double numPixelsH = 0;

  int hMatch = 0;
  //horizontal symmetry
  for (int i = 0; i < h_; i++)
  {
    for (int j = 0; j < (w_/2); j++) //only checks left side
    {
      if( img_[ul_.row+i][ul_.col+j] == img_[ul_.row +i][ul_.col+(w_-j-1)] )
      {
        hMatch++;
      }
      numPixelsH++;

    }
  }

  hSym_ = hMatch / numPixelsH;
}
