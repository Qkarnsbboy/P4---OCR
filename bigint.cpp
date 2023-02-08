#include <iostream>
#include "bigint.h"

using namespace std;

// Write your implementation below

  //BigInt
  //constructor --> convert string to a vector of ints using given base
BigInt::BigInt(string s, int base)
{
  //store base
  base_ = base;

  //get size of string
  int size = s.size();

  //loop through string backwards (starting with last value)
  for (int i = size-1; i >= 0; i--)
  {
    char curr = s[i];

    //if 0-9
    if ( (curr >= '0') && (curr <= '9') )
    {
      int x = static_cast<int>(curr) - '0';
      intVector.push_back(x);
    }
    else if ( (curr >= 'A') && (curr <= 'Z') ) //if Letter (A-Z)
    {
      int x = 10 + static_cast<int>(curr) - 'A';
      intVector.push_back(x);
    }
    
  }
  //remove leading zeroes
  removeLeadingZeroes();

}


  // to_string
  // get string representation
string BigInt::to_string() const
{
  //initialize string
  string word = "";

  //get size of BigInt
  int size = intVector.size();

  //loop through vector in reverse order
  for (int i = size-1; i >= 0; i--)
  {
    int curr = intVector[i];

    //if 0-9
    if ( (curr >= 0) && (curr <= 9 ) )
    {
      char char_digit = (char)(curr+'0');
      word += char_digit;
    }
    else
    {
      char char_letter = (char)(curr-10+'A');
      word += char_letter;
    }
  }

  return word;
}

// add another BigInt to this one //adding vectors
void BigInt::add(BigInt b)
{
  int size_a = intVector.size();
  int size_b = b.intVector.size();
  int base = base_;
  int carry = 0;

  //make sure sizes are the same
  
    //if a > b
  if (size_a > size_b)
  {
    int difference = size_a - size_b;

    for (int i = 0; i < difference; i++)
    {
      b.intVector.push_back(0); //adds 0 for each empty space
    }
  }
  if (size_a < size_b)
  {
    int difference = size_b - size_a;

    for (int i = 0; i < difference; i++)
    {
      intVector.push_back(0); //adds 0 for each empty space
    }
  }

    //add algorithm

  //loop through vector A
  for (size_t i = 0; i < intVector.size(); i++)
  {

    //get new values
    int aValue = intVector[i];
    int bValue = b.intVector[i];

    int sum = aValue + bValue + carry; //add carry as well

    //reset carry
    carry = 0;

    if (sum < base) //no carry needed
    {
      intVector[i] = sum;
    }
    else{ //carry needed //ex 13 --> carry = 1 --> stay = 3 (sum - base)
      carry = sum/base; //get carry
      intVector[i] = sum - base;
    }
  }

  //if final addition is bigger than base --> add enough digits to match
  if (carry != 0)
  {
    //add another digit
    intVector.push_back(carry);
  }

}

//remove leading zeroes
//use for 1) constructor and 2) subtraction (will leave you with leading zeroes)
void BigInt::removeLeadingZeroes()
{
  int size = intVector.size();
  bool startFound = false;

  if(size == 1) //size 1 vector do nothing
  {

  }
  else if (size > 1)
  {

    //loop through vector in reverse order until non-zero character found, won't delete if one 0 left
    for(int i = size-1; i>=1; i--)
    {
      if(intVector[i] != 0)
      {
        startFound = true;
      }

      //if still leading and a zero --> delete it by popping back
      if( !startFound && (intVector[i]==0))
      {
        intVector.pop_back();
      }
    }
  }

}


// returns the sum of this BigInt and rhs
BigInt BigInt::operator+(const BigInt& rhs) const
{
  //create copy BigInts
  BigInt tempa = *this;
  BigInt tempb = rhs;

  int size_a = tempa.intVector.size();
  int size_b = tempb.intVector.size();
  int base = base_;
  int carry = 0;

  //make sure sizes are the same
  
    //if a > b
  if (size_a > size_b)
  {
    int difference = size_a - size_b;

    for (int i = 0; i < difference; i++)
    {
      tempb.intVector.push_back(0); //adds 0 for each empty space
    }
  }
  if (size_a < size_b)
  {
    int difference = size_b - size_a;

    for (int i = 0; i < difference; i++)
    {
      tempa.intVector.push_back(0); //adds 0 for each empty space
    }
  }

    //add algorithm

  //loop through vector A
  for (size_t i = 0; i < tempa.intVector.size(); i++)
  {

    //get new values
    int aValue = tempa.intVector[i];
    int bValue = tempb.intVector[i];

    int sum = aValue + bValue + carry; //add carry as well

    //reset carry
    carry = 0;

    if (sum < base) //no carry needed
    {
      tempa.intVector[i] = sum;
    }
    else{ //carry needed //ex 13 --> carry = 1 --> stay = 3 (sum - base)
      carry = sum/base; //get carry
      tempa.intVector[i] = sum - base;
    }
  }

  //if final addition is bigger than base --> add enough digits to match
  if (carry != 0)
  {
    //add another digit
    tempa.intVector.push_back(carry);
  }

  return tempa;
}

// returns true if this BigInt is less than rhs
bool BigInt::operator<(const BigInt& rhs) const
{
  int sizeLHS = intVector.size();
  int sizeRHS = rhs.intVector.size();

  //1. check which has bigger digits
    //if LHS has less digits, return true
  if (sizeLHS < sizeRHS)
  {
    return true;
  }
  else if (sizeLHS > sizeRHS)
  {
    return false;
  }

   //2. if same digits, start checking digits (last digit of vectors is first digits usually)
  else{

    for (int i = sizeLHS-1; i>=0; i--)
    {
      int LHSDigit = intVector[i];
      int RHSDigit = rhs.intVector[i];

      if(LHSDigit < RHSDigit)
      {
        return true;
      }
      else if (LHSDigit > RHSDigit)
      {
        return false;
      }
      
    }
  }

 return false;
}

// returns the difference of this BigInt minus rhs
BigInt BigInt::operator-(const BigInt& rhs) const
{
  //1. make copies + get sizes + declare variables
  BigInt LHS = *this;
  BigInt RHS = rhs;

  int size_LHS = LHS.intVector.size();
  int size_RHS = RHS.intVector.size();
  int base = base_;
  int borrow = 0;

  //2. check if LHS is bigger than RHS therefore its valid
  if (LHS < RHS)
  {
    cout << "LHS must be bigger than RHS! Returning LHS" << endl;
    return LHS;
  }

  //3. add leading 0's
  if (size_LHS > size_RHS)
  {
    int difference = size_LHS - size_RHS;

    for (int i = 0; i < difference; i++)
    {
      RHS.intVector.push_back(0); //adds 0 for each empty space
    }
  }
  if (size_LHS < size_RHS)
  {
    int difference = size_RHS - size_LHS;

    for (int i = 0; i < difference; i++)
    {
      LHS.intVector.push_back(0); //adds 0 for each empty space
    }
  }

  //4. loop through vectors in reverse
  for (int i = 0; i < size_LHS; i++)
  {
    

    //get new values
    int LHSValue = LHS.intVector[i];
    int RHSValue = RHS.intVector[i];

    //compute the difference
    int difference = LHSValue - RHSValue - borrow;

    //reset borrow
    borrow = 0;
    
    //determine if borrow needed (negative number)
    if (difference >= 0)
    {
      LHS.intVector[i] = difference;
    }
    else{ //borrow needed //ex -3 --> carry = 1 --> stay = 10-3 (base + difference))
      borrow = 1; //get carry
      LHS.intVector[i] = base + difference;
    }

  }

  //remove leading zeroes
  LHS.removeLeadingZeroes();

  //return
  return LHS;
}